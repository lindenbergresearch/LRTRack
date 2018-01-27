#include "MS20zdf.hpp"


/**
 * @brief Calculate prewarped vars on parameter change
 */
void dsp::MS20zdf::invalidate() {
    // translate frequency to logarithmic scale
    freqHz = 8.f * powf(2500.f, param[FREQUENCY].value);

    b = tanf(freqHz * (float) M_PI / engineGetSampleRate() * OVERSAMPLE);
    g = b / (1 + b);
    k = 2 * param[PEAK].value;
    g2 = g * g;
}


/**
 * @brief Proccess one sample of filter
 */
void dsp::MS20zdf::process() {
    float s1, s2;

    zdf1.compute(input[IN].value - ky, g);
    s1 = zdf1.s;

    zdf2.compute(zdf1.y + ky, g);
    s2 = zdf2.s;

    y = 1 / (g2 * k - g * k + 1) * (g2 * input[IN].value + g * s1 + s2);

    ky = k * y;

    output[LOWPASS].value = y;
    output[HIGHPASS].value = input[IN].value - y;
}


/**
 * @brief Inherit constructor
 * @param sr sample rate
 */
dsp::MS20zdf::MS20zdf(float sr) : DSPSystem(sr) {}

