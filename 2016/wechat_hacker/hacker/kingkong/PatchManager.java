package com.tencent.kingkong;

import android.content.Context;

import com.tencent.kingkong.Common.Log;
import com.tencent.kingkong.MainConfig.PatchInfo;
import com.tencent.kingkong.MainConfig.BasicPatchInfo;

import java.io.File;
import java.util.ArrayList;

public class PatchManager {

	private static final String LOG_TAG = "KingKongPatchManager";

	private static final String PATCH_LIST_FILE = ".cfg";
	
	private static Context mContext = null;
	private static String mPatchDownloadFolder = "";
	private static String mPatchFolder = "";

	private static ArrayList<Patch> mPatches = new ArrayList<Patch>();
    private static MainConfig mMainConfig = null;

    public static native byte[] nativeGetFingerprint(String libraryName, String functionName, int fingerprintType);
    public static native boolean nativeHoldLibStrongRef(String libraryName);
    public static native int nativeCalcParameter(int paramType, String value1, String value2, int value3);
    public static native int nativeCalcJumperPoint(String libraryName);
    public static native boolean nativeDoPatch(String patchFilePatch, String libraryName, String functionName, 
    									int hookPoint, int jumperPoint, int params[], int paramCount);
    public static native boolean nativeVerifyPatchParams(String patchFilePatch,int hookPoint,
    									int jumperPoint, int params[], int paramCount);

	public static synchronized int InitPatchManager(Context context) {

		if (context == null) return 1;

		mContext = context.getApplicationContext();
		mPatchDownloadFolder = Common.GetDownloadFolder();
		mPatchFolder = Common.GetPatchFolder();

		Common.savePointLog(++Common.mLogPoint);
		if (!initPatchFiles()) {
			return 2;
		}

		Common.savePointLog(++Common.mLogPoint);
		if (!loadSecureLibrary()) {
			return 3;
		}

		Common.SetInitializeStatus(Common.STATUS_INITIALIZED_PATCH_MANAGER);
		return 0;
	}

	public static boolean initPatchFiles() {
		
		try {
			File file = new File(mPatchDownloadFolder);
			if (!file.exists()) {
				if (file.mkdirs()) {
					Log.d(LOG_TAG, "Create patch download folder : " + mPatchDownloadFolder + " OK");
				} else {
					Log.d(LOG_TAG, "Create patch download folder : " + mPatchDownloadFolder + " failed");
					return false;
				}
			}
			
			file = new File(mPatchFolder);
			if (!file.exists()) {
				if (file.mkdirs()) {
					Log.d(LOG_TAG, "Create patch folder : " + mPatchFolder + " OK");
				} else {
					Log.d(LOG_TAG, "Create patch folder : " + mPatchFolder + " failed");
					return false;
				}
			}

			return true;
		} catch (Exception e0) {
			e0.printStackTrace();
			return false;
		}
	}

    private static boolean loadSecureLibrary() {
		// TODO: How to load the framework library?
		//String library = mPatchFolder + mPathSeparator + PATCH_DRIVER_LIBRARY_FILE;
		String library = "kkfixerdriver";

    	try {

    		//System.load(library);
			System.loadLibrary(library);
    		Log.d(LOG_TAG, "Load library " + library + " OK");
    	} catch (Exception e) {
    		Log.d(LOG_TAG, "Load library " + library + " failed : " + e);
    		return false;
    	} catch (Throwable e) {
    		Log.d(LOG_TAG, "Load library " + library + " failed : " + e);
    		return false;
    	}
    	return true;
    }

    private static boolean createPatches() {

    	for (PatchInfo patchInfo : mMainConfig.patches) {
            BasicPatchInfo basicPatchInfo = patchInfo.mCurrentPatchInfo;
            String patchName = patchInfo.mPatchName;
            if (basicPatchInfo == null) {
                Log.d(LOG_TAG, "Why there is no installed patch ?! " + patchName);
                continue;
            }

    		Patch patch = Patch.CreatePatch(mPatchFolder, basicPatchInfo);
    		if (patch == null
                    || !patch.initPatch()) {
                // TODO: how to disable it?
    		    // patchInfo.disable();
    		} else {
                Log.d(LOG_TAG, "Patch is ready -->" + patch.mPatchName);
                mPatches.add(patch);
            }
    	}

        return true;
    }

    public static void DoPatches() {
        String fileName = Common.makeInstallName(DataReport.getPackageName() + PATCH_LIST_FILE);
        mMainConfig = MainConfig.parseFromFile(fileName);
        if (mMainConfig == null) return;

        if (!createPatches()) return;

    	for (Patch patch : mPatches) {
    	    patch.doPatch(mContext);
    	}
    }

}
