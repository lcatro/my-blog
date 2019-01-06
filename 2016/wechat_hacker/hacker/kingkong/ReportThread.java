package com.tencent.kingkong;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;

import com.tencent.kingkong.Common.Log;

public class ReportThread extends Thread {

	private static final String LOG_TAG = "KingKongReportThread";

	public static final int MSG_CONSOLE = 0;
	public static final int MSG_WEBVIEW = 1;
	
	public static final String KEY_TYPE = "KEY_TYPE";
	public static final String KEY_INDEX = "KEY_INDEX";
	public static final String KEY_SUBINDEX = "KEY_SUBINDEX";	
	public static final String KEY_EXTRAINFO = "KEY_EXTRAINFO";
	
	private ReportHandler mHandler = null;
	private Looper mLooper = null;

	private Object mLock = new Object();	// To protect mHanlder & mLooper

	public void report(int type, int index, int subIndex, String extraInfo) {
		if (mHandler == null) {
			return;
		}
		
		synchronized(mLock) {
			if (mHandler != null) {
				Message msg = mHandler.obtainMessage();
				Bundle data = new Bundle();
				data.putInt(KEY_TYPE, type);
				data.putInt(KEY_INDEX, index);
				data.putInt(KEY_SUBINDEX, subIndex);
				data.putString(KEY_EXTRAINFO, extraInfo);
				msg.setData(data);
				mHandler.sendMessage(msg);
			}
		}		
	}
		
	public void stopThread() {
		if (mLooper != null) {
			synchronized(mLock) {
				if (mLooper != null) {
					mLooper.quit();
					mHandler.removeCallbacksAndMessages(null);
					mHandler = null;
					mLooper = null;
				}
			}
		}
	}

	static class ReportHandler extends Handler {
		
		@Override
		public void handleMessage(Message msg) {
			Bundle data = msg.getData();
					
			String type = String.valueOf(data.getInt(KEY_TYPE));
			String index = String.valueOf(data.getInt(KEY_INDEX));
			String subIndex = String.valueOf(data.getInt(KEY_SUBINDEX));
			String extraInfo = data.getString(KEY_EXTRAINFO);
		
			String urlStr = null;
			try {
				urlStr = "http://zyjc.sec.qq.com/reportFixer.php"
						+ "?type=" + URLEncoder.encode(type, "UTF-8")
					   	+ "&idx=" + URLEncoder.encode(index, "UTF-8")
					   	+ "&subIdx=" + URLEncoder.encode(subIndex, "UTF-8")
					   	+ extraInfo;
			} catch (UnsupportedEncodingException e1) {
				e1.printStackTrace();
			}
			if (TextUtils.isEmpty(urlStr)) {
				return;
			}
			urlStr = addDeviceInfoToUrl(urlStr);

			Log.d(LOG_TAG, urlStr);

			// Send the error to server
			HttpURLConnection connection = null;
			InputStreamReader in = null;
			try {
				URL url = new URL(urlStr);
				connection = (HttpURLConnection) url.openConnection();
				in = new InputStreamReader(connection.getInputStream());
	            BufferedReader bufferedReader = new BufferedReader(in);
	            while (bufferedReader.readLine() != null) {
               
	            }
			} catch (Throwable e) {
				StringWriter errors = new StringWriter();
				e.printStackTrace(new PrintWriter(errors));
				Log.e(LOG_TAG, "ReportHandler - exceptions1:" + errors.toString());
			} finally {
				if (connection != null) {
					connection.disconnect();
				}
				if (in != null) {
					try {
						in.close();
					} catch (Throwable e) {
						StringWriter errors = new StringWriter();
						e.printStackTrace(new PrintWriter(errors));
						Log.e(LOG_TAG, "ReportHandler - exceptions2:" + errors.toString());
					} 
				}
			}
			
			Log.d(LOG_TAG, "Send done");
		}
	};
			
	
	public void run() {
		
		Looper.prepare();
		
		mHandler = new ReportHandler();
		
		mLooper = Looper.myLooper();
		
		Looper.loop();
	}

