#include "DSPMath.hpp"
#include "engine.hpp"


/**
 * @brief Fast sin approximation
 * @param angle Angle
 * @return App. value
 */
float fastSin(float angle) {
    float sqr = angle * angle;
    float result = -2.39e-08f;
    result *= sqr;
    result += 2.7526e-06f;
    result *= sqr;
    result -= 1.98409e-04f;
    result *= sqr;
    result += 8.3333315e-03f;
    result *= sqr;
    result -= 1.666666664e-01f;
    result *= sqr;
    result += 1.0f;
    result *= angle;
    return result;
    // return sinf(angle);
}


/**
 * @brief Quadratic sin approximation low precicion
 * @param x
 * @return
 */
float qsinlp(float x) {
    if (x < -3.14159265f)
        x += 6.28318531f;
    else if (x > 3.14159265f)
        x -= 6.28318531f;

    if (x < 0)
        return x * (1.27323954f + 0.405284735f * x);
    else
        return x * (1.27323954f - 0.405284735f * x);
}


/**
 * @brief Quadratic sin approximation high precicion
 * @param x
 * @return
 */
float qsinhp(float x) {
    float sin;

    if (x < -3.14159265f)
        x += 6.28318531f;
    else if (x > 3.14159265f)
        x -= 6.28318531f;

    if (x < 0) {
        sin = x * (1.27323954f + 0.405284735f * x);

        if (sin < 0)
            sin = sin * (-0.255f * (sin + 1) + 1);
        else
            sin = sin * (0.255f * (sin - 1) + 1);
    } else {
        sin = x * (1.27323954f - 0.405284735f * x);

        if (sin < 0)
            sin = sin * (-0.255f * (sin + 1) + 1);
        else
            sin = sin * (0.255f * (sin - 1) + 1);
    }

    return sin;
}


/**
 * @brief Fast sin approximation with input wrapping of -PI..PI
 * @param angle Angle
 * @return App. value
 */
float fastSinWrap(float angle) {
    return fastSin(angle);
}


/**
 * @brief Clip signal at bottom by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float clipl(float in, float clip) {
    if (in < clip) return clip;
    else return in;
}


/**
 * @brief Clip signal at top by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float cliph(float in, float clip) {
    if (in > clip) return clip;
    else return in;
}


/**
 * @brief Wrap input number between -PI..PI
 * @param n Input number
 * @return Wrapped value
 */
float wrapTWOPI(float n) {
    float b = 1.f / TWOPI * n;
    return (b - lround(b)) * TWOPI;
}


/**
 * @brief Get PLL increment depending on frequency
 * @param frq Frequency
 * @return  PLL increment
 */
float getPhaseIncrement(float frq) {
    return TWOPI * frq / engineGetSampleRate();
}


/**
 * @brief Actual BLIT core computation
 * @param N Harmonics
 * @param phase Current phase value
 * @return
 */
float BLITcore(float N, float phase) {
    float a = wrapTWOPI((clipl(N - 1, 0.f) + 0.5f) * phase);
    float x = fastSin(a) * 1.f / fastSin(0.5f * phase);
    return (x - 1.f) * 0.5f;
}


/**
 * @brief BLIT generator based on current phase
 * @param N Harmonics
 * @param phase Current phase of PLL
 * @return
 */
float BLIT(float N, float phase) {
    if (phase == 0.f) return 1.f;
    else return BLITcore(N, phase);
}


/**
 * @brief Add value to integrator
 * @param in Input
 * @param Fn
 * @return
 */
float Integrator::add(float in, float Fn) {
    value = (in - value) * (0.25f * Fn) + value;
    return value;
}


/**
 * @brief Filter function for DC block
 * @param sample Input sample
 * @return Filtered sample
 */
float DCBlocker::filter(float sample) {
    float y = sample - xm1 + R * ym1;
    xm1 = sample;
    ym1 = y;

    return y;
}


/**
 * @brief Filter function for simple 6dB lowpass filter
 * @param x Input sample
 * @return
 */
float LP6DBFilter::filter(float x) {
    float y = y0 + (alpha * (x - y0));
    y0 = y;
    return y;
}


/**
 * @brief Init randomizer
 */
Randomizer::Randomizer() {}


/**
 * @brief Return next random float in the range of 0.0 - 1.0
 * @return Random number
 */
float Randomizer::nextFloat(float start, float stop) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(start, stop); // rage 0 - 1
    return (float) dis(e);
}