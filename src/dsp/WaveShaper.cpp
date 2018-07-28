
#include "WaveShaper.hpp"

using namespace dsp;


/*
void WaveShaper::process() {
    float x = input[IN].value;
    float g = param[GAIN].value;
    float b = param[BIAS].value;
    float kpos = param[KPOS].value;
    float kneg = param[KNEG].value;

    rs.doUpsample(IN, x);

    for (int i = 0; i < rs.getFactor(); i++) {
        x = rs.getUpsampled(IN)[i] + b;

        /* float y = fastSin(x / g) * g;

         y = y / sqrt(s + y * y);


         y = fastatan(y);

         y = y * g;

        // for (int j = 0; j < 5; j++) {
        if (x >= 0)
            x = 1 / atan(kpos) * atan(kpos * x);
        else
            x = 1 / atan(kneg) * atan(kneg * x);

        //     if (j % 2 == 0) x *= -1;
        //   }

        rs.data[IN][i] = x * g;
    }


    output[OUT].value = rs.getDownsampled(IN);
}*/

template<int OVERSAMPLE>

float WaveShaper<OVERSAMPLE>::getIn() const {
    return in;
}


template<int OVERSAMPLE>

void WaveShaper<OVERSAMPLE>::setIn(float in) {
    WaveShaper::in = in;
}


template<int OVERSAMPLE>

float WaveShaper<OVERSAMPLE>::getGain() const {
    return gain;
}


template<int OVERSAMPLE>

void WaveShaper<OVERSAMPLE>::setGain(float gain) {
    WaveShaper::gain = gain;
}


template<int OVERSAMPLE>

float WaveShaper<OVERSAMPLE>::getBias() const {
    return bias;
}


template<int OVERSAMPLE>

void WaveShaper<OVERSAMPLE>::setBias(float bias) {
    WaveShaper::bias = bias;
}


template<int OVERSAMPLE>

float WaveShaper<OVERSAMPLE>::getK() const {
    return k;
}


template<int OVERSAMPLE>

void WaveShaper<OVERSAMPLE>::setK(float k) {
    WaveShaper::k = k;
}


template<int OVERSAMPLE>

float WaveShaper<OVERSAMPLE>::getOut() const {
    return out;
}


template<int OVERSAMPLE>
void WaveShaper<OVERSAMPLE>::setOut(float out) {
    WaveShaper::out = out;
}


template<int OVERSAMPLE>
const Vec &WaveShaper<OVERSAMPLE>::getAmplitude() const {
    return amp;
}


template<int OVERSAMPLE>
void WaveShaper<OVERSAMPLE>::process() {

    /* if no oversampling enabled */
    if (OVERSAMPLE == 1) {
        return compute(in);
    }

    rs.doUpsample(STD_CHANNEL, in);

    for (int i = 0; i < OVERSAMPLE; i++) {
        float x = rs.getUpsampled(STD_CHANNEL)[i];
        rs.data[STD_CHANNEL][i] = compute(x);
    }

    return rs.getDownsampled(STD_CHANNEL);
}


void LockhartWavefolder::init() {
    DSPEffect::init();
}


void LockhartWavefolder::invalidate() {
    DSPEffect::invalidate();
}


void LockhartWavefolder::process() {
    DSPEffect::process();
}


float LockhartWavefolder::compute(float x) {
    // DUMMY FOR NOW
    return x;
}
