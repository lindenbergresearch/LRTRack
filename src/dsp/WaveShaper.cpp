
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







































