
#include "MS20zdf.hpp"


/**
 * @brief Get current filter cutoff frequency
 * @return
 */
float rack::MS20zdf::getFrequency() const {
    return frequency;
}


/**
 * @brief Set filter cutoff frequency and trigger recalculation of prewarped vars
 * @param frequency
 */
void rack::MS20zdf::setFrequency(float frequency) {
    if (MS20zdf::frequency != frequency) {
        MS20zdf::frequency = frequency;

        invalidate();
    }
}


/**
 * @brief Get filter peak (resonance)
 * @return
 */
float rack::MS20zdf::getPeak() const {
    return peak;
}


/**
 * @brief Set filter peak (resonance) and trigger recalculation of prewarped vars
 * @param peak
 */
void rack::MS20zdf::setPeak(float peak) {
    if (MS20zdf::peak != peak) {
        MS20zdf::peak != peak;

        invalidate();
    }
}


/**
 * @brief Calculate prewarped vars on parameter change
 */
void MS20zdf::invalidate() {
    b = tanf(frequency * (float) M_PI / engineGetSampleRate());
    g = b / (1 + b);
    k = 2 * peak;
    g2 = g * g;
}


/**
 * @brief Proccess one sample of filter
 */
void MS20zdf::process() {
    float s1, s2;

    zdf1.compute(in - ky, g);
    s1 = zdf1.s;

    zdf2.compute(zdf1.y + ky, g);
    s2 = zdf2.s;

    y = 1 / (g2 * k - g * k + 1) * (g2 * in + g * s1 + s2);

    ky = k * y;

    lpOut = y;
    hpOut = in - y;
}


/**
 * @brief Current Lowpass out
 * @return
 */
float MS20zdf::getLpOut() const {
    return lpOut;
}


/**
 * @brief Current Highpass out
 * @return
 */
float MS20zdf::getHpOut() const {
    return hpOut;
}


/**
 * @brief Set filter input
 * @param in
 */
void MS20zdf::setIn(float in) {
    MS20zdf::in = in;
}
