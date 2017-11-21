#pragma once

#include <cmath>
#include "rack.hpp"

namespace Rack {
    #define PI 3.14159265359f
    #define TWOPI 6.28318530718f

    float wrapTWOPI(float n);
    float getPhaseIncrement(float frq);
}