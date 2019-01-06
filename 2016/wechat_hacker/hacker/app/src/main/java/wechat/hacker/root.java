package wechat.hacker;

/**
 * Created by lctang on 2016/10/12.
 */
public class root {

    static {
        System.loadLibrary("app");
    }

    public static native int try_root();
    public static native int root_shell(String command);

}
