#include "MS20zdf.hpp"

using namespace dsp;


/**
 * @brief Calculate prewarped vars on parameter change
 */
void MS20zdf::invalidate() {
    // translate frequency to logarithmic scale
    freqHz = 20.f * powf(860.f, param[FREQUENCY].value) - 20.f;

    b = tanf(freqHz * (float) M_PI / sr / OVERSAMPLE);
    g = b / (1 + b);
    k = 2 * param[PEAK].value * 1.0f;
    g2 = g * g;
}


/**
 * @brief Proccess one sample of filter
 */
void MS20zdf::process() {
    os.next(IN, input[IN].value);
    os.doUpsample(IN);

    float s1, s2;
    float gain = quadraticBipolar(param[DRIVE].value) * DRIVE_GAIN + 1.f;

    for (int i = 0; i < os.factor; i++) {
        float x = os.up[IN][i];

        zdf1.set(x - ky, g);
        s1 = zdf1.s;

        zdf2.set(zdf1.y + ky, g);
        s2 = zdf2.s;

        y = 1.f / (g2 * k - g * k + 1.f) * (g2 * x + g * s1 + s2);

        ky = k * atanf(y / 130.f) * 130.f;

        os.data[IN][i] = atanf(gain * y / 10.f) * 10.f;
    }

    float out = os.getDownsampled(IN);

    output[LOWPASS].value = out;
    output[HIGHPASS].value = out - input[IN].value;
}


/**
 * @brief Inherit constructor
 * @param sr sample rate
 */
MS20zdf::MS20zdf(float sr) : DSPSystem(sr) {}

