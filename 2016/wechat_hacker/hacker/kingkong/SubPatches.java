package com.tencent.kingkong;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.tencent.kingkong.Common.Log;

public class SubPatches {
	
	private static final String LOG_TAG = "SubPatches";
	
	private final static String TAG_SUBPATCH_HEAD = "subpatches";
	private final static String TAG_SUBPATCH_COUNT = "subpatch_count";
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

	public ArrayList<SubPatch> subPatches = new ArrayList<SubPatch>();
	public int subPatchCount = 0;
	
	// Calculate Fingerprint via libraryNames and functionNames
	public int fingerprintCount = 0;
	public ArrayList<String> libraryNames = new ArrayList<String>();
	public ArrayList<String> functionNames = new ArrayList<String>();
	
	public String hookpointLibName = "";
	public String hookpointFuncName = "";
	
	public int parameterCount =0;
	public ArrayList<ParameterDef> parameterDefines = new ArrayList<ParameterDef>();

	public boolean parseFromFile(String fileName) {
		
		String jsonStr = getJsonString(fileName);
		JSONObject jsonObj = null;
		JSONArray jsonSubPatches = null;
		try {
			jsonObj = new JSONObject(jsonStr);
			
			fingerprintCount = jsonObj.getInt(TAG_FINGERPRINT_COUNT);
			JSONArray jsonFingerprintDef = jsonObj.getJSONArray(TAG_FINGERPRINT_DEFINITION);
			if (jsonFingerprintDef.length() != fingerprintCount) {
				Log.d(LOG_TAG, "Fingerprint count mismatch!");
				return false;
			}
			for (int i = 0; i < fingerprintCount; i++) {
				JSONObject jsonDef = jsonFingerprintDef.getJSONObject(i);
				String libraryName = jsonDef.getString(TAG_LIBRARY_NAME).trim();
				String functionName = jsonDef.getString(TAG_FUNCTION_NAME).trim();
				libraryNames.add(libraryName);
				functionNames.add(functionName);
				Log.d(LOG_TAG, "--> Fingerprint Library Name : " + libraryName + " ; Function Name : " + functionName);
			}
			
			parameterCount = jsonObj.getInt(TAG_PARAMETER_COUNT);
			JSONArray jsonParameterDef = jsonObj.getJSONArray(TAG_PARAMETER_DEFINE);
			if (parameterCount != jsonParameterDef.length()) {
				Log.d(LOG_TAG, "Parameter count mismatch :" + parameterCount + ", " + jsonParameterDef.length());
				return false;
			}
			for (int i = 0; i < parameterCount; i++) {
				ParameterDef param = new ParameterDef();
				if (param.parse(jsonParameterDef.getJSONObject(i))) {
					if (param.index != i) {
						// Parameter index is very very important!
						Log.d(LOG_TAG, "Parameter index error!");
						return false;
					}
					Log.d(LOG_TAG, "--> Parameter definition : " + param);
					parameterDefines.add(param);
				} else {
					return false;
				}

			}
			
			hookpointFuncName = jsonObj.getString(TAG_HOOKPOINT_FUNCTION_NAME).trim();
			hookpointLibName = jsonObj.getString(TAG_HOOKPOINT_LIBRARY_NAME).trim();
			Log.d(LOG_TAG, "--> Hookpoint library Name : " + hookpointLibName);
			Log.d(LOG_TAG, "--> Hookpoint function Name : " + hookpointFuncName);
			
			
			subPatchCount = jsonObj.getInt(TAG_SUBPATCH_COUNT);
			jsonSubPatches = jsonObj.getJSONArray(TAG_SUBPATCH_HEAD);
			for (int i = 0; i < jsonSubPatches.length(); i++) {
				Log.d(LOG_TAG, "Parseing sub patch " + i);
				
				SubPatch subPatch = new SubPatch();
				if (subPatch.parse(this, jsonSubPatches.getJSONObject(i))) {
					subPatch.print();
					subPatches.add(subPatch);
				} else {
					Log.d(LOG_TAG, "Parse sub patch failed, give up");
					return false;
				}
				
			}
			
			if (subPatchCount != subPatches.size()) {
				Log.d(LOG_TAG, "Sub patch count mismatch, give up");
				return false;
			}
			 
		} catch (JSONException e) {
			Log.d(LOG_TAG, "Parse sub patches failed : " + e);
			return false;
		}
		
		return true;
	}

	private static String getJsonString(String fileName) {

		StringBuilder stringBuilder = new StringBuilder();
		BufferedReader bf = null;
		try {
			bf = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)), 8192);
			String line;
			while ((line = bf.readLine()) != null) {
				stringBuilder.append(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (bf != null) {
				try {
					bf.close();
				} catch (IOException e) {
					
				}
			}
		}
		
		return stringBuilder.toString();
		
	}	
	
	public SubPatches() {};

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
