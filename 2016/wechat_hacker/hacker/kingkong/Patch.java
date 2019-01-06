package com.tencent.kingkong;

import android.content.Context;
import android.os.Build;

import com.tencent.kingkong.Common.Log;
import com.tencent.kingkong.MainConfig.BasicPatchInfo;

import java.io.File;
import java.util.ArrayList;

abstract public class Patch {
	
	private static final String LOG_TAG = "KingKongPatchInfo";

	public static final String PATCH_TYPE_NATIVE = "native";
	public static final String PATCH_TYPE_JAVA = "java";
	public static final String PATCH_TYPE_DVM = "DVM";
	public static final String PATCH_TYPE_ART = "art";
	public static final String PATCH_TYPE_ARBITRARY = "arbitrary";

    private static String mPathSeparator = File.separator;

    public String mPath = "";
    public String mPatchName = "";
    public String mPatchVersion = "";
    public String mPatchType = "";
    public BasicPatchInfo mBasicPatchInfo = null;

    public static ArrayList<String> getFileList(String patchName, String patchType) {
        ArrayList<String> fileList = new ArrayList<String>();
        if (patchType.equals(PATCH_TYPE_NATIVE)) {
            fileList.add(patchName + ".cfg");
            fileList.add("lib" + patchName + ".so");
            fileList.add(patchName + ".subpatch");
            return fileList;
        } else if (patchType.equals(PATCH_TYPE_DVM)) {
            fileList.add(patchName + ".cfg");
            fileList.add(patchName + ".jar");
            return fileList;
        }
        return null;
    }

    public ArrayList<String> getFileList() {
        return getFileList(mPatchName, mPatchType);
    }

    abstract public boolean doPatch(Context context);
    abstract public boolean generateInstallFiles();
    abstract public boolean initPatch();

    /*
	public boolean isAllowDelayLoad() {
		return mBasicPatchInfo.mAllowDelayLoad.equals("true");
	}

	public boolean isCritical() {
		return critical.equals("true");
	}
    */

	public boolean isSupportDriver() {
        String[] drivers = mBasicPatchInfo.mDriverVersion.split(",");
        String curDriver =Common.PATCH_DRIVER_VERSION;
        for (String driver : drivers) {
            if (curDriver.equals(driver.trim())) {
                return true;
            }
        }
        return false;
	}

	public boolean isSupportSystem() {
		String[] systems = mBasicPatchInfo.mSupportSystem.split(",");
		String curSystem = String.valueOf(Build.VERSION.SDK_INT);
		for (String system : systems) {
			if (curSystem.equals(system.trim())) {
				return true;
			}
		}
		return false;
	}

	public String getConfigFileName() {
		return mPath + mPathSeparator + mPatchName + ".cfg";
	}

    public static Patch CreatePatch(String path, BasicPatchInfo basicPatchInfo) {

        String type = basicPatchInfo.mType;
        Patch patch = null;
        if (type.equals(PATCH_TYPE_NATIVE)) {
            patch = NativePatch.CreatePatch(path, basicPatchInfo);
        } else if (type.equals(PATCH_TYPE_DVM)) {
            patch = DvmPatch.CreatePatch(path, basicPatchInfo);
        } else {
            Log.d(LOG_TAG, "Unsupported patch type : " + type);
            return null;
        }

        if (patch == null) {
            return null;
        }

        // Do simple verification
        if (!patch.mPatchName.equals(basicPatchInfo.mPatchName)
                || !patch.mPatchVersion.equals(basicPatchInfo.mVersion)
                || !patch.mPatchType.equals(basicPatchInfo.mType)) {
            Log.d(LOG_TAG, "Patch name, version or type mismatch : "
                    + patch.mPatchName + ", " + patch.mPatchVersion + ", " + patch.mPatchType);
            return null;
        }

        return patch;
    }
}
