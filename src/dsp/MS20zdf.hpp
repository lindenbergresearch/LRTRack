#pragma once


#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"

namespace rack {

    /**
     * @brief MS20 Topology Preserving Transform
     */
    struct TPT {
        float y0, y;


        /**
         * @brief Get current Y value
         * @param x
         * @param g
         * @return
         */
        float getY(float x, float g) {
            float gx = g * x;
            y0 = y;

            y = gx + y0 + gx;

            return y0;
        }
    };


    /**
     * @brief Zero Delay Feedback
     */
    struct ZDF {
        float y0, y;
        float s;
        TPT tpt;


        /**
         * @brief Compute ZDF
         * @param x
         * @param g
         */
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