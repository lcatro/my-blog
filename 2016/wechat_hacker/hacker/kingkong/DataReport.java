package com.tencent.kingkong;

import java.io.File;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Proxy;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.os.StatFs;
import android.provider.Settings.Secure;
import android.telephony.CellIdentityCdma;
import android.telephony.CellIdentityGsm;
import android.telephony.CellIdentityLte;
import android.telephony.CellInfo;
import android.telephony.CellInfoCdma;
import android.telephony.CellInfoGsm;
import android.telephony.CellInfoLte;
import android.telephony.CellLocation;
import android.telephony.TelephonyManager;
import android.telephony.cdma.CdmaCellLocation;
import android.telephony.gsm.GsmCellLocation;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class DataReport {
	private static Context mContext;

	private static PackageManager mPackageManager = null;
	private static SensorManager mSensorManager = null;
	private static TelephonyManager mTelephonyManager = null;
	private static ConnectivityManager mConnectivetyManager = null;
	private static WifiManager mWifiManager = null;
	
	private static String mQQUni = "0";
	
	public static void setContext(Context context) {
		mContext = context;
	}

	private static synchronized PackageManager getPackageManager() {
		if (mContext != null && mPackageManager == null) {
			mPackageManager = mContext.getPackageManager();
		}
		return mPackageManager;
	}

	private static synchronized SensorManager getSensorManager() {
		if (mContext != null && mSensorManager == null) {
			mSensorManager = (SensorManager) mContext.getSystemService("sensor");
		}
		return mSensorManager;
	}

	private static synchronized TelephonyManager getTelephonyManager() {
		if (mContext != null && mTelephonyManager == null) {
			mTelephonyManager = (TelephonyManager) mContext.getSystemService("phone");
		}
		return mTelephonyManager;
	}

	private static synchronized ConnectivityManager getConnectivityManager() {
		if (mContext != null && mConnectivetyManager == null) {
			mConnectivetyManager = (ConnectivityManager) mContext.getSystemService("connectivity");
		}
		return mConnectivetyManager;
	}

	private static synchronized WifiManager getWifiManager() {
		if (mContext != null && mWifiManager == null) {
			mWifiManager = (WifiManager) mContext.getSystemService("wifi");
		}
		return mWifiManager;
	}

	public static String getHardware() {
		return android.os.Build.HARDWARE;
	}

	public static String getBrand() {
		return android.os.Build.BRAND;
	}

	public static int getOSSDK() {
		return android.os.Build.VERSION.SDK_INT;
	}

	public static String getAppVersion() {
		if (mContext == null) {
			return null;
		}
		try {
			PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0);
			if (packageInfo != null) {
				String versionName = packageInfo.versionName;
				if (!TextUtils.isEmpty(versionName)) {
					return versionName;
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getPackageName() {
		if (mContext == null) {
			return null;
		}
		try {
			String packageName = mContext.getPackageName();
			if (!TextUtils.isEmpty(packageName)) {
				return packageName;
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getAccelerometerName() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
				if (sensor != null) {
					return sensor.getName();
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getAccelerometerVendor() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
				if (sensor != null) {
					return sensor.getVendor();
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getAccessSubtype() {
		try {
			TelephonyManager tm = getTelephonyManager();
			if (tm != null) {
				switch (tm.getNetworkType()) {
				case TelephonyManager.NETWORK_TYPE_GPRS:
				case TelephonyManager.NETWORK_TYPE_EDGE:
				case TelephonyManager.NETWORK_TYPE_CDMA:
				case TelephonyManager.NETWORK_TYPE_1xRTT:
				case TelephonyManager.NETWORK_TYPE_IDEN: {
					return "2G";
				}
				case TelephonyManager.NETWORK_TYPE_LTE: {
					return "4G";
				}
				case TelephonyManager.NETWORK_TYPE_UMTS:
				case TelephonyManager.NETWORK_TYPE_EVDO_0:
				case TelephonyManager.NETWORK_TYPE_EVDO_A:
				case TelephonyManager.NETWORK_TYPE_HSDPA:
				case TelephonyManager.NETWORK_TYPE_HSUPA:
				case TelephonyManager.NETWORK_TYPE_HSPA:
				case TelephonyManager.NETWORK_TYPE_EVDO_B:
				case TelephonyManager.NETWORK_TYPE_EHRPD:
				case TelephonyManager.NETWORK_TYPE_HSPAP: {
					return "3G";
				}
				}
			}
		} catch (Exception e) {
			return null;
		}
		return "unknown";
	}

	public static int getBatteryLevel() {
		int level = -1;
		try {
			if (mContext != null) {
				Intent intent = mContext.registerReceiver(null, new IntentFilter(
						"android.intent.action.BATTERY_CHANGED"));
				if (intent != null) {
					level = intent.getIntExtra("level", -1);
				}
			}
		} catch (Exception e) {
			return level;
		}
		return level;
	}

	public static int getBatteryTemp() {
		int level = -1;
		try {
			if (mContext != null) {
				Intent intent = mContext.registerReceiver(null, new IntentFilter(
						"android.intent.action.BATTERY_CHANGED"));
				if (intent != null) {
					level = intent.getIntExtra("temperature", -1);
				}
			}
		} catch (Exception e) {
			return level;
		}
		return level;
	}

	public static int getBatteryVoltage() {
		int level = -1;
		try {
			if (mContext != null) {
				Intent intent = mContext.registerReceiver(null, new IntentFilter(
						"android.intent.action.BATTERY_CHANGED"));
				if (intent != null) {
					level = intent.getIntExtra("voltage", -1);
				}
			}
		} catch (Exception e) {
			return level;
		}
		return level;
	}

	public static String getCarrier() {
		try {
			TelephonyManager tm = getTelephonyManager();
			if (tm != null) {
				String networkOperatorName = tm.getNetworkOperatorName();
				if (!TextUtils.isEmpty(networkOperatorName)) {
					return networkOperatorName;
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}
	

    private static String getCellIdentity(CellInfo arg13) {
        return null;
    }

	private static String formatCellInfo(String cellInfo) {
		String result = null;
		if (cellInfo != null) {
			String tempInfo = cellInfo.trim();
			if (tempInfo.length() != 0) {
				result = tempInfo;
			}
			if (result.charAt(0) == 34) {
				return result;
			}
			if (result.charAt(0) == 91) {
				return result;
			}
			if (result.charAt(0) == 123) {
				return result;
			}
			result = "\"" + result + "\"";
		}
		return result;
	}

	public static String getCellInfo() {
		return null;
	}

	public static String getGravityName() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_GRAVITY);
				if (sensor != null) {
					return sensor.getName();
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getGravityVendor() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_GRAVITY);
				if (sensor != null) {
					return sensor.getVendor();
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getImsi() {
		try {
			TelephonyManager tm = getTelephonyManager();
			if (tm != null) {
				String subscriberId = tm.getSubscriberId();
				if (!TextUtils.isEmpty(subscriberId)) {
					return subscriberId;
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getIpAddr() {
		try {
			ConnectivityManager cm = getConnectivityManager();
			if (cm != null) {
				NetworkInfo info = cm.getActiveNetworkInfo();
				if (info != null && info.isAvailable()) {
					info = cm.getNetworkInfo(1);
					if (info != null && info.isAvailable()) {
						WifiManager wifiManager = getWifiManager();
						if (wifiManager != null && wifiManager.isWifiEnabled()) {
							WifiInfo wifiInfo = wifiManager.getConnectionInfo();
							int ipAddress = wifiInfo.getIpAddress();
							return (ipAddress & 0xFF) + "." + ((ipAddress >> 8) & 0xFF) + "."
									+ ((ipAddress >> 16) & 0xFF) + "." + (ipAddress >> 24 & 0xFF);
						}
					}

					info = cm.getNetworkInfo(0);
					if (info != null && info.isAvailable()) {
						for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en
								.hasMoreElements();) {
							NetworkInterface intf = en.nextElement();
							for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr
									.hasMoreElements();) {
								InetAddress inetAddress = enumIpAddr.nextElement();
								if (!inetAddress.isLoopbackAddress()) {
									return inetAddress.getHostAddress().toString();
								}
							}
						}
					}
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}

	public static String getResolution() {
		return null;
	}

	public static String getRomTotal() {
		try {
			File file = Environment.getDataDirectory();
			if (file == null || file.getAbsolutePath().length() == 0) {
				return null;
			}
			StatFs sf = new StatFs(file.getPath());
			if (android.os.Build.VERSION.SDK_INT < 18) {
				return (long) sf.getBlockCount() * (long) sf.getBlockSize() + "";
			} else {
				Object total = sf.getClass().getDeclaredMethod("getTotalBytes").invoke(sf);
				if (total != null) {
					return ((Long) total).toString();
				}
			}

		} catch (Exception e) {
		}
		return null;
	}

	public static String getSDCardTotal() {
		try {
			if (!"mounted".equals(Environment.getExternalStorageState())) {
				return null;
			}

			File file = Environment.getExternalStorageDirectory();
			if (file == null || file.getAbsolutePath().length() == 0) {
				return null;
			}
			StatFs sf = new StatFs(file.getPath());
			if (android.os.Build.VERSION.SDK_INT < 18) {
				return (long) sf.getBlockCount() * (long) sf.getBlockSize() + "";
			} else {
				Object total = sf.getClass().getDeclaredMethod("getTotalBytes").invoke(sf);
				if (total != null) {
					return ((Long) total).toString();
				}
			}

		} catch (Exception e) {
		}
		return null;
	}

	public static boolean isRoot() {
		boolean isRoot = true;
		boolean temp;
		Object secure;
		try {
			secure = Class.forName("android.os.SystemProperties").getMethod("get", String.class)
					.invoke(null, "ro.secure");

		} catch (Exception e) {
			secure = null;
		}
		if (secure != null && ("1".equals(secure))) {
			temp = false;
			if (!new File("/system/bin/su").exists() && !new File("/system/xbin/su").exists()) {
				isRoot = temp;
			}
			return isRoot;
		}
		if (secure != null && ("0".equals(secure))) {
			temp = true;
		} else {
			temp = false;
		}
		if (temp) {
			isRoot = temp;
		} else {
			if (!new File("/system/bin/su").exists() && !new File("/system/xbin/su").exists()) {
				isRoot = temp;
			}
		}
		return isRoot;
	}

	public static boolean isAccelerometerSupported() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
				if (sensor != null) {
					return true;
				}
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isBlueToothLeSupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.bluetooth_le");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isBlueToothSupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.bluetooth");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isGpsLocationSupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.location.gps");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isGravitySupported() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_GRAVITY);
				if (sensor != null) {
					return true;
				}
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isGyroscopeSupported() {
		try {
			SensorManager sm = getSensorManager();
			if (sm != null) {
				Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
				if (sensor != null) {
					return true;
				}
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isNfcSupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.nfc");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}

	public static boolean isProxyEnabled() {
		try {
			String proxyHost = null;
			if (android.os.Build.VERSION.SDK_INT > 14) {
				proxyHost = System.getProperty("http.proxyHost");
			} else if (mContext != null) {
				proxyHost = Proxy.getHost(mContext);
			}
			if (!TextUtils.isEmpty(proxyHost)) {
				return true;
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}
	
	public static boolean isTelephonySupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.telephony");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}
	
	public static boolean isUsbAccessorySupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.usb.accessory");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}
	
	public static boolean isWifiSupported() {
		try {
			PackageManager pm = getPackageManager();
			if (pm != null) {
				return pm.hasSystemFeature("android.hardware.wifi");
			}
		} catch (Exception e) {
			return false;
		}
		return false;
	}
	
	public static String getQQUin() {
		return mQQUni;
	}
	
	public static void setQQUni(String QQUni) {
		mQQUni = QQUni;
	}
	
	// TODO
	public static String getWeixinUin() {
		return null;
	}
	
	/**
	 * 获取系统的androidID，不�?要权�?
	 * ANDROID_ID是设备第�?次启动时产生和存储的64bit的一个数，当设备被wipe后该数重�?
	 * 有缺陷：
	 * 1、它在Android <=2.1 or Android >=2.3的版本是可靠、稳定的，但�?2.2的版本并不是100%可靠�?
	 * 2、在主流厂商生产的设备上，有�?个很经常的bug，就是每个设备都会产生相同的ANDROID_ID�?9774d56d682e549c
	 * @param context
	 * @return
	 */
	public static String getAndroidID() {
		if (mContext == null) {
			return null;
		}
		String androidID = null;
		try {
			androidID = Secure.getString(mContext.getContentResolver(), Secure.ANDROID_ID);
		} catch (Exception e) {
		}
		return androidID;
	}

	/**
	 * 获取手机型号
	 * 
	 * @return
	 */
	public static String getModel() {
		return android.os.Build.MODEL;
	}
	
	/**
	 * 获取系统的版本号
	 * 
	 * @return
	 */
	public static String getOSVersion() {
		return android.os.Build.VERSION.RELEASE;
	}
	
	/**
	 * 返回mac地址，需要权限ACCESS_WIFI_STATE
	 * 有缺陷：
	 * 在某些山寨机，这个mac地址是软件生成的，会动�?�变�?
	 * @param context
	 * @return
	 */
	public static String getLocalMacAddress() {
		String mac = null;
		try {
			WifiManager wifi = getWifiManager();
			if (wifi != null) {
				WifiInfo info = wifi.getConnectionInfo();
				if (info != null) {
					mac = info.getMacAddress();
				}
			}
		} catch (Exception e) {
		}
		return mac;
	}
	
	/**
	 * 根据不同的手机设备返回IMEI，MEID或�?�ESN�?,�?要权限READ_PHONE_STATE
	 * 有缺陷：
	 * 在少数的�?些手机设备上，该实现有漏洞，会返回垃�?
	 * @param context
	 * @return
	 */
	public static String getDeviceId() {
		try {
			TelephonyManager tm = getTelephonyManager();
			if (tm != null) {
				String deviceId = tm.getDeviceId();
				if (!TextUtils.isEmpty(deviceId)) {
					return deviceId;
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}
	
	public static String getNetworkType() {
		try {
			ConnectivityManager cm = getConnectivityManager();
			if (cm != null) {
				NetworkInfo info = cm.getActiveNetworkInfo();
				if (info != null && info.isConnected()) {
					switch (info.getType()) {
					case ConnectivityManager.TYPE_WIFI:
						return info.getTypeName();
					case ConnectivityManager.TYPE_MOBILE:
						return info.getSubtypeName();
					default:
						return null;
					}
				}
			}
		} catch (Exception e) {
			return null;
		}
		return null;
	}
}
