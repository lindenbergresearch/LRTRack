#pragma once

#include <cmath>
#include <random>
#include "rack.hpp"

using namespace rack;

const static float TWOPI = (float) M_PI * 2;

/* standard precision */
typedef double sfloat;


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
    sfloat RC;
    sfloat dt;
    sfloat alpha;
    sfloat y0;
    sfloat fc;

public:

    /**
     * @brief Create a new filter with a given cutoff frequency
     * @param fc cutoff frequency
     * @param factor Oversampling factor
     */
    LP6DBFilter(sfloat fc, int factor) {
        updateFrequency(fc, factor);
        y0 = 0.f;
    }


    /**
     * @brief Set new cutoff frequency
     * @param fc cutoff frequency
     */
    void updateFrequency(sfloat fc, int factor);

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
struct Oversampler {
    enum SampleFactor {
        OVERSAMPLE_2X = 2,
        OVERSAMPLE_4X = 4,
        OVERSAMPLE_8X = 8,
        OVERSAMPLE_16X = 16,
        OVERSAMPLE_32X = 32
    };

    sfloat y0, y1;
    sfloat up[OVERSAMPLE_32X];
    sfloat data[OVERSAMPLE_32X];
    SampleFactor factor;
    LP6DBFilter filter = LP6DBFilter(18000);


    Oversampler(SampleFactor factor) : factor(factor) {
        y0 = 0;
        y1 = 0;
    }


    /**
     * @brief Return linear interpolated position
     * @param point Point in oversampled data
     * @return
     */
    sfloat interpolate(int point);


    /**
     * @brief Create up-sampled data out of two basic values
     */
    void upsample();

    /**
     * @brief Downsample data
     * @return Downsampled point
     */
    sfloat downsample();

    /**
     * @brief Compute data for oversampled points
     * @param transform Pointer to transform function
     */
    void compute(sfloat (*transform)(sfloat));

    /**
     * @brief Step to next sample point
     * @param y Next sample point
     */
    void next(sfloat y);
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
