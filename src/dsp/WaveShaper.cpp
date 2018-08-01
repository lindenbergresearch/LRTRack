
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
    /* if no oversampling set up */
    if (rs->getFactor() == 1) {
        out = compute(in);
        //debug("%f", out);
        return;
    }

    rs->doUpsample(STD_CHANNEL, in);

    for (int i = 0; i < rs->getFactor(); i++) {
        float x = rs->getUpsampled(STD_CHANNEL)[i];
        rs->data[STD_CHANNEL][i] = compute(x);
    }

    out = rs->getDownsampled(STD_CHANNEL);
}


WaveShaper::WaveShaper(float sr) : DSPEffect(sr) {}


void LockhartWavefolder::init() {
    rs = new Resampler<1>(1);
}


double LockhartWFStage::compute(double x) {
    double out;
    double l, u, ln, fn;

    // Compute Antiderivative
    l = sign(x);
    u = d * pow(M_E, l * b * x);
    ln = lambert_W_Fritsch(u);
    fn = (0.5 * LOCKHART_VT / b) * (ln * (ln + 2.)) - 0.5 * a * x * x;

    // Check for ill-conditioning
    if (abs(x - xn1) < LOCKHART_THRESHOLD) {
        // Compute Averaged Wavefolder Output
        double xn = 0.5 * (x + xn1);
        u = d * pow(M_E, l * b * xn);
        ln = lambert_W_Fritsch(u);
        out = l * LOCKHART_VT * ln - a * xn;

    } else {
        // Apply AA Form
        out = (fn - fn1) / (x - xn1);
    }

    ln1 = ln;
    fn1 = fn;
    xn1 = x;

    return out;
}


LockhartWFStage::LockhartWFStage() {
    ln1 = 0;
    fn1 = 0;
    xn1 = 0;

    a = 2. * LOCKHART_RL / LOCKHART_R;
    b = (LOCKHART_R + 2. * LOCKHART_RL) / (LOCKHART_VT * LOCKHART_R);
    d = (LOCKHART_RL * LOCKHART_Is) / LOCKHART_VT;
}


void LockhartWavefolder::invalidate() {
}


void LockhartWavefolder::process() {
    WaveShaper::process();
}


double LockhartWavefolder::compute(double x) {
    double out;
    double in = (x + bias) * gain;

    in = lh.compute(in * 0.7);
    in = lh.compute(in);
     in = lh.compute(in);
    in = lh.compute(in);

    in *= 3.f;

    out = tanh(in);

    return out * 30;
}


LockhartWavefolder::LockhartWavefolder(float sr) : WaveShaper(sr) {
    init();
}







































