#ifndef FREEREFLECTION_ART_H
#define FREEREFLECTION_ART_H

#include <jni.h>
#include <string>

struct JavaVMExt {
    void *functions;
    void *runtime;
};

// Refer: https://android.googlesource.com/platform/art/+/master/runtime/experimental_flags.h
struct ExperimentalFlags {
    uint32_t value;
};

// Refer: https://android.googlesource.com/platform/art/+/master/runtime/hidden_api.h
// Hidden API enforcement policy
// This must be kept in sync with ApplicationInfo.ApiEnforcementPolicy in
// frameworks/base/core/java/android/content/pm/ApplicationInfo.java
enum class EnforcementPolicy {
    kNoChecks = 0,
    kJustWarn = 1,  // keep checks enabled, but allow everything (enables logging)
    kDarkGreyAndBlackList = 2,  // ban dark grey & blacklist
    kBlacklistOnly = 3,  // ban blacklist violations only
    kMax = 3,
};

enum class RuntimeDebugState {
// This doesn't support any debug features / method tracing. This is the expected state usually.
    kNonJavaDebuggable,
// This supports method tracing and a restricted set of debug features (for ex: redefinition
// isn't supported). We transition to this state when method tracing has started or when the
// debugger was attached and transition back to NonDebuggable once the tracing has stopped /
// the debugger agent has detached..
    kJavaDebuggable,
// The runtime was started as a debuggable runtime. This allows us to support the extended set
// of debug features (for ex: redefinition). We never transition out of this state.
    kJavaDebuggableAtInit
};

// Android 12: http://aospxref.com/android-12.0.0_r3/xref/art/runtime/runtime.h
struct PartialRuntimeS {
    // Specifies target SDK version to allow workarounds for certain API levels.
    uint64_t monitor_timeout_ns_;
    bool is_profileable_from_shell_ = false;
    bool is_profileable_ = false;
    uint32_t zygote_max_failed_boots_;
    ExperimentalFlags experimental_flags_;
    std::string fingerprint_;
    void *oat_file_manager_;
    bool is_low_memory_mode_;
    bool madvise_random_access_;
    size_t madvise_willneed_total_dex_size_;
    size_t madvise_willneed_odex_filesize_;
    size_t madvise_willneed_art_filesize_;
    bool safe_mode_;

    // Whether access checks on hidden API should be performed.
    EnforcementPolicy hidden_api_policy_;
};


int unseal(JNIEnv *env, jint targetSdkVersion);

#endif //FREEREFLECTION_ART_H