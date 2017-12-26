#pragma once

#include <cmath>
#include <random>
#include "rack.hpp"
#include "dsp/decimator.hpp"

using namespace rack;

const static float TWOPI = (float) M_PI * 2;


/**
 * @brief Basic leaky integrator
 */
struct Integrator {
    const float d = 0.25f;
    float value = 0.f;

    /**
     * @brief Add value to integrator
     * @param x Input sample
     * @param Fn
     * @return Current integrator state
     */
    float add(float x, float Fn);
};


/**
 * @brief Filter out DC offset / 1-Pole HP Filter
 */
struct DCBlocker {
    const float R = 0.999;
    float xm1 = 0.f, ym1 = 0.f;

    /**
     * @brief Filter signal
     * @param x Input sample
     * @return Filtered output
     */
    float filter(float x);
};


/**
 * @brief Simple 6dB lowpass filter
 */
struct LP6DBFilter {
private:
    float RC;
    float dt;
    float alpha;
    float y0;
    float fc;

public:

    /**
     * @brief Create a new filter with a given cutoff frequency
     * @param fc cutoff frequency
     * @param factor Oversampling factor
     */
    LP6DBFilter(float fc, int factor) {
        updateFrequency(fc, factor);
        y0 = 0.f;
    }


    /**
     * @brief Set new cutoff frequency
     * @param fc cutoff frequency
     */
    void updateFrequency(float fc, int factor);

    /**
     * @brief Filter signal
     * @param x Input sample
     * @return Filtered output
     */
    double filter(double x);
};


/**
 * @brief Simple randomizer
 */
struct Randomizer {

    Randomizer();

    float nextFloat(float start, float stop);
};


/**
 * @brief Simple oversampling class
 */
template<int OVERSAMPLE, int CHANNELS>
struct Oversampler {
    float y0, y1;
    float up[OVERSAMPLE];
    float data[CHANNELS][OVERSAMPLE];
    Decimator<OVERSAMPLE, OVERSAMPLE> decimator;
    int factor = OVERSAMPLE;


    /**
     * @brief Constructor
     * @param factor Oversampling factor
     */
    Oversampler() {
        y0 = 0;
        y1 = 0;
    }


    /**
     * @brief Return linear interpolated position
     * @param point Point in oversampled data
     * @return
     */
    float interpolate(int point) {
        return y0 + (point / factor) * (y1 - y0);
    }


    /**
     * @brief Create up-sampled data out of two basic values
     */
    void doUpsample() {
        for (int i = 0; i < factor; i++) {
            up[i] = interpolate(i + 1);
        }
    }


    /**
     * @brief Downsample data from agiven channel
     * @param channel Channel to proccess
     * @return Downsampled point
     */
    float getDownsampled(int channel) {
        return decimator.process(data[channel]);
    }

    /**
     * @brief Step to next sample point
     * @param y Next sample point
     */
    void doNext(float y) {
        y0 = y1;
        y1 = y;
    }
};


float wrapTWOPI(float n);

float getPhaseIncrement(float frq);

float clipl(float in, float clip);

float cliph(float in, float clip);

float fastSinWrap(float angle);

float fastSin(float angle);

float qsinhp(float x);

float qsinlp(float x);

float BLIT(float N, float phase);

float shape1(float a, float x);

double saturate(double x, double a);

double overdrive(double input);


/**
 * @brief Double version of clamp
 * @param x
 * @param min
 * @param max
 * @return
 */
inline double clampd(double x, double min, double max) {
    return fmax(fmin(x, max), min);
}


/**
 * @brief Soft clipping
 * @param x
 * @param sat
 * @param satinv
 * @return
 */
inline double clip(double x, double sat, double satinv) {
    double v2 = (x * satinv > 1 ? 1 :
                 (x * satinv < -1 ? -1 :
                  x * satinv));
    return (sat * (v2 - (1. / 3.) * v2 * v2 * v2));
}


/**
 * @brief Clamp without branching
 * @param input Input sample
 * @return
 */
inline double saturate2(double input) { //clamp without branching
    const double _limit = 0.3;
    double x1 = fabs(input + _limit);
    double x2 = fabs(input - _limit);
    return 0.5 * (x1 - x2);
}