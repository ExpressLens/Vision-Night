#include "nightvision.h"
#include <stdbool.h>

/* Private: Convert a single ARGB pixel to grayscale (i.e. 0 - 255). */
uint8_t rgbToGrayscale(argb* pixel) {
    return 0.3 * pixel->red + 0.59 * pixel->green + 0.11 * pixel->blue;
}

jboolean Java_com_ford_openxc_nightvision_NightVisionView_detectObjects(
        JNIEnv* 