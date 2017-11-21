#include "helper.hpp"
#include "../LindenbergResearch.hpp"

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