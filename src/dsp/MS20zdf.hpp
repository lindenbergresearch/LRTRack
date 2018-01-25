#pragma once


#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"

namespace rack {

    struct TPT {
        float y0, y;


        float getY(float x, float g) {
            float gx = g * x;
            y0 = y;

            y = gx + y0 + gx;

            return y0;
        }
    };


    struct ZDF {
        float y0, y;
        float s;
        TPT tpt;


        void compute(float x, float g) {
            y0 = y;
            s = tpt.getY(x - y0, g);
            y = g * x + s;
        }

    };


    struct MS20zdf : DSPEffect {
    private:
        // cutoff frequency and peak
        float frq, peak;


    public:

    };


}