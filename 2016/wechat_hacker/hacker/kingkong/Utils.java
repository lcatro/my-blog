package com.tencent.kingkong;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.Signature;
import android.text.TextUtils;
import android.util.Base64;

import com.tencent.kingkong.Common.Log;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.security.MessageDigest;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class Utils {

    private static final String LOG_TAG = "KingKongUtils";

    public static String ReadFileToString(String fileName) {
        if (!new File(fileName).exists()) {
            Log.d(LOG_TAG, "file " + fileName  + " does not exist.");
            return "";
        }
        StringBuilder str = new StringBuilder();
        BufferedReader bf = null;
        try {
            bf = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)));
            String line;
            while ((line = bf.readLine()) != null) {
                str.append(line);
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

        return str.toString();
    }

    public static boolean checkFileSignature(String fileName, String signature) {
        JarFile jarFile = null;
        try {
            jarFile = new JarFile(fileName);
            Enumeration<JarEntry> entries = jarFile.entries();

            while (entries.hasMoreElements()) {
                JarEntry je = (JarEntry)entries.nextElement();
                if (je.isDirectory()) continue;
                if (je.getName().startsWith("META-INF/")) continue;

                Certificate[] certs = loadCertificates(jarFile, je);

                // Never allow un-certificated file
                // Never allow multi-certificated file
                if (certs == null) return false;
                if (certs.length != 1) return false;

                Signature sig = new Signature(certs[0].getEncoded());

                if (!sig.equals(new Signature(signature))) {
                    Common.Log.d(LOG_TAG, "Signature mismatch in : " + fileName);
                    return false;
                }
            }

        } catch (IOException e) {
            Common.Log.d(LOG_TAG, "Check file signature failed : " + e);
            return false;
        } catch (CertificateEncodingException e) {
            Common.Log.d(LOG_TAG, "Check file signature failed : " + e);
            return false;
        }

        Common.Log.d(LOG_TAG, "Check file signatures OK : " + fileName);
        return true;
    }

    private static Certificate[] loadCertificates(JarFile jarFile, JarEntry je) {
        try {
            byte[] readBuffer = new byte[8192];
            InputStream is = new BufferedInputStream(jarFile.getInputStream(je));
            while (is.read(readBuffer, 0, readBuffer.length) != -1) {
                // Do nothing
            }
            is.close();
            return je != null ? je.getCertificates() : null;
        } catch (IOException e) {

        } catch (RuntimeException e) {

        }
        return null;
    }

    public static boolean extractFile(String apkFileName, String fileNameInApk, String fullFileName) {
        return extractFile(apkFileName, fileNameInApk, fullFileName, false, "");
    }

    public static boolean verifyAndExtractFile(String apkFileName, String fileNameInApk, String fullFileName, String signature) {
        return extractFile(apkFileName, fileNameInApk, fullFileName, true, signature);
    }

    public static boolean extractFile(String apkFileName, String fileNameInApk, String fullFileName, boolean verifySig, String signature) {

        try {
            JarFile jarFile = new JarFile(apkFileName);
            Enumeration<JarEntry> entries = jarFile.entries();

            File file = new File(fullFileName);

            while (entries.hasMoreElements()) {
                JarEntry je = (JarEntry) entries.nextElement();
                if (je.isDirectory()) continue;
                if (je.getName().equals(fileNameInApk)) {
                    if (verifySig) {
                        Certificate[] certs = loadCertificates(jarFile, je);
                        if (certs == null) continue;
                        if (certs.length != 1) continue;

                        Signature sig = new Signature(certs[0].getEncoded());
                        if (!sig.equals(new Signature(signature))) {
                            Common.Log.d(LOG_TAG, "Extract " + fileNameInApk + " from " + apkFileName
                                    + " failed : Signature mismatch!");
                            return false;
                        }
                    }
                    if (readJarFile(jarFile, je, fullFileName)) {
                        Common.Log.d(LOG_TAG, "Extract " + fileNameInApk + " from " + apkFileName + " OK");
                        return true;
                    } else {
                        Common.Log.d(LOG_TAG, "Extract " + fileNameInApk + " from " + apkFileName
                                + " failed : Read error");
                        return false;
                    }
                }
            }

            Common.Log.d(LOG_TAG, "Extract " + fileNameInApk + " from " + apkFileName
                    + " failed : No such file in Apk");
            return false;

        } catch (Exception e) {
            Common.Log.d(LOG_TAG, "Extract " + fileNameInApk + " from " + apkFileName
                    + " failed : Exception " + e);
            return false;
        }
    }

    public static boolean copyFile(String dest, String src) {

        File file = new File(dest);
        if (file.exists()) {
            file.delete();
        }

        try {
            FileOutputStream fos = new FileOutputStream(dest, false);
            FileInputStream fis = new FileInputStream(src);
            byte buf[] = new byte[8192];

            do {
                int count = fis.read(buf);
                if (count <= 0) {
                    break;
                }
                fos.write(buf, 0, count);
            } while(true);
            fos.close();
            fis.close();

            Log.d(LOG_TAG, "Copy file " + src + " to " + dest + " OK");
            return true;
        } catch (IOException e) {
            Log.d(LOG_TAG, "Copy file " + src + " to " + dest + " failed:" + e);
            e.printStackTrace();
            return false;
        }
    }

    private static boolean readJarFile(JarFile jarFile, JarEntry je, String fileName) {
        try {
            byte[] readBuffer = new byte[8192];
            InputStream is = new BufferedInputStream(jarFile.getInputStream(je));
            FileOutputStream fos = new FileOutputStream(fileName, false);
            int length;
            while (true) {
                length = is.read(readBuffer, 0, readBuffer.length);
                if (length != -1) {
                    fos.write(readBuffer, 0, length);
                } else {
                    break;
                }
            }
            is.close();
            fos.close();
            return true;
        } catch (IOException e) {
            return false;
        } catch (RuntimeException e) {
            return false;
        }
    }

    public static String getFileSHA1String(String filePath) {
        try {
            return getSHA1String(new FileInputStream(filePath));
        } catch (FileNotFoundException e) {
            return "";
        }
    }

    public static String getSHA1String(byte[] buffer) {
        try {
            MessageDigest messageDigest = MessageDigest.getInstance("SHA1");
            messageDigest.reset();
            messageDigest.update(buffer, 0, buffer.length);
            return new String(android.util.Base64.encode(messageDigest.digest(), android.util.Base64.NO_WRAP));
        } catch (Throwable e) {
            Common.Log.d("Lynn", "Unable to calculate SHA1");
        }
        return "";
    }

    public static String getSHA1String(InputStream is) {
        try {
            BufferedInputStream bin = new BufferedInputStream(is);
            MessageDigest messageDigest = MessageDigest.getInstance("SHA1");
            messageDigest.reset();
            byte[] buffer = new byte[8192];
            int numRead = 0;
            while ((numRead = bin.read(buffer)) > 0)
                messageDigest.update(buffer, 0, numRead);
            bin.close();
            is.close();
            return Base64.encodeToString(messageDigest.digest(), Base64.NO_WRAP).trim();
        } catch (Throwable e) {
        }
        return "";
    }

    public static String getFileSHA1StringInApk(String apkFileName, String fileName) throws IOException {
        JarFile jarFile = new JarFile(apkFileName);
        Enumeration<JarEntry> entries = jarFile.entries();

        String SHA1String = "";
        while (entries.hasMoreElements()) {
            JarEntry je = (JarEntry)entries.nextElement();
            if (je.isDirectory()) continue;

            if (je.getName().equals(fileName)) {
                SHA1String = getSHA1String(jarFile.getInputStream(je));
                break;
            }
        }

        jarFile.close();
        return SHA1String;
    }

    public static boolean checkFileSHA1(String fileName, String sha1) {
        return getFileSHA1String(fileName).equals(sha1);
    }

    // Zap the whole folder recursively
    // But don't delete the folder itself!
    public static boolean ZapFolder(File folder) {

        if (!folder.exists()) {
            return false;
        }

        if (!folder.isDirectory()) {
            return false;
        }

        File[] childs = folder.listFiles();
        for (int i = 0; i < childs.length; i++) {
            File child = childs[i];
            if (child.isDirectory()) {
                if (!ZapFolder(child)) {
                    return false;
                }
            }
            if (!child.delete()) {
                return false;
            }
        }

        return true;
    }

    // Notice: Please note that this lock itself is NOT synchronized !!!
    public static class InterProcessLock {

        private String mLockFileName = "";
        private FileChannel mFileChannel = null;
        private FileLock mFileLock = null;

        private boolean mInitialized = false;
        private boolean mLocked = false;

        InterProcessLock(String lockFileName) {
            mLockFileName = lockFileName;
            try {
                mFileChannel = new RandomAccessFile(new File(mLockFileName), "rw").getChannel();
                mInitialized = true;
            } catch (Exception e) {
                Log.d(LOG_TAG, "Initial FileChannel exception : " + e);
                mInitialized = false;
            }
        }

        public boolean lock() {

            if (!mInitialized || mLocked) return false;

            Log.d(LOG_TAG, "Do Inter-Process Lock " + mLockFileName);

            try {
                mFileLock = mFileChannel.lock();
                mLocked = true;
                Log.d(LOG_TAG, "Do Inter-Process Lock OK " + mLockFileName);
                return true;
            } catch (Exception e) {
                Log.e(LOG_TAG, "Do Inter-Process Lock " + mLockFileName + " exception : " + e );
                if (mFileLock != null) {
                    try {
                        mFileLock.release();
                    } catch (IOException e1) {
                    }
                }
                if (mFileChannel != null) {
                    try {
                        mFileChannel.close();
                    } catch (IOException e1) {
                    }
                }
            }
            Log.d(LOG_TAG, "Do Inter-Process Lock failed " + mLockFileName);
            return false;
        }

        public void unlock() {

            if (!mLocked) return;

            Log.d(LOG_TAG, "Release Inter-Process Lock " + mLockFileName);

            if (mFileLock != null) {
                try {
                    mFileLock.release();
                } catch (IOException e) {
                    Log.e(LOG_TAG, "Release Inter-Process Lock " + mLockFileName + " exception : " + e );
                    e.printStackTrace();
                }
            }
            if (mFileChannel != null) {
                try {
                    mFileChannel.close();
                } catch (IOException e) {
                    Log.e(LOG_TAG, "Release Inter-Process Lock " + mLockFileName + " exception : " + e );
                }
            }

            mLocked = false;
        }

    }

    public static boolean download(String urlStr, String fileName) {

        // TODO : Test code, remove it
        if (urlStr.startsWith("/sdcard/")) {
            return Utils.copyFile(fileName, urlStr);
        }

        InputStream is = null;
        FileOutputStream fos = null;
        HttpURLConnection conn = null;

        try {
            URL url = new URL(urlStr);

            conn = (HttpURLConnection) url.openConnection();
            is = conn.getInputStream();
            fos = new FileOutputStream(fileName, false);

            conn.setConnectTimeout(300);
            conn.setReadTimeout(300);
            conn.connect();

            byte buf[] = new byte[1024];
            do {
                int numRead = is.read(buf);
                if (numRead <= 0) {
                    Log.d(LOG_TAG, "Download done : " + urlStr + ", " + fileName);
                    return true;
                }
                fos.write(buf, 0, numRead);
            } while (true);

        } catch (Throwable e) {
            Log.e(LOG_TAG, "Download exception " + urlStr + ", " + fileName + " : "+ e);
            return false;
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                }
            }
            if (conn != null) {
                conn.disconnect();
            }
        }
    }

    public static String getCurrentProcessName(Context context) {
        if (context == null) return "";

        int pid = android.os.Process.myPid();
        ActivityManager mActivityManager = (ActivityManager) context
                .getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningAppProcessInfo appProcess :
                mActivityManager.getRunningAppProcesses()) {
            if (appProcess.pid == pid) {
                return appProcess.processName;
            }
        }

        Log.d(LOG_TAG, "Unable to get current process name!");
        return "";
    }

    public static String getPackageName(Context context) {
        if (context == null) return "";

        try {
            String packageName = context.getPackageName();
            if (!TextUtils.isEmpty(packageName)) {
                return packageName;
            }
        } catch (Exception e) {
            return "";
        }
        return "";
    }

}