	public static synchronized String addDeviceInfoToUrl(String url) {
		StringBuffer sb = new StringBuffer(url);
		String tempStr = null;
		int tempInt = -1;
		try {
			tempStr = DataReport.getHardware();
			if (tempStr != null) {
				sb.append("&hardware=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getBrand();
			if (tempStr != null) {
				sb.append("&pb=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempInt = DataReport.getOSSDK();
			if (tempInt != -1) {
				sb.append("&ps=").append(URLEncoder.encode(Integer.toString(tempInt), "UTF-8"));
			}
			tempStr = DataReport.getAppVersion();
			if (tempStr != null) {
				sb.append("&av=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getPackageName();
			if (tempStr != null) {
				sb.append("&pn=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getAccelerometerName();
			if (tempStr != null) {
				sb.append("&amn=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getAccelerometerVendor();
			if (tempStr != null) {
				sb.append("&amv=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getAccessSubtype();
			if (tempStr != null) {
				sb.append("&nm=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempInt = DataReport.getBatteryLevel();
			if (tempInt != -1) {
				sb.append("&bl=").append(URLEncoder.encode(Integer.toString(tempInt), "UTF-8"));
			}
			tempInt = DataReport.getBatteryTemp();
			if (tempInt != -1) {
				sb.append("&bt=").append(URLEncoder.encode(Integer.toString(tempInt), "UTF-8"));
			}
			tempInt = DataReport.getBatteryVoltage();
			if (tempInt != -1) {
				sb.append("&bv=").append(URLEncoder.encode(Integer.toString(tempInt), "UTF-8"));
			}
			tempStr = DataReport.getCarrier();
			if (tempStr != null) {
				sb.append("&cr=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getCellInfo();
			if (tempStr != null) {
				sb.append("&cl=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getGravityName();
			if (tempStr != null) {
				sb.append("&gn=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getGravityVendor();
			if (tempStr != null) {
				sb.append("&gv=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getImsi();
			if (tempStr != null) {
				sb.append("&imsi=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getIpAddr();
			if (tempStr != null) {
				sb.append("&ip_addr=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getResolution();
			if (tempStr != null) {
				sb.append("&re=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getRomTotal();
			if (tempStr != null) {
				sb.append("&rt=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getSDCardTotal();
			if (tempStr != null) {
				sb.append("&st=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			
			sb.append("&root=").append(URLEncoder.encode(DataReport.isRoot() ? "1" : "0", "UTF-8"));
			sb.append("&ams=").append(URLEncoder.encode(DataReport.isAccelerometerSupported() ? "1" : "0", "UTF-8"));
			sb.append("&btls=").append(URLEncoder.encode(DataReport.isBlueToothLeSupported() ? "1" : "0", "UTF-8"));
			sb.append("&bts=").append(URLEncoder.encode(DataReport.isBlueToothSupported() ? "1" : "0", "UTF-8"));
			sb.append("&gls=").append(URLEncoder.encode(DataReport.isGpsLocationSupported() ? "1" : "0", "UTF-8"));
			sb.append("&gs=").append(URLEncoder.encode(DataReport.isGravitySupported() ? "1" : "0", "UTF-8"));
			sb.append("&gss=").append(URLEncoder.encode(DataReport.isGyroscopeSupported() ? "1" : "0", "UTF-8"));
			sb.append("&ns=").append(URLEncoder.encode(DataReport.isNfcSupported() ? "1" : "0", "UTF-8"));
			sb.append("&pe=").append(URLEncoder.encode(DataReport.isProxyEnabled() ? "1" : "0", "UTF-8"));
			sb.append("&ts=").append(URLEncoder.encode(DataReport.isTelephonySupported() ? "1" : "0", "UTF-8"));
			sb.append("&uas=").append(URLEncoder.encode(DataReport.isUsbAccessorySupported() ? "1" : "0", "UTF-8"));
			sb.append("&ws=").append(URLEncoder.encode(DataReport.isWifiSupported() ? "1" : "0", "UTF-8"));

			tempStr = DataReport.getQQUin();
			if (tempStr != null) {
				sb.append("&qu=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getWeixinUin();
			if (tempStr != null) {
				sb.append("&wu=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getAndroidID();
			if (tempStr != null) {
				sb.append("&android_id=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getModel();
			if (tempStr != null) {
				sb.append("&pm=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getOSVersion();
			if (tempStr != null) {
				sb.append("&pr=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getLocalMacAddress();
			if (tempStr != null) {
				sb.append("&mac=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getDeviceId();
			if (tempStr != null) {
				sb.append("&device_id=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
			tempStr = DataReport.getNetworkType();
			if (tempStr != null) {
				sb.append("&nt=").append(URLEncoder.encode(tempStr, "UTF-8"));
			}
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return sb.toString();
	}

}
