package com.tencent.kingkong;

import android.content.Context;

import com.tencent.kingkong.Common.Log;
import com.tencent.kingkong.MainConfig.BasicPatchInfo;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.util.ArrayList;

public class NativePatch extends Patch {

	private static final String LOG_TAG = "KingKongNativePatch";

    private static final String TAG_PATCH_NAME = "name";
    private static final String TAG_VERSION = "ver";
    private static final String TAG_TYPE= "type";
	private final static String TAG_FINGERPRINT_COUNT = "fingerprint_count";
	private final static String TAG_FINGERPRINT_DEFINITION = "fingerprint_definition";
	private final static String TAG_LIBRARY_NAME = "lib_name";
	private final static String TAG_FUNCTION_NAME = "func_name";
	private final static String TAG_HOOKPOINT_LIBRARY_NAME = "hookpoint_lib_name";
	private final static String TAG_HOOKPOINT_FUNCTION_NAME = "hookpoint_func_name";
	private final static String TAG_PARAMETER_DEFINE = "parameter_definition";
	private final static String TAG_PARAMETER_COUNT = "param_count";
	private final static String TAG_PARAM_INDEX = "param_index";
	private final static String TAG_PARAM_TYPE = "param_type";
	private final static String TAG_PARAM_VALUE1 = "param_value1";
	private final static String TAG_PARAM_VALUE2 = "param_value2";
    private final static String TAG_BASIC_GROUP = "basic_group";

	private ArrayList<NativeSubPatch> mNativeSubPatches = new ArrayList<NativeSubPatch>();
    private NativeSubPatch mValidSubPatch = null;

    // Calculate Fingerprint via libraryNames and functionNames
    private int mFingerprintCount = 0;
    private ArrayList<String> mLibraryNames = new ArrayList<String>();
    private ArrayList<String> mFunctionNames = new ArrayList<String>();

    private String mHookpointLibName = "";
    private String mHookpointFuncName = "";

    private int mParameterCount =0;
    private ArrayList<ParameterDef> mParameterDefines = new ArrayList<ParameterDef>();

    private static String mPathSeparator = File.separator;

    @Override
    public boolean doPatch(Context context) {

        int[] realParams = new int[mParameterCount];
        if (!getPatchParameters(mParameterDefines, mValidSubPatch.parameters, realParams)) {
            Log.d(LOG_TAG, "Do patch get parameters failed : " + mPatchName);
            return false;
        }

        int hookPoint = PatchManager.nativeCalcParameter(
                Common.PARAM_TYPE_OFFSET_TO_FUNCTION, mHookpointLibName,
                mHookpointFuncName, mValidSubPatch.hookPoint);
        if (hookPoint == -1) {
            Log.d(LOG_TAG, "Do patch calc hookPoint failed");
            return false;
        }

        // TODO: It's a const, calculate it when install
        int jumperPoint = PatchManager.nativeCalcJumperPoint(mHookpointLibName);
        if (jumperPoint == -1) {
            Log.d(LOG_TAG, "Do patch calc jumperPoint failed");
            return false;
        }

        String fullFileName = Common.makeInstallName("lib" + mPatchName + ".so");
        if (PatchManager.nativeVerifyPatchParams(fullFileName, hookPoint, jumperPoint, realParams, mParameterCount)) {
            Log.d(LOG_TAG, "Verify parameters OK : " + mPatchName);
        } else {
            Log.d(LOG_TAG, "Verify parameters failed : " + mPatchName);
            return false;
        }

        if (PatchManager.nativeDoPatch(fullFileName, mHookpointLibName, mHookpointFuncName,
                hookPoint, jumperPoint, realParams, mParameterCount)) {
            Log.d(LOG_TAG, "Do patch OK : " + mPatchName);
            return true;
        } else {
            Log.d(LOG_TAG, "Do patch failed : " + mPatchName);
            return false;
        }
    }

    public static String getFingerprint(String libraryName, String functionName, int fingerprintType) {

        byte[] fingerprint = PatchManager.nativeGetFingerprint(libraryName, functionName, fingerprintType);

        if (fingerprint != null) {
            if (fingerprint.length != 0) {
                return Utils.getSHA1String(fingerprint);
            }
        }
        return "";
    }

    public static boolean getPatchParameters(ArrayList<ParameterDef> paramDefines,
                                             ArrayList<Integer> param, int realParams[]) {
        for (int i = 0; i < paramDefines.size(); i++) {
            ParameterDef paramDef = paramDefines.get(i);
            int value = param.get(i);
            realParams[i] = PatchManager.nativeCalcParameter(paramDef.type, paramDef.value1, paramDef.value2, value);
            if (realParams[i] == -1) {
                return false;
            }
        }

        return true;
    }

    public static Patch CreatePatch(String path, BasicPatchInfo basicPatchInfo) {

        String patchName = basicPatchInfo.mPatchName;

        NativePatch patch = new NativePatch(path, patchName);
        String configFileName = patch.getConfigFileName();

        patch.mBasicPatchInfo = basicPatchInfo;
        if (patch.parseConfig(configFileName)
                && patch.parseSubPatches()) {
            return patch;
        }
        return null;
    }

