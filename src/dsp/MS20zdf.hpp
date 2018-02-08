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
    struct MS20ZDF : DSPSystem2x2 {
        float y;
        float s;
        MS20TPT tpt;


        void process() override {
            y = input[IN1].value * input[IN2].value + s;

            tpt.set(input[IN1].value - y, input[IN2].value);
            s = tpt.s;
        }

    };


    /**
     * @brief High quality oversampled drive using arctan

    template<int OVERSAMPLE>
    struct HQOverdrive : DSPSystem1x1<2> {
        enum Params {
            LEVEL,
            GAIN
        };

    private:
        OverSampler<OVERSAMPLE, 1> os;

    public:

        HQOverdrive(float level, float gain) {
            setLevel(level);
            setGain(gain);
        }


        void setLevel(float value) {
            param[LEVEL].value = value;
        }


        void setGain(float value) {
            param[GAIN].value = value;
        }


        void process() override {
            float level = getParam(LEVEL);
            float gain = getParam(GAIN);
            float x = input[IN].value;

            os.next(IN, x);
            os.doUpsample(IN);

            for (int i = 0; i < os.factor; i++) {
                os.data[IN][i] = atanf(gain * os.up[IN][i] / level) * level;
            }

            output[OUT].value = os.getDownsampled(IN);
        }

    };*/


    /**
     * @brief MS20 Filter class
     */
    struct MS20zdf : DSPSystem<1, 2, 4> {
        static const int OVERSAMPLE = 8;                // factor of internal oversampling
        static constexpr float DRIVE_GAIN = 20.f;       // max drive gain

        enum Inputs {
            IN
        };

        enum Params {
            FREQUENCY,
            PEAK,
            DRIVE,
            TYPE
        };

        enum Outputs {
            OUT,
        };

    private:
        float g, g2, b, k;
        float ky, y;
        float freqHz;

        MS20ZDF zdf1, zdf2;
        OverSampler<OVERSAMPLE, 1> os;

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


        void setDrive(float value) {
            setParam(DRIVE, value);
        }


        float getDrive() {
            return getParam(DRIVE);
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


        float getLPOut() {
            return getOutput(OUT);
        }


        float getType() {
            return getParam(TYPE);
        }


        void setType(float value) {
            setParam(TYPE, value);
        }

        void invalidate() override;
        void process() override;
    };


}