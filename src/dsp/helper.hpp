#pragma once

#include <cmath>
#include "rack.hpp"

using namespace rack;

const static float TWOPI = (float) M_PI * 2;
const static float BLIT_HARMONICS = 18000.f;


/**
 * @brief Basic integrator
 */
struct Integrator {
    float value = 0.f;

    float add(float in, float Fn);
};


/**
 * @brief Filter out DC offset
 */
struct DCBlocker {
    const float R = 0.995;
    float xm1 = 0.f, ym1 = 0.f;

    float filter(float sample);
};


float wrapTWOPI(float n);

float getPhaseIncrement(float frq);

float clipl(float in, float clip);

float cliph(float in, float clip);

float fastSinWrap(float angle);

float fastSin(float angle);

float BLIT(float N, float phase);