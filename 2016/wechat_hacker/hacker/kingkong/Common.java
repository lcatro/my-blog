package com.tencent.kingkong;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URLEncoder;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Build;

public class Common {
	public static final String PATCH_DRIVER_VERSION = "1.0";

	private static final String LOG_TAG = "KingKongCommon";

	private static final boolean ENABLE_DEBUG_LOG = true;
	private static final boolean ENABLE_ERROR_LOG = true;

	public static final int NOTIFICATION_TYPE_KINGKONG_PATCH = 2080;
	public static final int NOTIFICATION_TYPE_DO_PATCH_OK = NOTIFICATION_TYPE_KINGKONG_PATCH + 1;
	public static final int NOTIFICATION_TYPE_EVIL_CATCHED = NOTIFICATION_TYPE_KINGKONG_PATCH + 2;
	public static final int NOTIFICATION_TYPE_NATIVE_ERROR = NOTIFICATION_TYPE_KINGKONG_PATCH + 3;
	public static final int NOTIFICATION_TYPE_DO_PATCH_FAILED = NOTIFICATION_TYPE_KINGKONG_PATCH + 4;
	public static final int NOTIFICATION_TYPE_INITIAL_FAILED = NOTIFICATION_TYPE_KINGKONG_PATCH + 5;	
	public static final int NOTIFICATION_TYPE_REPORT_FINGERPRINT = NOTIFICATION_TYPE_KINGKONG_PATCH + 6;
	public static final int NOTIFICATION_TYPE_REPORT_POINT_LOG = NOTIFICATION_TYPE_KINGKONG_PATCH + 7;
	
	public static final int FAIL_REASON_INVALID_FINGERPRINT = 1;
	public static final int FAIL_REASON_NO_FINGERPRINT_MATCH = 2;
	public static final int FAIL_REASON_VERIFY_PARAM_ERROR = 3;
	public static final int FAIL_REASON_CALCULATE_PARAM_ERROR = 4;
	public static final int FAIL_REASON_NATIVE_DO_PATCH_ERROR = 5;
	public static final int FAIL_REASON_INVALID_HOOK_POINT = 6;
	public static final int FAIL_REASON_INVALID_JUMPER_POINT = 7;
	
	public static final String SHARED_PREFERENCE_NAME = "SHARED_PREFERENCE_KINGKONG_PATCH";
	public static final String SHARED_PREFERENCE_DPC_STATUS = "DPC_STATUS";
	public static final String SHARED_PREFERENCE_INITIAL_FAIL_REPORT = "INITIAL_FAILED";
	public static final String SHARED_PREFERENCE_DO_PATCH_OK_REPORT_PREFIX = "DO_PATCH_OK_";
	public static final String SHARED_PREFERENCE_DO_PATCH_FAIL_REPORT_PREFIX = "DO_PATCH_FAIL_";
	public static final String SHARED_PREFERENCE_PATCH_STATUS = "PATCH_STATUS";
	public static final String SHARED_PREFERENCE_POINT_LOG= "POINT_LOG";
	public static final String SHARED_PREFERENCE_POINT_LOG_CRASH_COUNT = "POINT_LOG_CRASH_COUNT";
	public static final String SHARED_PREFERENCE_POINT_LOG_CONTINUOUS_COUNT = "POINT_LOG_CONTINUOUS_COUNT";
	public static final String SHARED_PREFERENCE_INITIALIZE_STATUS = "INITIALIZE_STATUS";
	public static final String SHARED_PREFERENCE_LAST_UPDATE_TIME = "LAST_UPDATE_TIME";
	public static final String SHARED_PREFERENCE_UPDATE_STATUS = "UPDATE_STATUS";

	// See Common.h
	public final static int PARAM_TYPE_MIN = 0;
	public final static int PARAM_TYPE_OFFSET_TO_FUNCTION = 1;
	public final static int PARAM_TYPE_OFFSET_TO_FILE = 2;
	public final static int PARAM_TYPE_ARBITRARY_VALUE = 3;
	public final static int PARAM_TYPE_SYMBOL_ADDRESS = 4;
	public final static int PARAM_TYPE_MAX = PARAM_TYPE_SYMBOL_ADDRESS + 1;

	public final static int STATUS_UNINITIALIZED = 0;
	public final static int STATUS_INITIALIZED_PATCH_MANAGER = 1;
	public final static int STATUS_INITIALIZE_PATCH_MANAGER_FAILED = 2;
	public final static int STATUS_INITIALIZE_UPDATE_MANAGER_FAILED = 3;

