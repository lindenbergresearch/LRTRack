#include "helper.hpp"
#include "engine.hpp"

/**
 * Clip signal at bottom by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float clipl(float in, float clip) {
    if (in < clip) return clip;
    else return in;
}

/**
 * Clip signal at top by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float cliph(float in, float clip) {
    if (in > clip) return clip;
    else return in;
}

/**
 * Wrap input number between -PI..PI
 * @param n Input number
 * @return Wrapped value
 */
float wrapTWOPI(float n) {
    float b = 1.f / TWOPI * n;
    return (b - lround(b)) * TWOPI;
}

/**
 * Get PLL increment depending on frequency
 * @param frq Frequency
 * @return  PLL increment
 */
float getPhaseIncrement(float frq) {
    return TWOPI * frq / engineGetSampleRate();
}


float BLIT(float N, float phase, float inc) {

}