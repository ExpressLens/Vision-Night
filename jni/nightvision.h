#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <stdlib.h>

#define LOG_TAG "NightVisionJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__