	public static int mLogPoint = 0;

	private static Context mContext = null;
	private static ReportThread mReportThread = null;
	private static boolean mStatus = false;
	private static SharedPreferences mSharedPref = null;
	private static Editor mSharedPrefEditor = null;
	private static Utils.InterProcessLock mInterProcessLock = null;
	private static boolean isCheckedDeviceSupport = false; 
	private static boolean isDeviceSupportable = false;

	public static final String MAIN_INTERPROCESS_LOCK_FILE = "KingkongPatchInterprocess.Lock";
	public static final String UPDATE_INTERPROCESS_LOCK_FILE = "KingkongUpdateInterprocess.Lock";

	private static final String PATCH_FOLDER = "patches";
	private static final String PATCH_DOWNLOAD_FOLDER = "download";
	private static final String PATCH_ROOT_FOLDER = "kingkong";
	private static String mFilesDir = "";
	private static String mPatchDownloadFolder = "";
	private static String mPatchFolder = "";
	private static String mPathSeparator = File.separator;
	private static String mMainInterProcessFileName = "";
	private static String mUpdateInterProcessFileName = "";

	private final static String SIGNATURE = "30820254308201bda00302010202044c5eafe7300d06092a864886f70d01010b0500305d310b300906035504061302434e310b3009060355040813024744310b300906035504071302535a3110300e060355040a130754656e63656e743110300e060355040b130754656e63656e743110300e0603550403130754656e63656e74301e170d3134313033313038333935345a170d3135313033313038333935345a305d310b300906035504061302434e310b3009060355040813024744310b300906035504071302535a3110300e060355040a130754656e63656e743110300e060355040b130754656e63656e743110300e0603550403130754656e63656e7430819f300d06092a864886f70d010101050003818d0030818902818100b293a98fe569b7f9ba099e041c25038d8230e6fcbcee332499723e7d3c635795f6f8c04cdb25683080390119c4e5575bdf9d94b1969caeae09927ee38eb8e3ad9a5003a3dcc9055196341a50f5b06a6ec6e8c415ea8e42dee8d8838096022c3b54b299aafe3d2f934b65864506b379210382f826103476087d47c5191fb00e4b0203010001a321301f301d0603551d0e04160414b049af36c79e57278b3fda5ff8b1152ede6c83ca300d06092a864886f70d01010b0500038181003820e8817688a08d8bfef1cc3c5e7fe3343fa5786db96680d55a6d89145498fa1ae7f2de349e9deecd8ae9499e95a870f5810a1d9d81662f41ff29c23c0ddb51878b6926943fd5771d0e3dc463a7f0deb881355df3a45a206508ae5bc2c818038b0cd8fff3f52aeac3c70464c886917c67bc391fdae70a79fe02b9657190e6d4";

	// Called by JNI
	public static void NotificationFromNative(int type, int index, int value)	
	{
		Log.d("Lynn", "Notification From Native " + type + ", " + index + ", " + value);

		if (mReportThread != null) {
			mReportThread.report(type, index, value, "");
		}
	}

	// Called by JNI
	public Common() {
		
	}

