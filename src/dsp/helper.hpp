#pragma once

#include <cmath>
#include "rack.hpp"

using namespace rack;

#define PI 3.14159265359f
#define TWOPI 6.28318530718f
#define BLIT_CUTOFF 18000.0f

float wrapTWOPI(float n);

float getPhaseIncrement(float frq);

float clipl(float in, float clip);

float cliph(float in, float clip);

float fastSinWrap(float angle);

float fastSin(float angle);