#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <stdlib.h>

#define LOG_TAG "NightVisionJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define OBJECT_DETECT_BLOCK_SIZE_X 8
#define OBJECT_DETECT_BLOCK_SIZE_Y 8
#define OBJECT_DETECT_BLOCK_AREA (OBJECT_DETECT_BLOCK_SIZE_X * \
        OBJECT_DETECT_BLOCK_SIZE_Y)

// The minimum number of edge pi