	public static void reportFingerprint(String fingerprint, int[] params) {

		if (mReportThread != null) {
			Log.d("Lynn", "Report fingerprint : " + fingerprint);
			String paramStr = "";
			for (int i = 0; i < params.length; i++) {
				paramStr = paramStr + "&v" + (i + 1) +  "=" + params[i];
			}
			String extraInfo = "";
			try {
				extraInfo += "&str1=" + URLEncoder.encode(fingerprint, "UTF-8");
				extraInfo += paramStr;
				
				mReportThread.report(NOTIFICATION_TYPE_REPORT_FINGERPRINT, 0, 0, extraInfo);
				
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	public static void reportInitialFailed(int position) {
		Log.d("Lynn", "Do patch initial failed at " + position);

		String spString = SHARED_PREFERENCE_INITIAL_FAIL_REPORT;
		boolean reported = mSharedPref.getBoolean(spString, false);
		
		if (!reported) {
			if (mReportThread != null) {
				mReportThread.report(NOTIFICATION_TYPE_INITIAL_FAILED, position, 0, "");
			}
			mSharedPrefEditor.putBoolean(spString, true);
			mSharedPrefEditor.commit();
		}
	}

	public static void reportDoPatchFailed(int reason, String patchName, String fingerprint) {
		Log.d("Lynn", "Do patch failed " + reason + ", "+ patchName + ", " + fingerprint);
		
		String spString = SHARED_PREFERENCE_DO_PATCH_FAIL_REPORT_PREFIX + patchName;
		boolean reported = mSharedPref.getBoolean(spString, false);
		
		if (!reported) {
			if (mReportThread != null) {
				try {
					mReportThread.report(NOTIFICATION_TYPE_DO_PATCH_FAILED, reason, 0,
										"&str1=" + URLEncoder.encode(patchName, "UTF-8")
									  + "&str2=" + URLEncoder.encode(fingerprint, "UTF-8"));
				} catch (UnsupportedEncodingException e) {
					e.printStackTrace();
				}
			}

			mSharedPrefEditor.putBoolean(spString, true);
			mSharedPrefEditor.commit();
		}
	}

	public static void reportDoPatchOK(String patchName, String fingerprint) {
		Log.d("Lynn", "Do patch OK " + patchName + ", " + fingerprint);
		
		String spString = SHARED_PREFERENCE_DO_PATCH_OK_REPORT_PREFIX + patchName;
		String reported = mSharedPref.getString(spString, "");
		
		if (!reported.equals(fingerprint)) {
			if (mReportThread != null) {
				try {
					mReportThread.report(NOTIFICATION_TYPE_DO_PATCH_OK, 0, 0, 
										"&str1=" + URLEncoder.encode(patchName, "UTF-8")
									  + "&str2=" + URLEncoder.encode(fingerprint, "UTF-8"));
				} catch (UnsupportedEncodingException e) {
					e.printStackTrace();
				}
			}
			
			mSharedPrefEditor.putString(spString, fingerprint);
			mSharedPrefEditor.commit();
		}
	}	
	
	public static void reportPointLog() {
		int count = getPointLogCount();
		StringBuffer sb = new StringBuffer();
		int point = getPointLog();
		try {
			sb.append("&str1=").append(URLEncoder.encode(Integer.toString(count), "UTF-8")).append("&str2=")
					.append(URLEncoder.encode(Integer.toString(point), "UTF-8"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		if (mReportThread != null) {
			mReportThread.report(NOTIFICATION_TYPE_REPORT_POINT_LOG, 0, 0, sb.toString());
		}
	}
	
	public static void SetQQUni(String QQUni) {
		DataReport.setQQUni(QQUni);
	}

    public static boolean shouldLoadSecureLibrary() {
    	if (Build.VERSION.SDK_INT < 14 
    			|| Build.VERSION.SDK_INT > 19) {
    		return false;
    	}
    		
    	return android.os.Build.CPU_ABI.contains("armeabi");
    }
    
    public static void SetContext(Context context) {
    	if (mContext == null) {
        	mContext = context.getApplicationContext();        	
    	}    	
    }

	public synchronized static void SetSafeStatus(boolean on) {

    	if (!shouldLoadSecureLibrary()) return;

		if (mContext == null) return;

		mSharedPref = mContext.getSharedPreferences(SHARED_PREFERENCE_NAME, Context.MODE_PRIVATE);
		mSharedPrefEditor = mSharedPref.edit();

		mFilesDir =  mContext.getFilesDir().getAbsolutePath();
		mPatchDownloadFolder = mFilesDir + mPathSeparator + PATCH_ROOT_FOLDER + mPathSeparator + PATCH_DOWNLOAD_FOLDER;
		mPatchFolder = mFilesDir  + mPathSeparator + PATCH_ROOT_FOLDER + mPathSeparator + PATCH_FOLDER;

		mMainInterProcessFileName = mFilesDir  + mPathSeparator + MAIN_INTERPROCESS_LOCK_FILE;
		mUpdateInterProcessFileName = mFilesDir  + mPathSeparator + UPDATE_INTERPROCESS_LOCK_FILE;

		mInterProcessLock = new Utils.InterProcessLock(mMainInterProcessFileName);

		if (on && !mStatus && getDPCStatus() && getPatchStatus()) {
			if (!mInterProcessLock.lock()) return;

			DataReport.setContext(mContext);

			mReportThread = new ReportThread();
	  		mReportThread.start();

	    	if (!checkPatchStatus()) {
				mInterProcessLock.unlock();
				return;
			}

			int result = PatchManager.InitPatchManager(mContext);
			if (result != 0) {
				Common.SetInitializeStatus(Common.STATUS_INITIALIZE_PATCH_MANAGER_FAILED);
				Common.savePointLog(0);
				reportInitialFailed(result);
				mInterProcessLock.unlock();
				return;
			}

			UpdateManager.InitUpdateManager(mContext);

            if (UpdateManager.ShouldInstallUpdates()) {
                UpdateManager.InstallUpdates();
            } else if (UpdateManager.ShouldDoUpdate()) {
                UpdateManager.Update();
            } else if (UpdateManager.ShouldWaitUpdateFinish()) {
                // Only install update files in main process
                // Otherwise, if we overwrite files used by other process
                // e.g., the library, we'll cause other process crash
            } else {
                PatchManager.DoPatches();
            }

			mStatus = true;

			mInterProcessLock.unlock();
		}
		
	}

	private static boolean checkPatchStatus() {
		int count = getPointLogCount();
		if (count >= 0) {
			int point = getPointLog();
			if (point != 0) { // 上次失败
				setPointLogCount(count + 1);

				int crashCount = getPointLogContinuousCrashCount();
				if (crashCount >= 2) { // 连续出错次数�?3次，关闭组件
					setPatchStatus(false);
					reportPointLog();
					return false;
				} else {
					setPointLogContinuousCrashCount(crashCount + 1);
				}
			} else {
				setPointLogContinuousCrashCount(0);
			}
		}
		return true;
	}

	public static String GetDownloadFolder() {
		return mPatchDownloadFolder;
	}

	public static String GetPatchFolder() {
		return mPatchFolder;
	}

	public static String GetMainInterProcessLockFile() {
		return mMainInterProcessFileName;
	}

	public static String GetUpdateInterProcessLockFile() {
		return mUpdateInterProcessFileName;
	}

	public static String GetSignature() {
		return SIGNATURE;
	}

	public synchronized static void SetSafeStatusServer(Context context, boolean on) {

		if (context == null) return;
		mContext = context.getApplicationContext();
		
		if (on && !mStatus && getDPCStatus()) {
						
	  		mReportThread = new ReportThread();
	  		mReportThread.start();
	       	
	    	DataReport.setContext(mContext);
	       	
	    	mStatus = true;
		}
		
	}

    public static String makeDownloadName(String fileName) {
        return mPatchDownloadFolder + mPathSeparator + fileName;
    }

    public static String makeInstallName(String fileName) {
        return mPatchFolder + mPathSeparator + fileName;
    }

	private static void setPointLogCount(int count) {
		mSharedPrefEditor.putInt(SHARED_PREFERENCE_POINT_LOG_CRASH_COUNT, count);
		mSharedPrefEditor.commit();
	}

	private static int getPointLogCount() {
		return mSharedPref.getInt(SHARED_PREFERENCE_POINT_LOG_CRASH_COUNT, 0);
	}

	private static void setPointLogContinuousCrashCount(int count) {
		mSharedPrefEditor.putInt(SHARED_PREFERENCE_POINT_LOG_CONTINUOUS_COUNT, count);
		mSharedPrefEditor.commit();
	}

	private static int getPointLogContinuousCrashCount() {
		return mSharedPref.getInt(SHARED_PREFERENCE_POINT_LOG_CONTINUOUS_COUNT, 0);
	}

	private static void setPointLog(int point) {
		mSharedPrefEditor.putInt(SHARED_PREFERENCE_POINT_LOG, point);
		mSharedPrefEditor.commit();
	}

	private static int getPointLog() {
		return mSharedPref.getInt(SHARED_PREFERENCE_POINT_LOG, 0);
	}

	public static void savePointLog(int point) {
		setPointLog(point);
	}

	public static void setPatchStatus(boolean on) {
		mSharedPrefEditor.putBoolean(SHARED_PREFERENCE_PATCH_STATUS, on);
		mSharedPrefEditor.commit();
	}

	private static boolean getPatchStatus() {
		return mSharedPref.getBoolean(SHARED_PREFERENCE_PATCH_STATUS, true);
	}

	// DPC Status maybe called before SetContext
	// So we should not use mContext here!!!
	public static void SetDPCStatus(Context context, boolean on) {
		SharedPreferences sharedPreferences = context.getSharedPreferences(SHARED_PREFERENCE_NAME, Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();

		editor.putBoolean(SHARED_PREFERENCE_DPC_STATUS, on);
		editor.commit();
	}

	private static boolean getDPCStatus() {
		return mSharedPref.getBoolean(SHARED_PREFERENCE_DPC_STATUS, true);
	}

	public static int GetInitializeStatus() {
		return mSharedPref.getInt(SHARED_PREFERENCE_INITIALIZE_STATUS, STATUS_UNINITIALIZED);
	}

	public static void SetInitializeStatus(int status) {
		mSharedPrefEditor.putInt(SHARED_PREFERENCE_INITIALIZE_STATUS, status);
		mSharedPrefEditor.commit();
	}

	public static void SetLastUpdateTime(String lastUpdateTime) {
		mSharedPrefEditor.putString(SHARED_PREFERENCE_LAST_UPDATE_TIME, lastUpdateTime);
		mSharedPrefEditor.commit();
	}

	public static String GetLastUpdateTime() {
		return mSharedPref.getString(SHARED_PREFERENCE_LAST_UPDATE_TIME, "");
	}

	public static void SetUpdateStatus(String status) {
		mSharedPrefEditor.putString(SHARED_PREFERENCE_UPDATE_STATUS, status);
		mSharedPrefEditor.commit();
	}

	public static String GetUpdateStatus() {
		return mSharedPref.getString(SHARED_PREFERENCE_UPDATE_STATUS, "");
	}
	
	public static class Log {
		public static void d(String className, String msg) {
			if (ENABLE_DEBUG_LOG) {
				android.util.Log.d(Constant.LOG_TAG, className + " : " + msg);
			}
		}
		
		public static void e(String className, String msg) {
			if (ENABLE_ERROR_LOG) {
				android.util.Log.e(Constant.LOG_TAG, className + " : " + msg);
			}
		}
		
		public static void i(String className, String msg) {
			if (ENABLE_DEBUG_LOG) {
				android.util.Log.i(Constant.LOG_TAG, className + " : " + msg);
			}
		}
	}
	
	public static boolean isDeviceSupport() {
		try {
			if (isCheckedDeviceSupport)
				return isDeviceSupportable;

			if (!isX86CPU() && !isYunOS()) {
				isDeviceSupportable = true;
			} else {
				isDeviceSupportable = false;
			}
		} finally {
			Log.d(LOG_TAG, "device support is " + isDeviceSupportable + "checked" + isCheckedDeviceSupport);
			isCheckedDeviceSupport = true;
		}
		return isDeviceSupportable;
	}
	
	private static boolean isX86CPU() {
		Process process = null;
		String abi = null;
		InputStreamReader ir = null;
		BufferedReader input = null;
		try {
			process = Runtime.getRuntime().exec("getprop ro.product.cpu.abi");
			ir = new InputStreamReader(process.getInputStream());
			input = new BufferedReader(ir);
			abi = input.readLine();
			// TODO
			Log.i(LOG_TAG, "isX86CPU-abi = " + abi);
			if (abi.contains("x86")) {
				return true;
			}
		} catch (Exception e) {
		} finally {
			if (input != null) {
				try {
					input.close();
				} catch (Exception e) {
				}
			}

			if (ir != null) {
				try {
					ir.close();
				} catch (Exception e) {
				}
			}

			if (process != null) {
				try {
					process.destroy();
				} catch (Exception e) {
				}
			}
		}

		return false;
	}

	@SuppressLint("DefaultLocale")
	public static boolean isYunOS() {
		String s1 = null;
		String s2 = null;
		try {
			Method m = Class.forName("android.os.SystemProperties").getMethod(
					"get", String.class);
			s1 = (String) m.invoke(null, "ro.yunos.version");
			s2 = (String) m.invoke(null, "java.vm.name");
		} catch (NoSuchMethodException a) {
		} catch (ClassNotFoundException b) {
		} catch (IllegalAccessException c) {
		} catch (InvocationTargetException d) {
		}
		
		// TODO
		Log.i(LOG_TAG, "isYunOS-s1 = " + (s1 == null ? "" : s1));
		Log.i(LOG_TAG, "isYunOS-s2 = " + (s2 == null ? "" : s2));
		if ((s2 != null && s2.toLowerCase().contains("lemur")) || (s1 != null && s1.trim().length() > 0)) {
			return true;
		} else {
			return false;
		}
	}
}