	private boolean parseConfig(String fileName) {

        // TODO: Check whether the fields are correct!

		String jsonStr = Utils.ReadFileToString(fileName);
		JSONObject jsonObj;
		try {
			jsonObj = new JSONObject(jsonStr);

            mPatchName = jsonObj.getString(TAG_PATCH_NAME);
            mPatchVersion = jsonObj.getString(TAG_VERSION);
            mPatchType = jsonObj.getString(TAG_TYPE);

            JSONObject jsonBasicGroup = jsonObj.getJSONObject(TAG_BASIC_GROUP);
			mFingerprintCount = jsonBasicGroup.getInt(TAG_FINGERPRINT_COUNT);
            mParameterCount = jsonBasicGroup.getInt(TAG_PARAMETER_COUNT);
            mHookpointFuncName = jsonBasicGroup.getString(TAG_HOOKPOINT_FUNCTION_NAME).trim();
            mHookpointLibName = jsonBasicGroup.getString(TAG_HOOKPOINT_LIBRARY_NAME).trim();

            Log.d(LOG_TAG, "--> Hookpoint library Name : " + mHookpointLibName);
            Log.d(LOG_TAG, "--> Hookpoint function Name : " + mHookpointFuncName);
            Log.d(LOG_TAG, "--> Fingerprint count : " + mFingerprintCount);
            Log.d(LOG_TAG, "--> Parameter count : " + mParameterCount);

			for (int i = 0; i < mFingerprintCount; i++) {
                String tag = TAG_FINGERPRINT_DEFINITION + "_" + String.valueOf(i + 1);
                JSONObject jsonDef = jsonObj.getJSONObject(tag);
				String libraryName = jsonDef.getString(TAG_LIBRARY_NAME).trim();
				String functionName = jsonDef.getString(TAG_FUNCTION_NAME).trim();
                mLibraryNames.add(libraryName);
                mFunctionNames.add(functionName);
				Log.d(LOG_TAG, "--> Fingerprint Library Name : " + libraryName + " ; Function Name : " + functionName);
			}

			for (int i = 0; i < mParameterCount; i++) {
                String tag = TAG_PARAMETER_DEFINE + "_" + String.valueOf(i + 1);
                JSONObject jsonParam = jsonObj.getJSONObject(tag);
				ParameterDef param = new ParameterDef();
				if (param.parse(jsonParam)) {
					if (param.index != i) {
						// Parameter index is very very important!
						Log.d(LOG_TAG, "Parameter index error!");
						return false;
					}
					Log.d(LOG_TAG, "--> Parameter definition : " + param);
                    mParameterDefines.add(param);
				} else {
					return false;
				}
			}

		} catch (JSONException e) {
			Log.d(LOG_TAG, "Parse sub patches failed : " + e);
			return false;
		}

		return true;
	}

    private String getSubPatchFileName() {
        return mPath + mPathSeparator + mPatchName + ".subpatch";
    }

    private boolean parseSubPatches() {
        String fileName = getSubPatchFileName();
        mNativeSubPatches = NativeSubPatch.parseSubPatchesFromFile(fileName);
        return (mNativeSubPatches != null);
    }

    @Override
    public boolean initPatch() {
        ArrayList<String> curFingerprints = new ArrayList<String>();
        int curFingerprintCount = mFingerprintCount;

        if (curFingerprintCount <= 0) {
            Log.d(LOG_TAG, "Unexpected fingerprint count");
            return false;
        }

        for (int i = 0; i < curFingerprintCount; i++) {
            String libraryName = mLibraryNames.get(i);
            String functionName = mFunctionNames.get(i);
            String fingerprint = getFingerprint(libraryName, functionName, 0);
            if (fingerprint.equals("")) {
                Log.d(LOG_TAG, "Calculate fingerprint for " + libraryName + " , " + functionName + " failed");
                return false;
            } else {
                Log.d(LOG_TAG, "Current fingerprint : " + fingerprint);
                curFingerprints.add(fingerprint);
            }
        }

        for (int i = 0; i < mNativeSubPatches.size(); i++) {
            NativeSubPatch nativeSubPatch = mNativeSubPatches.get(i);
            if (nativeSubPatch.fingerprints.size() != curFingerprintCount) {
                Log.d(LOG_TAG, "Skip fingerprint mismatch patch " + i);
                continue;
            }
            boolean found = true;
            for (int j = 0; j < curFingerprintCount; j++) {
                if (!nativeSubPatch.fingerprints.get(j).equals(curFingerprints.get(j))) {
                    found = false;
                    break;
                }
            }
            if (found) {
                Log.d(LOG_TAG, "Fingerprint matched!");
                mValidSubPatch = nativeSubPatch;
                return true;
            }
        }

        Log.d(LOG_TAG, "Unable to get valid SubPatch!");
        return false;
    }

    @Override
    public boolean generateInstallFiles() {

        String subPatchFile = getSubPatchFileName();

        ArrayList<NativeSubPatch> subPatches = new ArrayList<NativeSubPatch>();
        subPatches.add(mValidSubPatch);

        return NativeSubPatch.generateConfigFile(subPatchFile, subPatches);
    }

	public NativePatch(String path, String patchName) {
        mPath = path;
        mPatchName = patchName;
    }

	public class ParameterDef {
		public int index;
		public int type;
		public String value1;
		public String value2;

		public boolean parse(JSONObject jsonObject) {
			try {
				index = Integer.parseInt(jsonObject.getString(TAG_PARAM_INDEX));
				type = Integer.parseInt(jsonObject.getString(TAG_PARAM_TYPE));
				value1 = jsonObject.getString(TAG_PARAM_VALUE1).trim();
				value2 = jsonObject.getString(TAG_PARAM_VALUE2).trim();
			} catch (Exception e) {
				Log.d(LOG_TAG, "Parse parameter error!");
				return false;
			}
			return true;
		}

		@Override
		public String toString() {
			return "Index " + index + ": " + type + ", " + value1 + ", " + value2;
		}
	}
}
