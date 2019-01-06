package com.tencent.kingkong;

import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.tencent.kingkong.Common.Log;

public class SubPatch {

	private static final String LOG_TAG = "SubPatch";
	
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
		
	public SubPatch() {}
	
	public boolean parse(SubPatches parent,JSONObject jsonObj) {
		
		try {
			patchFileName = jsonObj.getString(TAG_PATCH_FILE).trim();
			
			hookPoint = jsonObj.getInt(TAG_HOOK_POINT);
			jumperPoint = jsonObj.getInt(TAG_JUMPER_POINT);
			
			int parameterCount = parent.parameterCount;
			int fingerprintCount = parent.fingerprintCount;
			
			JSONArray jsonParameters = jsonObj.getJSONArray(TAG_PARAMETERS);
			JSONArray jsonFingerprints = jsonObj.getJSONArray(TAG_FINGERPRINTS);
			
			if (jsonParameters.length() != parameterCount) {
				Log.d(LOG_TAG, "Parameter count error!");
				return false;
			}
			for (int i = 0; i < parameterCount; i++) {
				int value = jsonParameters.getInt(i);
				parameters.add(value);
			}
			
			if (jsonFingerprints.length() != fingerprintCount) {
				Log.d(LOG_TAG, "Fingerprint count error!");
				return false;
			}
			for (int i = 0; i < fingerprintCount; i++) {
				fingerprints.add(jsonFingerprints.getString(i).trim());
			}
			
		} catch (JSONException e) {
			Log.d(LOG_TAG, "Parse subpatch error : " + e);
			return false;
		} catch (Exception e) {
			return false;
		}
		
		return true;
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
	
}