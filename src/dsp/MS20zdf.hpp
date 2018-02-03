#pragma once

#include "DSPSystem.hpp"
#include "DSPMath.hpp"

namespace dsp {


    /**
     * @brief
     */
    struct MS20TPT : DSPSystem2x1 {
        float s;
        DSPDelay1 z;


        void process() override {
            float gx = input[IN1].value * input[IN2].value;

            z.set(gx + z.get() + gx);
            s = z.get();
        }
    };

    /**
     * @brief Zero Delay Feedback
     */
    struct ZDF {
        float y;
        float s;
        MS20TPT tpt;


        /**
         * @brief Compute ZDF
         * @param x
         * @param g
         */
        void compute(float x, float g) {
            y = g * x + s;

            tpt.set(x - y, g);
            s = tpt.s;
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
        explicit MS20zdf(float sr);


        float getFrequency() {
            return getParam(FREQUENCY);
        }


        float getFrequencyHz() const {
            return freqHz;
        }


        void setFrequency(float value) {
            setParam(FREQUENCY, value);
        }


        float getPeak() {
            return getParam(PEAK);
        }


        void setPeak(float value) {
            setParam(PEAK, value);
        }


        void setIn(float value) {
            setInput(IN, value);
        }


        float getHPOut() {
            return getOutput(HIGHPASS);
        }


        float getLPOut() {
            return getOutput(LOWPASS);
        }


        void invalidate() override;
        void process() override;
    };


}