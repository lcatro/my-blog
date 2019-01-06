package com.tencent.kingkong;

import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONObject;

import android.text.TextUtils;
import android.util.JsonWriter;

import com.tencent.kingkong.Common.Log;

public class MainConfig {

    private static final String LOG_TAG = "KingKongMainConfig";

    public ArrayList<PatchInfo> patches = new ArrayList<PatchInfo>();
    public String mAppName = "";

    private final static String TAG_APP_NAME = "app";
    private final static String TAG_PATCHES = "patches";

    public static MainConfig parseFromFile(String fileName) {
        Log.d(LOG_TAG, "Analysing Config File " + fileName);

        String appName = DataReport.getPackageName();
        if (appName == null) {
            Log.d(LOG_TAG, "Unable to get application name");
            return null;
        }

        MainConfig mainConfig = new MainConfig();
        try {
            String jsonStr = Utils.ReadFileToString(fileName);
            Log.d(LOG_TAG, "Main config : " + jsonStr);
            if (TextUtils.isEmpty(jsonStr)) {
                return null;
            }
            JSONObject jsonObj = new JSONObject(jsonStr);
            JSONArray jsonArray = jsonObj.getJSONArray(TAG_PATCHES);

            mainConfig.mAppName = jsonObj.getString(TAG_APP_NAME);
            if (!mainConfig.mAppName.equals(appName)) {
                Log.d(LOG_TAG, "Application mismatch : " + appName + ", " + mainConfig.mAppName);
                return null;
            }

            for (int i = 0; i < jsonArray.length(); i++) {
                JSONObject jsonPatch = jsonArray.getJSONObject(i);
                PatchInfo patch = PatchInfo.parseFromJSON(jsonPatch);
                if (patch == null) {
                    return null;
                }
                mainConfig.patches.add(patch);
            }
        } catch (Exception e) {
            Log.d(LOG_TAG, "Parse main config exception " + e);
            return null;
        }
        return mainConfig;
    }

    public static boolean GenerateConfigFile(ArrayList<PatchInfo> patches, String fileName) {

        Log.d(LOG_TAG, "Generate main config file " + fileName);
        try {
            FileOutputStream fos = new FileOutputStream(fileName);
            JsonWriter writer = new JsonWriter(new OutputStreamWriter(fos, "UTF-8"));
            writer.beginObject();
            writer.name(TAG_APP_NAME).value(DataReport.getPackageName());   // TODO:handle get package exception
            writer.name(TAG_PATCHES);
            writer.beginArray();
            for (int i = 0; i < patches.size(); i++) {
            	PatchInfo patchInfo = patches.get(i);
				if (patchInfo.mCurrentPatchInfo == null && patchInfo.mObsoletedPatchInfo == null
						&& patchInfo.mUpdateStatus == PatchInfo.STATUS_NO_UPDATE) {
                    continue;
            	}
                if (!patches.get(i).writePatchInfo(writer)) {
                    return false;
                }
            }
            writer.endArray();    // End of TAG_PATCHES
            writer.endObject();
            writer.close();
            fos.close();
        } catch (Exception e) {
            Log.d(LOG_TAG, "Generate main config file exception : " + e);
            return false;
        }

        Log.d(LOG_TAG, "Generate OK");
        return true;
    }

    public PatchInfo findPatch(String patchName) {
        for (int i = 0; i < patches.size(); i++) {
            PatchInfo patch = patches.get(i);
            if (patch.mPatchName.equals(patchName)) {
                return patch;
            }
        }
        return null;
    }

    public static class BasicPatchInfo {

        public String mVersion = "";
        public String mUrl = "";
        public String mDriverVersion = "";      // patch driver version
        public String mSupportSystem = "";	   // Android version list 18,19...
        public String mType = "";				   // Native, DVM, art

        public String mPatchName = "";
        public String mStatus = "enabled";

        private static final String TAG_URL = "url";
        private static final String TAG_DRIVER_VERSION = "driver";
        private static final String TAG_SYSTEM_VERSION = "system";
        private static final String TAG_TYPE = "type";
        private static final String TAG_VERSION = "ver";
        private static final String TAG_STATUS = "status";

        public static final String STATUS_ENABLED = "enabled";
        public static final String STATUS_DISABLED = "disabled";

        public static BasicPatchInfo parseFromJSON(JSONObject jsonObj, String patchName) {
            BasicPatchInfo basicPatchInfo = new BasicPatchInfo();
            basicPatchInfo.mPatchName = patchName;
            try {
                basicPatchInfo.mVersion = jsonObj.getString(TAG_VERSION).trim();
                basicPatchInfo.mUrl = jsonObj.getString(TAG_URL);
                basicPatchInfo.mDriverVersion = jsonObj.getString(TAG_DRIVER_VERSION);
                basicPatchInfo.mSupportSystem = jsonObj.getString(TAG_SYSTEM_VERSION);
                basicPatchInfo.mType = jsonObj.getString(TAG_TYPE);
                basicPatchInfo.mStatus = jsonObj.optString(TAG_STATUS, "enabled");

                basicPatchInfo.print();
                return basicPatchInfo;
            } catch (Exception e) {
                Log.d(LOG_TAG, "Parse basic patch info exception : " + e);
                return null;
            }
        }

