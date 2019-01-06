//
// Created by lctang on 2016/10/12.
//

#include <jni.h>

#include "main.h"

JNIEXPORT jint JNICALL Java_wechat_hacker_root_try_1root(JNIEnv *env, jclass class) {
    return try_root();
}

JNIEXPORT jint JNICALL Java_wechat_hacker_root_root_1shell(JNIEnv *env, jclass class, jstring input_command) {
    return 0;//system((*env)->GetStringUTFChars(input_command,NULL));
}
