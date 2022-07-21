#include "nightvision.h"
#include <stdbool.h>

/* Private: Convert a single ARGB pixel to grayscale (i.e. 0 - 255). */
uint8_t rgbToGrayscale(argb* pixel) {
    return 0.3 * pixel->red + 0.59 * pixel->green + 0.11 * pixel->blue;
}

jboolean Java_com_ford_openxc_nightvision_NightVisionView_detectObjects(
        JNIEnv* env, jobject thiz, jobject edgeBitmap, jobject overlayBitmap) {
    AndroidBitmapInfo edgeInfo;
    int result;
    if((result = AndroidBitmap_getInfo(env, edgeBitmap, &edgeInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed, error=%d", result);
        return false;
    }

    AndroidBitmapInfo overlayInfo;
    if((result = AndroidBitmap_getInfo(env, overlayBitmap, &overlayInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed, error=%d", result);
        return false;
    }

    uint8_t* edgePixels;
    if((result = AndroidBitmap_lockPixels(env, edgeBitmap,
                    (void*)&edgePixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed, error=%d", result);
        return false;
    }

    uint8_t* o