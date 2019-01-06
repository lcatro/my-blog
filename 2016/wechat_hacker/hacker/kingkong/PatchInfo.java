package com.tencent.kingkong;

import com.tencent.kingkong.Common.Log;

public class PatchInfo {
	
	private final static String LOG_TAG = "PatchInfo";
	
	// Defined in PATCH_LIST_FILE
	public String name;
	public String index;
	public String enabled;
	public String type;					// Native, Dex, or both?	
	public String url;					// Reserved for patch download
	public String critical;				// Reserved for whether allow load patch fail
	public String priviousPatchIndex;	// Reserved for patch load sequence
	public String allowDelayLoad;		// Reserved for asyn-load 
	public String sha1;					// Reserved for patch download
	
	public SubPatches subPatches;

	public static final String PATCH_TYPE_NATIVE = "native"; 
	public static final String PATCH_TYPE_JAVA = "java";
	
	// For dex class loader
	public String dexPath;
	public String soPath;
	
	public boolean patchFileReady;

	public String curFingerprintStr = "";
	
	public boolean isEnabled() {
		return enabled.equals("true");
	}
	
	public boolean isAllowDelayLoad() {
		return allowDelayLoad.equals("true");
	}

	public boolean isCritical() {
		return critical.equals("true");
	}
	
	public boolean isPatchFileReady() {
		return patchFileReady;
	}
	
	public PatchInfo() {
		patchFileReady = false;
	}
	
	public void print() {
		Log.d(LOG_TAG, "I'm Patch " + index + " : " 
								+ name + ","
								+ enabled + ","
								+ type + ","								
								+ url + ","
								+ critical + ","
								+ priviousPatchIndex + ","
								+ allowDelayLoad + ","
								+ sha1);
	}
	
	public boolean isValidPatchType() {
		return type.equals(PATCH_TYPE_NATIVE) 
				|| type.equals(PATCH_TYPE_JAVA);
	}
	
	public void disable() {
		enabled = "false";
	}
	
};
