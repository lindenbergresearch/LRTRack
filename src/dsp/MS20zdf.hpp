#pragma once


#include "DSPSystem.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"
#include "DSPEffect.hpp"

namespace dsp {

    /**
     * @brief MS20 Topology Preserving Transform
     */
    struct TPT {
        float s;


        /**
         * @brief Get current Y value
         * @param x
         * @param g
         * @return
         */
        void compute(float x, float g) {
            float gx = g * x;

            s = gx + s + gx;
        }
    };


    /**
     * @brief Zero Delay Feedback
     */
    struct ZDF {
        float y;
        float s;
        TPT tpt;


        /**
         * @brief Compute ZDF
         * @param x
         * @param g
         */
        void compute(float x, float g) {
            tpt.compute(x - y, g);
            s = tpt.s;
            y = g * x + s;
        }

    };


    /**
     * @brief MS20 Filter class
     */
    struct MS20zdf : DSPSystem<1, 2, 2> {
        static const int OVERSAMPLE = 1;    // factor of internal oversampling

        enum Inputs {
            IN
        };

        enum Params {
            FREQUENCY,
            PEAK
        };

        enum Outputs {
            LOWPASS,
            HIGHPASS
        };

    private:
        float g, g2, b, k;
        float ky, y;
        float freqHz;

        ZDF zdf1, zdf2;

    public:
        MS20zdf(float sr);


        float getFrequency() const {
            return param[FREQUENCY].value;
        }


        void setFrequency(float frequency) {
            param[FREQUENCY].value = frequency;
        }


        float getPeak() {
            return param[PEAK].value;
        }


        void setPeak(float value) {
            param[PEAK].value = value;
        }


        void setIn(float value) {
            input[IN].value = value;
        }


        float getHPOut() {
            return output[HIGHPASS].value;
        }


        float getLPOut() {
            return output[LOWPASS].value;
        }


        void invalidate() override;
        void process() override;
    };


}