#include <jni.h>
#include <string>
#include "art.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_spx_simplereflectionandroid12_MainActivity_enableFreeReflection(JNIEnv *env,
                                                                         jobject thiz,
                                                                         jint targetSdkVersion) {
    return unseal(env, targetSdkVersion);
}