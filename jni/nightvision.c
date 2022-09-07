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

    uint8_t* overlayPixels;
    if((result = AndroidBitmap_lockPixels(env, overlayBitmap,
                    (void*)&overlayPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed, error=%d", result);
        AndroidBitmap_unlockPixels(env, edgeBitmap);
        return false;
    }

    bool objectDetected = false;
    for(int y = edgeInfo.height * DETECTION_WINDOW_SIZE;
            y < edgeInfo.height * (1 - DETECTION_WINDOW_SIZE);
            y += (OBJECT_DETECT_BLOCK_SIZE_Y / 2)) {

        uint8_t* edgeLine = edgePixels + edgeInfo.stride * y;
        uint8_t* overlayLine = overlayPixels + overlayInfo.stride * y;

        for(int x = edgeInfo.width * DETECTION_WINDOW_SIZE;
                x < edgeInfo.width * (1 - DETECTION_WINDOW_SIZE);
                x += (OBJECT_DETECT_BLOCK_SIZE_X / 2)) {

            int gradient = 0;
            for(int i = 0; i < OBJECT_DETECT_BLOCK_SIZE_X; i++) {
                for(int j = 0; j < OBJECT_DETECT_BLOCK_SIZE_Y; j++) {
                    uint8_t pixel = *(edgeLine + x + i + j * edgeInfo.stride);
                    if(pixel == 255) {
                        gradient++;
                    }
                }
            }

            if (gradient > OBJECT_EDGE_THRESHOLD) {
                for(int i = 0; i < OBJECT_DETECT_BLOCK_SIZE_X; i++) {
                    for(int j = 0; j < OBJECT_DETECT_BLOCK_SIZE_Y; j++) {
                        *(overlayLine + x + i + j * overlayInfo.stride) = 255;
                    }
                }
                objectDetected = true;
            }
        }
    }

    AndroidBitmap_unlockPixels(env, edgeBitmap);
    AndroidBitmap_unlockPixels(env, overlayBitmap);

    return objectDetected;
}

void Java_com_ford_openxc_nightvision_NightVisionView_detectEdges(JNIEnv* env,
        jobject thiz, jobject imageBitmap, jobject edgeBitmap) {
    uint8_t* imagePixels;
    int ret;
    if((ret = AndroidBitmap_lockPixels(env, imageBitmap,
                    (void*)&imagePixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed, error=%d", ret);
        return;
    }

    AndroidBitmapInfo imageInfo;
    if((ret = AndroidBitmap_getInfo(env, imageBitmap, &imageInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed, error=%d", ret);
        return;
    }

    uint8_t* edgePixels;
    if((ret = AndroidBitmap_lockPixels(env, edgeBitmap,
                    (void*)&edgePixels)) < 0) {
    