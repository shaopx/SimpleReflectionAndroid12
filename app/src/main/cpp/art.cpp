#include "art.h"
#include <android/log.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sys/system_properties.h>

#define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_INFO, "FreeReflect", __VA_ARGS__))

template<typename T>
int findOffset(void *start, int regionStart, int regionEnd, T value) {

    if (NULL == start || regionEnd <= 0 || regionStart < 0) {
        return -1;
    }
    char *c_start = (char *) start;

    for (int i = regionStart; i < regionEnd; i += 4) {
        T *current_value = (T *) (c_start + i);
        if (value == *current_value) {
            LOGV("found offset: %d", i);
            return i;
        }
    }
    return -2;
}

template<typename Runtime>
int unseal0(PartialRuntimeS *partialRuntime) {
    LOGV("hidden api policy before : %d", partialRuntime->hidden_api_policy_);
    LOGV("fingerprint: %s", partialRuntime->fingerprint_.c_str());
    partialRuntime->hidden_api_policy_ = EnforcementPolicy::kNoChecks;
    LOGV("hidden api policy after: %d", partialRuntime->hidden_api_policy_);
    return 0;
}

int unseal(JNIEnv *env, jint targetSdkVersion) {

    char api_level_str[5];
    char preview_api_str[5];
    __system_property_get("ro.build.version.sdk", api_level_str);
    __system_property_get("ro.build.version.preview_sdk", preview_api_str);

    int api_level = atoi(api_level_str);
    LOGV("api_level: %d", api_level);
    bool is_preview = atoi(preview_api_str) > 0;
    LOGV("is_preview: %d", is_preview);

    bool isAndroidS = api_level >= 31 || (api_level == 29 && is_preview);
    LOGV("isAndroidS: %d", isAndroidS);

    JavaVM *javaVM;
    env->GetJavaVM(&javaVM);

    JavaVMExt *javaVMExt = (JavaVMExt *) javaVM;
    void *runtime = javaVMExt->runtime;

    LOGV("runtime ptr: %p, vmExtPtr: %p", runtime, javaVMExt);

    const int MAX = 3000;
    int offsetOfVmExt = findOffset(runtime, 0, MAX, (size_t) javaVMExt);
    LOGV("offsetOfVmExt: %d", offsetOfVmExt);
    if (offsetOfVmExt < 0) {
        return -1;
    }

    int startOffset = offsetOfVmExt;
    if (isAndroidS) {
        startOffset += 200;
    }

    LOGV("targetSdkVersion: %d", targetSdkVersion);
    int targetSdkVersionOffset = findOffset(runtime, startOffset, MAX, targetSdkVersion);
    LOGV("target: %d", targetSdkVersionOffset);

    if (targetSdkVersionOffset < 0) {
        return -2;
    }

    int offsetOfMonitorNs = findOffset(runtime, targetSdkVersionOffset, MAX, (uint64_t) 500000000);
    LOGV("offsetOfMonitorNs: %d", offsetOfMonitorNs);

    if (isAndroidS) {
        auto *partialRuntime = reinterpret_cast<PartialRuntimeS *>((char *) runtime + offsetOfMonitorNs);
        unseal0<PartialRuntimeS>(partialRuntime);
    }

    return 0;
}
