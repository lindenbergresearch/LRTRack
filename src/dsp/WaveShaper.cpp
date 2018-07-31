
#include "WaveShaper.hpp"

using namespace dsp;


float WaveShaper::getIn() const {
    return in;
}


void WaveShaper::setIn(float in) {
    WaveShaper::in = in;
}


float WaveShaper::getGain() const {
    return gain;
}


void WaveShaper::setGain(float gain) {
    WaveShaper::gain = gain;
}


float WaveShaper::getBias() const {
    return bias;
}


void WaveShaper::setBias(float bias) {
    WaveShaper::bias = clamp(bias, -MAX_BIAS_LEVEL, MAX_BIAS_LEVEL);
}


float WaveShaper::getK() const {
    return k;
}


void WaveShaper::setK(float k) {
    WaveShaper::k = k;
}


float WaveShaper::getOut() const {
    return out;
}


void WaveShaper::setOut(float out) {
    WaveShaper::out = out;
}


const Vec &WaveShaper::getAmplitude() const {
    return amp;
}


void WaveShaper::process() {

    /* if no oversampling enabled */
    if (OVERSAMPLE == 1) {
        out = compute(in * gain);
    }

    rs->doUpsample(STD_CHANNEL, in * gain);

    for (int i = 0; i < OVERSAMPLE; i++) {
        float x = rs->getUpsampled(STD_CHANNEL)[i];
        rs->data[STD_CHANNEL][i] = compute(x);
    }

    out = rs->getDownsampled(STD_CHANNEL);
}


WaveShaper::WaveShaper(float sr) : DSPEffect(sr) {}


void LockhartWavefolder::init() {
    rs = new Resampler<1>(1);
}


void LockhartWavefolder::invalidate() {
}


void LockhartWavefolder::process() {
}


float LockhartWavefolder::compute(float x) {
    float out;
    float a, b, d, l, u, ln, fn;

    a = 2 * LOCKHART_RL / LOCKHART_R;
    b = (LOCKHART_R + 2 * LOCKHART_RL) / (LOCKHART_VT * LOCKHART_R);
    d = (LOCKHART_RL * LOCKHART_Is) / LOCKHART_VT;

    // Compute Antiderivative
    l = sign(x);
    u = d * pow((float) M_E, l * b * x);
    ln = lambert_W(u, ln1);
    fn = (0.5f * LOCKHART_VT / b) * (ln * (ln + 2.f)) - 0.5f * a * x * x;

    // Check for ill-conditioning
    if (abs(x - xn1) < LOCKHART_THRESHOLD) {

        // Compute Averaged Wavefolder Output
        float xn = 0.5f * (x + xn1);
        u = d * powf((float) M_E, l * b * xn);
        ln = lambert_W(u, ln1);
        out = l * LOCKHART_VT * ln - a * xn;

    } else {

        // Apply AA Form
        out = (fn - fn1) / (x - xn1);

    }

    ln1 = ln;
    fn1 = fn;
    xn1 = x;

    return x;
}


LockhartWavefolder::LockhartWavefolder(float sr) : WaveShaper(sr) {}
