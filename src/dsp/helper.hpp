#pragma once

#include <cmath>
#include "rack.hpp"

using namespace rack;

const float TWOPI = (float)M_PI * 2;

/**
 * @brief Basic integrator
 */
struct Integrate {
    float value = 0.f;

    float add(float in, float Fn);
};

float wrapTWOPI(float n);

float getPhaseIncrement(float frq);

float clipl(float in, float clip);

float cliph(float in, float clip);

float fastSinWrap(float angle);

float fastSin(float angle);

float BLIT(float N, float phase);