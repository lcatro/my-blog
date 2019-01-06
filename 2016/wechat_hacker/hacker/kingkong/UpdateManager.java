package com.tencent.kingkong;

import android.content.Context;
import android.text.TextUtils;

import com.tencent.kingkong.Common.Log;
import com.tencent.kingkong.MainConfig.PatchInfo;
import com.tencent.kingkong.MainConfig.BasicPatchInfo;

import java.io.File;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class UpdateManager {
    public static final String LOG_TAG = "KingKongUpdateManager";

    private static Context mContext = null;

    private static final String STATUS_IDLE = "IDLE";
    private static final String STATUS_UPDATING = "UPDATING";
    private static final String STATUS_UPDATE_FAILED = "MAYDAY";
    private static final String STATUS_INSTALLING = "INSTALLING";

    private static final long MAX_FORCE_REUPDATE_TIME = 15 * 60 * 1000;     // 15 minutes
    //private static final long MAX_UPDATE_INTERVAL = 1 * 24 * 60 * 60 * 1000; // 1 day
    private static final long MAX_UPDATE_INTERVAL = 60 * 1000; // 1 minute for test

    private static final boolean ENABLE_OFFLINE_TEST = false;
    private static final String MAIN_CONFIG_APK_NAME = "patches.cfg.apk";

    private static String mPatchDownloadFolder = "";
    private static String mPatchFolder = "";
    private static String mSignature = "";

    private static String mPackageName = "";
    private static String MAIN_CONFIG_FILE_NAME = "";
    // TODO 测试服务器链�?
    private static String MAIN_CONFIG_URL = "http://10.208.168.172:8080/config?b=";
//    private static String MAIN_CONFIG_URL = "http://zyjc.sec.qq.com/config?b=";
    private static String MAIN_CONFIG_URL_OFFLINE = "/sdcard/";

    private static Utils.InterProcessLock mUpdateInterProcessLock = null;
    private static Utils.InterProcessLock mMainInterProcessLock = null;

    public static void InitUpdateManager(Context context) {
        mContext = context;
        mPatchDownloadFolder = Common.GetDownloadFolder();
        mPatchFolder = Common.GetPatchFolder();
        mSignature = Common.GetSignature();
        mUpdateInterProcessLock = new Utils.InterProcessLock(Common.GetUpdateInterProcessLockFile());
        mMainInterProcessLock = new Utils.InterProcessLock(Common.GetMainInterProcessLockFile());

        mPackageName = Utils.getPackageName(mContext);
        MAIN_CONFIG_URL = MAIN_CONFIG_URL + mPackageName;
        MAIN_CONFIG_URL_OFFLINE = MAIN_CONFIG_URL_OFFLINE + mPackageName + ".apk";
        MAIN_CONFIG_FILE_NAME = mPackageName + ".cfg";
    }

    public static void Update() {

        setUpdateStatus(STATUS_UPDATING);

        new Thread(new Runnable() {

            @Override
            public void run() {

                // TODO : Does this lock necessary?
                if (!mUpdateInterProcessLock.lock()) {
                    setUpdateStatus(STATUS_UPDATE_FAILED);
                    return;
                }

                doClear();
                if (!doUpdate()) {
                    setUpdateStatus(STATUS_UPDATE_FAILED);
                    doClear();
                }

                mUpdateInterProcessLock.unlock();
            }
        }).start();
    }

    public static void InstallUpdates() {
        new Thread(new Runnable() {

            @Override
            public void run() {

                // TODO : Does this lock necessary?
                if (!mUpdateInterProcessLock.lock()) {
                    setUpdateStatus(STATUS_UPDATE_FAILED);
                    return;
                }

                if (!doInstallUpdates()) {
                    setUpdateStatus(STATUS_UPDATE_FAILED);
                    // Clear all installed patches
                    Utils.ZapFolder(new File(mPatchFolder));
                } else {
                    // Well done!
                    setUpdateStatus(STATUS_IDLE);
                    Log.d(LOG_TAG, "Install updates success!");
                }

                doClear();
                mUpdateInterProcessLock.unlock();
            }
        }).start();
    }

    private static boolean doClear() {
            return true;
            //File downloadFolder = new File(mPatchDownloadFolder);
            //return Utils.ZapFolder(downloadFolder);
        }

    private static boolean doInstallUpdates() {
        String mainConfigFile = Common.makeDownloadName(MAIN_CONFIG_FILE_NAME);
        MainConfig mainConfig = MainConfig.parseFromFile(mainConfigFile);

        if (mainConfig == null) {
            return false;
        }

        ArrayList<PatchInfo> updatedPatches = mainConfig.patches;
        if (!removeObsoletedPatches(updatedPatches)
                || !installPatches(updatedPatches)
                || !installMainConfig(updatedPatches)) {
            return false;
        }

        return true;
    }

    private static boolean doUpdate() {

        if (!updateMainConfig()) return false;

        ArrayList<PatchInfo> updatedPatches = getUpdatedPatchList();
        if (updatedPatches == null) return false;

        int updatedPatchCount = 0;
        for (PatchInfo patchInfo : updatedPatches) {

            String status = patchInfo.mUpdateStatus;
            String patchName = patchInfo.mPatchName;

            if (status.equals(PatchInfo.STATUS_REMOVED)) {
                Log.d(LOG_TAG, "do remove " + patchName + "," + status);
                updatedPatchCount++;
                continue;
            }
            
            if (!status.equals(PatchInfo.STATUS_UPDATED)
                    && !status.equals(PatchInfo.STATUS_DEBUG)) {
                Log.d(LOG_TAG, "Don't do update " + patchName + "," + status);
                continue;
            }

            BasicPatchInfo updatePatchInfo = patchInfo.mCurrentPatchInfo;
            if (updatePatchInfo == null) {
                Log.d(LOG_TAG, "Unexpected update info " + patchName);
                return false;
            }

            if (!downloadPatch(updatePatchInfo)
                    || !extractPatchFiles(updatePatchInfo)) {
                if (status.equals(PatchInfo.STATUS_DEBUG)) { // for debug version, allow fail
                    patchInfo.mCurrentPatchInfo = patchInfo.mObsoletedPatchInfo;
                    patchInfo.mObsoletedPatchInfo = null;
                    patchInfo.mUpdateStatus = PatchInfo.STATUS_NO_UPDATE;
                    continue;
                }
                return false;
            }

            Patch patch = Patch.CreatePatch(mPatchDownloadFolder, updatePatchInfo);
            if (patch == null) return false;

            if (!InitPatch(patch)) {
                // It's OK, disable but don't remove
                // Or we'll think it's not the latest version next time, and will keep download it...
                updatePatchInfo.mStatus = BasicPatchInfo.STATUS_DISABLED;
                continue;
            }

            if (!generateInstallFiles(patch)) return false;

            updatedPatchCount++;
        }

        if (updatedPatchCount > 0) {
            String installMainConfigFile = Common.makeDownloadName(MAIN_CONFIG_FILE_NAME);
            if (!MainConfig.GenerateConfigFile(updatedPatches, installMainConfigFile)) {
                return false;
            }
            setUpdateStatus(STATUS_INSTALLING);
            Log.d(LOG_TAG, "Do update success --> Installing");
        } else {
            Log.d(LOG_TAG, "Do update success --> No update");
            setUpdateStatus(STATUS_IDLE);
        }

        return true;

    }

    private static boolean generateInstallFiles(Patch patch) {
        return patch.generateInstallFiles();
    }

    private static boolean InitPatch(Patch patch) {
        return patch.initPatch();
    }

    private static boolean removeObsoletedPatches(ArrayList<PatchInfo> updatedPatches) {
        Log.d(LOG_TAG, "Removing obsoleted patches");

        for (PatchInfo patch : updatedPatches) {
            String status = patch.mUpdateStatus;
            String patchName = patch.mPatchName;
            BasicPatchInfo obsoletedPatchInfo = patch.mObsoletedPatchInfo;

            if ((!status.equals(PatchInfo.STATUS_DEBUG)
                    && !status.equals(PatchInfo.STATUS_REMOVED)
                    && !status.equals(PatchInfo.STATUS_UPDATED))
                 || obsoletedPatchInfo == null) {
                continue;
            }

            String patchType = obsoletedPatchInfo.mType;
            ArrayList<String> fileList = Patch.getFileList(patchName, patchType);
            if (fileList == null || fileList.size() == 0) {
                Log.d(LOG_TAG, "Empty file list in " + patchName);
                return false;
            }
            for (String fileName : fileList) {
                String fullFileName = Common.makeInstallName(fileName);
                File file = new File(fullFileName);
                if (!file.delete()) {
                    Log.d(LOG_TAG, "Delete file failed " + fullFileName);
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean installPatches(ArrayList<PatchInfo> updatedPatches) {
        Log.d(LOG_TAG, "Installing updated patches");

        for (PatchInfo patch : updatedPatches) {
            String status = patch.mUpdateStatus;
            String patchName = patch.mPatchName;
            BasicPatchInfo currentPatchInfo = patch.mCurrentPatchInfo;

            if ((!status.equals(PatchInfo.STATUS_UPDATED)
                    && !status.equals(PatchInfo.STATUS_DEBUG))
                 || currentPatchInfo == null) {
                continue;
            }

            Log.d(LOG_TAG, "Install " + patch.mPatchName);

            String patchType = currentPatchInfo.mType;
            ArrayList<String> fileList = Patch.getFileList(patchName, patchType);
            if (fileList == null || fileList.size() == 0) {
                Log.d(LOG_TAG, "Empty file list in " + patchName);
                return false;
            }
            for (String fileName : fileList) {
                String src = Common.makeDownloadName(fileName);
                String dest = Common.makeInstallName(fileName);
                if (!Utils.copyFile(dest, src)) {
                    Log.d(LOG_TAG, "Install patch failed : " + patchName + ", " + dest + " <-- " + src);
                    return false;
                }
            }
        }

        return true;
    }

    private static boolean installMainConfig(ArrayList<PatchInfo> updatedPatches) {
        Log.d(LOG_TAG, "Installing main config file");

        for (PatchInfo patch : updatedPatches) {
            if (!patch.mUpdateStatus.equals(PatchInfo.STATUS_NO_UPDATE)) {
                patch.mObsoletedPatchInfo = null;
                patch.mUpdateStatus = PatchInfo.STATUS_NO_UPDATE;
            }
        }

        String installMainConfigFile = Common.makeDownloadName(MAIN_CONFIG_FILE_NAME);
        if (!MainConfig.GenerateConfigFile(updatedPatches, installMainConfigFile)) {
            return false;
        }

        String dest = Common.makeInstallName(MAIN_CONFIG_FILE_NAME);
        if (!Utils.copyFile(dest, installMainConfigFile)) {
            Log.d(LOG_TAG, "Install main config failed");
            return false;
        }

        return true;
    }

    private static boolean extractPatchFiles(BasicPatchInfo patch) {

        String patchName = patch.mPatchName;
        String patchType = patch.mType;

        ArrayList<String> fileList = Patch.getFileList(patchName, patchType);
        if (fileList == null || fileList.size() == 0) {
            Log.d(LOG_TAG, "Empty file list in " + patchName);
            return false;
        }
        for (int i = 0; i < fileList.size(); i++) {
            String fileName = fileList.get(i);
            String fullFileName = Common.makeDownloadName(fileName);
            String apkFileName = Common.makeDownloadName(patchName + ".apk");

            if (!Utils.extractFile(apkFileName, fileName, fullFileName)) return false;
        }

        return true;
    }

    private static boolean downloadPatch(BasicPatchInfo patch) {

        String url = patch.mUrl;
        String apk = Common.makeDownloadName(patch.mPatchName + ".apk");

        if (!Utils.download(url, apk)) {
            return false;
        }
        // To avoid MIM attack using files not signed by us
        if (!Utils.checkFileSignature(apk, mSignature)) return false;

        return true;
    }

    private static boolean updateMainConfig() {
        String mainConfigApk = Common.makeDownloadName(MAIN_CONFIG_APK_NAME);
        String mainConfigFile = Common.makeDownloadName(MAIN_CONFIG_FILE_NAME);
        String url = MAIN_CONFIG_URL;

        if (ENABLE_OFFLINE_TEST) {
            url = MAIN_CONFIG_URL_OFFLINE;
        }
        if (!Utils.download(url, mainConfigApk)) return false;
        if (!Utils.verifyAndExtractFile(mainConfigApk, MAIN_CONFIG_FILE_NAME, mainConfigFile, mSignature)) return false;

        return true;
    }

    private static ArrayList<PatchInfo> getUpdatedPatchList() {
        String newMainConfigFile = Common.makeDownloadName(MAIN_CONFIG_FILE_NAME);
        String oldMainConfigFile = Common.makeInstallName(MAIN_CONFIG_FILE_NAME);

        MainConfig oldMainConfig = MainConfig.parseFromFile(oldMainConfigFile);
        MainConfig newMainConfig = MainConfig.parseFromFile(newMainConfigFile);

        if (newMainConfig == null)  {
            Log.d(LOG_TAG, "Unable to get new config file");
            return null;
        }
        if (oldMainConfig == null) {
            Log.d(LOG_TAG, "No old patches, update all patches");

            for (int i = 0; i < newMainConfig.patches.size(); i++) {
                PatchInfo newPatchInfo = newMainConfig.patches.get(i);
                if (newPatchInfo.mReleasePatchInfo != null) {
                	newPatchInfo.mUpdateStatus = PatchInfo.STATUS_UPDATED;
                    newPatchInfo.mCurrentPatchInfo = newPatchInfo.mReleasePatchInfo;
                } else {
                	newPatchInfo.mUpdateStatus = PatchInfo.STATUS_DEBUG;
                    newPatchInfo.mCurrentPatchInfo = newPatchInfo.mDebugPatchInfo;
                }
            }
            return  newMainConfig.patches;
        }

        // Check for updated patches
        ArrayList<PatchInfo> newPatches = newMainConfig.patches;
        ArrayList<PatchInfo> oldPatches = oldMainConfig.patches;
        ArrayList<PatchInfo> updatedPatches = new ArrayList<PatchInfo>();
        for (int i = 0; i < newPatches.size(); i++) {
            PatchInfo newPatchInfo = newPatches.get(i);
            String patchName = newPatchInfo.mPatchName;

            BasicPatchInfo debugBasicPatchInfo = newPatchInfo.mDebugPatchInfo;
            BasicPatchInfo releaseBasicPatchInfo = newPatchInfo.mReleasePatchInfo;

            if (debugBasicPatchInfo == null) {
                newPatchInfo.mUpdateStatus = PatchInfo.STATUS_UPDATED;
                debugBasicPatchInfo = releaseBasicPatchInfo;
            }
            if (releaseBasicPatchInfo == null) {
            	newPatchInfo.mUpdateStatus = PatchInfo.STATUS_DEBUG;
                releaseBasicPatchInfo = debugBasicPatchInfo;
            }
            if (releaseBasicPatchInfo == null) {
                Log.d(LOG_TAG, "Both release and basic version is null!");
                return null;
            }
            
            PatchInfo oldPatchInfo = oldMainConfig.findPatch(patchName);
            if (oldPatchInfo == null) {
                newPatchInfo.mCurrentPatchInfo = releaseBasicPatchInfo;
                updatedPatches.add(newPatchInfo);
                Log.d(LOG_TAG, "Patch not exists, update to releaseVersion " + patchName);
                continue;
            }

            BasicPatchInfo oldBasicPatchInfo = oldPatchInfo.mCurrentPatchInfo;

            String oldVersion = oldBasicPatchInfo.mVersion;
            String debugVersion = debugBasicPatchInfo.mVersion;
            String releaseVersion = releaseBasicPatchInfo.mVersion;

            if (TextUtils.isEmpty(oldVersion)) {
                newPatchInfo.mUpdateStatus = PatchInfo.STATUS_UPDATED;
                newPatchInfo.mCurrentPatchInfo = newPatchInfo.mReleasePatchInfo;
                updatedPatches.add(newPatchInfo);
                Log.d(LOG_TAG, "Patch not exists, update to release version " + patchName + "," + releaseVersion);
                continue;
            }

            if(oldVersion.equals(debugVersion)) {
                newPatchInfo.mUpdateStatus = PatchInfo.STATUS_NO_UPDATE;
                newPatchInfo.mCurrentPatchInfo = oldPatchInfo.mCurrentPatchInfo;
                updatedPatches.add(newPatchInfo);
                Log.d(LOG_TAG, "Patch with latest version, keep it " + patchName + "," + debugVersion);
            } else if (oldVersion.equals(releaseVersion)) {
                newPatchInfo.mUpdateStatus = PatchInfo.STATUS_DEBUG;
                newPatchInfo.mCurrentPatchInfo = newPatchInfo.mDebugPatchInfo;
                newPatchInfo.mObsoletedPatchInfo = oldBasicPatchInfo;
                updatedPatches.add(newPatchInfo);
                Log.d(LOG_TAG, "Patch with release version, update to Debug Version "
                             + patchName + "," + releaseVersion + "," + debugVersion);
            } else {
                newPatchInfo.mUpdateStatus = PatchInfo.STATUS_UPDATED;
                newPatchInfo.mCurrentPatchInfo = newPatchInfo.mReleasePatchInfo;
                newPatchInfo.mObsoletedPatchInfo = oldBasicPatchInfo;
                updatedPatches.add(newPatchInfo);
                Log.d(LOG_TAG, "Patch with obsoleted version, update to Release Version "
                            + patchName + "," + oldVersion + "," +releaseVersion);
            }
        }

        // Check for removed patches
        for (int i = 0; i < oldPatches.size(); i++) {
            PatchInfo oldPatchInfo = oldPatches.get(i);
            String patchName = oldPatchInfo.mPatchName;

            // TODO 从新的MainConfig里找旧的补丁是否存在，若不存在则标记�? STATUS_REMOVED
            PatchInfo newPatchInfo = newMainConfig.findPatch(patchName);
            if (newPatchInfo == null) {
                oldPatchInfo.mUpdateStatus = PatchInfo.STATUS_REMOVED;
                oldPatchInfo.mObsoletedPatchInfo = oldPatchInfo.mCurrentPatchInfo;
                updatedPatches.add(oldPatchInfo);
                Log.d(LOG_TAG, "Patch has been removed " + patchName);
            }
        }

        return updatedPatches;
    }

    public static boolean ShouldInstallUpdates() {
        // Only install updates in main process
        // Other process should wait until install done
        if (!Utils.getCurrentProcessName(mContext).equals(Utils.getPackageName(mContext))) {
            return false;
        }

        String updateStatus = Common.GetUpdateStatus();
        if (updateStatus.equals(STATUS_INSTALLING)) {
            return true;
        }

        return false;
    }

    public static boolean ShouldWaitUpdateFinish() {
        // If process isn't main process
        // And there are new updates waiting for install
        // Then child process should wait until update finish
        if (Utils.getCurrentProcessName(mContext).equals(Utils.getPackageName(mContext))) {
            return false;
        }

        String updateStatus = Common.GetUpdateStatus();
        if (updateStatus.equals(STATUS_INSTALLING)) {
            return true;
        }

        return false;
    }

    public static boolean ShouldDoUpdate() {

        // Only do update in main process
        // Assume that the main process has the same name with package name
        // TODO : Maybe it's not true...
        if (!Utils.getCurrentProcessName(mContext).equals(Utils.getPackageName(mContext))) {
            Log.d(LOG_TAG, "Don't do update in sub-process");
            return false;
        }

        String updateStatus = Common.GetUpdateStatus();
        String lastUpdateTimeStr =  Common.GetLastUpdateTime();

        if (lastUpdateTimeStr.equals("")) {
            Log.d(LOG_TAG, "Never do update yet");
            return true;
        }
        Date lastUpdateTime;
        try {
            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            lastUpdateTime = df.parse(lastUpdateTimeStr);
        } catch (ParseException e) {
            return true;
        }

        Date now = new Date();
        long diff = (now.getTime() > lastUpdateTime.getTime()) ?
                (now.getTime() - lastUpdateTime.getTime()) :
                (lastUpdateTime.getTime() - now.getTime());

        if (updateStatus.equals(STATUS_UPDATING)) {
            // Reset update status if last updating cost too much time
            // e.g., there is an exception, or the process is killed before set status to IDLE
            if (diff > MAX_FORCE_REUPDATE_TIME) {
                Log.d(LOG_TAG, "Force re-update :" + diff);
                return true;
            }
        } else if (updateStatus.equals(STATUS_UPDATE_FAILED)) {
            Log.d(LOG_TAG, "Force do update");
            return true;
        } else if (!(updateStatus.equals(STATUS_IDLE)
                        || updateStatus.equals(STATUS_INSTALLING))) {
            Log.d(LOG_TAG, "Illegal status : " + updateStatus + ", force update!");
            return true;
        }

        if (diff > MAX_UPDATE_INTERVAL) {
            Log.d(LOG_TAG, "Need update. "
                    + "Last update time : " + lastUpdateTimeStr
                    + ", current time : " + now.toString());
            return true;
        } else {
            Log.d(LOG_TAG, "Don't need update. "
                    + "Last update time : " + lastUpdateTimeStr
                    + ", current time : " + now.toString());
            return false;
        }

    }

    private static void setUpdateStatus(String status) {
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Common.SetLastUpdateTime(df.format(new Date()));
        Common.SetUpdateStatus(status);
    }


}