        public boolean writePatchInfo(JsonWriter writer) throws Exception{
            writer.name(TAG_VERSION).value(mVersion);
            writer.name(TAG_DRIVER_VERSION).value(mDriverVersion);
            writer.name(TAG_SYSTEM_VERSION).value(mSupportSystem);
            writer.name(TAG_TYPE).value(mType);
            writer.name(TAG_URL).value("");
            writer.name(TAG_STATUS).value(mStatus);
            return true;
        }

        public void print() {
            Log.d(LOG_TAG, "I'm : " + mPatchName + ","
                    + " version : " + mVersion
                    + " url : " + mUrl
                    + " driver : " + mDriverVersion
                    + " system : " + mSupportSystem
                    + " type : " + mType);
        }
    }

    public static class PatchInfo {

        private static final String TAG_PATCH_NAME = "name";
        private static final String TAG_RELEASE_VERSION = "releasever";
        private static final String TAG_DEBUG_VERSION = "debugver";
        private static final String TAG_CURRENT_VERSION = "currentver";
        private static final String TAG_OBSOLETED_VERSION = "obsoletedver";
        private static final String TAG_UPDATE_STATUS = "update_status";

        public static final String STATUS_REMOVED = "removed";
        public static final String STATUS_UPDATED = "updated";
        public static final String STATUS_NO_UPDATE = "no_update";
        public static final String STATUS_DISABLED = "disabled";
        public static final String STATUS_DEBUG = "debug";

        public String mPatchName = "";
        public BasicPatchInfo mReleasePatchInfo = null;
        public BasicPatchInfo mDebugPatchInfo = null;
        public BasicPatchInfo mCurrentPatchInfo = null;
        public BasicPatchInfo mObsoletedPatchInfo = null;   // Been removed or updated
        public String mUpdateStatus = STATUS_NO_UPDATE;

        public boolean writePatchInfo(JsonWriter writer) throws Exception{
            writer.beginObject();
            writer.name(TAG_PATCH_NAME).value(mPatchName);
            writer.name(TAG_UPDATE_STATUS).value(mUpdateStatus);

            if (mUpdateStatus.equals(STATUS_NO_UPDATE)) {
                writer.name(TAG_CURRENT_VERSION);
                writer.beginObject();
                mCurrentPatchInfo.writePatchInfo(writer);
                writer.endObject();
            } else if (mUpdateStatus.equals(STATUS_DEBUG)
                    || mUpdateStatus.equals(STATUS_UPDATED)) {

                writer.name(TAG_CURRENT_VERSION);
                writer.beginObject();
                mCurrentPatchInfo.writePatchInfo(writer);
                writer.endObject();

                if (mObsoletedPatchInfo != null) {
                    writer.name(TAG_OBSOLETED_VERSION);
                    writer.beginObject();
                    mObsoletedPatchInfo.writePatchInfo(writer);
                    writer.endObject();
                }

            } else if (mUpdateStatus.equals(STATUS_REMOVED)) {
                writer.name(TAG_OBSOLETED_VERSION);
                writer.beginObject();
                mObsoletedPatchInfo.writePatchInfo(writer);
                writer.endObject();
            } else {
                Log.d(LOG_TAG, "Unexpected patch status " + mUpdateStatus);
                return false;
            }

            writer.endObject();
            return true;
        }

        public static PatchInfo parseFromJSON(JSONObject jsonPatch) {

            PatchInfo patchInfo = new PatchInfo();
            try {
                patchInfo.mPatchName = jsonPatch.getString(TAG_PATCH_NAME).trim();
                patchInfo.mUpdateStatus = jsonPatch.optString(TAG_UPDATE_STATUS, STATUS_NO_UPDATE).trim();

                JSONObject releaseVer = jsonPatch.optJSONObject(TAG_RELEASE_VERSION);
                if (releaseVer != null) {
                    patchInfo.mReleasePatchInfo = BasicPatchInfo.parseFromJSON(releaseVer, patchInfo.mPatchName);
                    if (patchInfo.mReleasePatchInfo == null) {
                        return null;
                    }
                }

                JSONObject debugVer = jsonPatch.optJSONObject(TAG_DEBUG_VERSION);
                if (debugVer != null) {
                    patchInfo.mDebugPatchInfo = BasicPatchInfo.parseFromJSON(debugVer, patchInfo.mPatchName);
                    if (patchInfo.mDebugPatchInfo == null) {
                        return null;
                    }
                }

                JSONObject currentVer = jsonPatch.optJSONObject(TAG_CURRENT_VERSION);
                if (currentVer != null) {
                    patchInfo.mCurrentPatchInfo = BasicPatchInfo.parseFromJSON(currentVer, patchInfo.mPatchName);
                    if (patchInfo.mCurrentPatchInfo == null) {
                        return null;
                    }
                }

                JSONObject obsoletedVer = jsonPatch.optJSONObject(TAG_OBSOLETED_VERSION);
                if (obsoletedVer != null) {
                    patchInfo.mObsoletedPatchInfo = BasicPatchInfo.parseFromJSON(obsoletedVer, patchInfo.mPatchName);
                    if (patchInfo.mObsoletedPatchInfo == null) {
                        return null;
                    }
                }

                return patchInfo;
            } catch (Exception e) {
                Log.d(LOG_TAG, "Parse PatchInfo from JSON exception " + e);
                return null;
            }
        }
    }
}
