#include "MS20zdf.hpp"


/**
 * @brief Calculate prewarped vars on parameter change
 */
void dsp::MS20zdf::invalidate() {
    // translate frequency to logarithmic scale
    freqHz = 8.f * powf(2500.f, frequency);

    b = tanf(freqHz * (float) M_PI / engineGetSampleRate() * OVERSAMPLE);
    g = b / (1 + b);
    k = 2 * peak;
    g2 = g * g;
}


/**
 * @brief Proccess one sample of filter
 */
void dsp::MS20zdf::process() {
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
 * @brief Inherit constructor
 * @param sr sample rate
 */
dsp::MS20zdf::MS20zdf(float sr) : DSPSystem(sr) {}

