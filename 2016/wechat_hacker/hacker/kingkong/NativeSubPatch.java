package com.tencent.kingkong;

import android.util.JsonWriter;

import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.tencent.kingkong.Common.Log;

public class NativeSubPatch {

	private static final String LOG_TAG = "KingKongSubPatch";
	
	private final static String TAG_FINGERPRINTS = "fingerprints";
	private final static String TAG_PARAMETERS = "parameters";
	private final static String TAG_HOOK_POINT = "hook_point";
	private final static String TAG_JUMPER_POINT = "jumper_point";	
	private final static String TAG_PATCH_FILE = "patch_file";

	public int hookPoint;
	public int jumperPoint;
	
	public String patchFileName;
	
	public ArrayList<Integer> parameters = new ArrayList<Integer>();
	public ArrayList<String> fingerprints = new ArrayList<String>();
		
	public NativeSubPatch() {}

	public boolean parseFromJSON(JSONObject jsonObj) {
		
		try {
			patchFileName = jsonObj.getString(TAG_PATCH_FILE).trim();
			
			hookPoint = jsonObj.getInt(TAG_HOOK_POINT);
			jumperPoint = jsonObj.getInt(TAG_JUMPER_POINT);

			JSONArray jsonParameters = jsonObj.getJSONArray(TAG_PARAMETERS);
			JSONArray jsonFingerprints = jsonObj.getJSONArray(TAG_FINGERPRINTS);

            // TODO: Verify length
            int parameterCount = jsonParameters.length();
            int fingerprintCount = jsonFingerprints.length();

			for (int i = 0; i < parameterCount; i++) {
				int value = jsonParameters.getInt(i);
				parameters.add(value);
			}

			for (int i = 0; i < fingerprintCount; i++) {
				fingerprints.add(jsonFingerprints.getString(i).trim());
			}
			
		} catch (JSONException e) {
			Log.d(LOG_TAG, "Parse SubPatch error : " + e);
			return false;
		} catch (Exception e) {
			return false;
		}
		
		return true;
	}

    public String getFingerprintStr() {
        String fingerprintStr = "";
        for (int i = 0; i < fingerprints.size(); i++) {
            if (i != 0) {
                fingerprintStr += ",";
            }
            fingerprintStr += fingerprints.get(i);
        }
        return fingerprintStr;
    }

	public void print() {
		Log.d(LOG_TAG, "--> HookPoint : " + hookPoint + " ; JumperPoint : " + jumperPoint + "\n");
		for (int i = 0; i < parameters.size(); i++) {
			Log.d(LOG_TAG, "--> Parameters : " + parameters.get(i).toString());
		}
		
		for (int i = 0; i < fingerprints.size(); i++) {
			Log.d(LOG_TAG, "--> FingerPrint : " + fingerprints.get(i));
		}
	}

    public static ArrayList<NativeSubPatch> parseSubPatchesFromFile(String fileName) {
        ArrayList<NativeSubPatch> nativeSubPatches = new ArrayList<NativeSubPatch>();
        try {
            String jsonStr = Utils.ReadFileToString(fileName);
            JSONArray jsonArray = new JSONArray(jsonStr);

            for (int i = 0; i < jsonArray.length(); i++) {
                Log.d(LOG_TAG, "Parsing sub patch " + i);

                NativeSubPatch nativeSubPatch = new NativeSubPatch();
                if (nativeSubPatch.parseFromJSON(jsonArray.getJSONObject(i))) {
                    nativeSubPatch.print();
                    nativeSubPatches.add(nativeSubPatch);
                } else {
                    Log.d(LOG_TAG, "Parse sub patch failed, give up");
                    return null;
                }
            }

            return nativeSubPatches;
        } catch (Exception e) {
            Log.d(LOG_TAG, "Parse SubPatch List exception : " + e);
            return null;
        }
    }

    public static boolean generateConfigFile(String fileName, ArrayList<NativeSubPatch> subPatches) {
        // TODO: todododododotodotodo
        /*
        try {
            FileOutputStream fos = new FileOutputStream(fileName);
            JsonWriter writer = new JsonWriter(new OutputStreamWriter(fos, "UTF-8"));
        } catch (Exception e) {
            return false;
        }
        return true;
        */
        return true;
    }

}