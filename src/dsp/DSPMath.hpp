#pragma once

#include <cmath>
#include <random>
#include "rack.hpp"

using namespace rack;

const static float TWOPI = (float) M_PI * 2;
const static float BLIT_HARMONICS = 18000.f;
const static float NOTE_C4 = 261.626f;
const static float OSC_SHAPING = 0.778;

/* standard precision */
typedef double sfloat;


/**
 * @brief Basic leaky integrator
 */
struct Integrator {
    const float d = 0.25f;
    float value = 0.f;

    float add(float in, float Fn);
};


/**
 * @brief Filter out DC offset
 */
struct DCBlocker {
    const float R = 0.999;
    float xm1 = 0.f, ym1 = 0.f;

    float filter(float sample);
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

    LP6DBFilter(sfloat fc) {
        updateFrequency(fc);
        y0 = 0.f;
    }

    void updateFrequency(sfloat fc);

    double filter(double x);
};


/**
 * @brief Simple randomizer
 */
struct Randomizer {

    Randomizer();

    float nextFloat(float start, float stop);
};


struct Oversampler {
private:


public:

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
