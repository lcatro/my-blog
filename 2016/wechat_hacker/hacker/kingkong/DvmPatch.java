package com.tencent.kingkong;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.text.TextUtils;

import com.tencent.kingkong.Common.Log;
import com.tencent.kingkong.xposed.XposedBridge;

public class DvmPatch extends Patch{
    private static final String LOG_TAG = "KingKongDvmPatch";

    private static final String TAG_PATCH_NAME = "name";
    private static final String TAG_VERSION = "ver";
    private static final String TAG_TYPE= "type";
    private static final String TAG_CLASS_NAME = "class_name";
    private static final String TAG_BASIC_CONFIG = "basic_config";

    private static String mClassName = "";

    @Override
    public boolean doPatch(Context context) {
        if (context == null || TextUtils.isEmpty(mPath) || TextUtils.isEmpty(mPatchName) || TextUtils.isEmpty(mClassName)) {
            Log.d(LOG_TAG, "DvmPatch doPath value is empty");
            return false;
        }
        return XposedBridge.doPatch(context, mPath, mPatchName, mClassName);
    }

    public static Patch CreatePatch(String path, MainConfig.BasicPatchInfo basicPatchInfo) {

        String patchName = basicPatchInfo.mPatchName;

        DvmPatch patch = new DvmPatch(path, patchName);
        String configFileName = patch.getConfigFileName();

        patch.mBasicPatchInfo = basicPatchInfo;
        if (patch.parseConfig(configFileName)) {
            return patch;
        }
        return null;
    }

    private boolean parseConfig(String fileName) {

        String jsonStr = Utils.ReadFileToString(fileName);
        JSONObject jsonObj;
        try {
            jsonObj = new JSONObject(jsonStr);

            mPatchName = jsonObj.getString(TAG_PATCH_NAME);
            mPatchVersion = jsonObj.getString(TAG_VERSION);
            mPatchType = jsonObj.getString(TAG_TYPE);

            mClassName = jsonObj.getJSONObject(TAG_BASIC_CONFIG).getString(TAG_CLASS_NAME).trim();
            Common.Log.d(LOG_TAG, "--> Hookpoint Class Name : " + mClassName);
        } catch (JSONException e) {
            Common.Log.d(LOG_TAG, "Parse dvm config failed : " + e);
            return false;
        }
        return true;
    }

    @Override
    public boolean generateInstallFiles() {
        return true;
    }

    @Override
    public boolean initPatch() {
        return XposedBridge.initPatch();
    }

    public DvmPatch(String path, String patchName) {
        mPath = path;
        mPatchName = patchName;
    }
}
