#pragma once

#include "DSPMath.hpp"
#include "DSPSystem.hpp"

#define LFO_SCALE 25.f
#define TUNE_SCALE 17.3f
#define LFO_MODE -4
#define CV_BOUNDS 10.f

namespace dsp {

    /**
     * DSP model of a leaky integrator
     */
    struct DSPIntegrator : DSPSystem<1, 1, 1> {
        enum Inputs {
            IN,
            FN
        };

        enum Outputs {
            OUT
        };

        enum Params {
            D
        };

    private:
        float value;

    public:
        void process() override {
            value = (input[IN].value - value) * (param[D].value * input[FN].value) + value;
            output[OUT].value = value;
        }


        void setInputs(float x, float fn) {
            input[IN].value = x;
            input[FN].value = fn;

            invalidate();
        }
    };


    struct DSPBLOscillator : DSPSystem<4, 6, 10> {
        /**
         * Bandwidth-limited threshold in hz.
         * Should be at least SR/2 !
         * */
        static constexpr float BLIT_HARMONICS = 18000.f;
        static constexpr float NOTE_C4 = 261.626f;

        enum Inputs {
            VOCT1, VOCT2,
            FM_CV,
            TUNE,
            OCTAVE
        };

        enum Outputs {
            SAW,
            PULSE,
            SINE,
            TRI,
            NOISE
        };

        enum Params {
            FREQUENCY,
            PULSEWIDTH
        };

    private:
        float phase;     // current phase
        float incr;      // current phase increment for PLL
        float detune;    // analogue detune
        float drift;     // oscillator drift
        float warmup;    // oscillator warmup detune
        int n;
        bool lfoMode;    // LFO mode?
        Noise noise;     // randomizer

        Integrator int1;
        Integrator int2;
        Integrator int3;


        void reset();

        /* saved frequency states */
        float _cv, _oct, _base, _coeff, _tune, _biqufm;


    public:
        explicit DSPBLOscillator(float sr);

        void updatePitch();

        void setFrequency(float frq);


        void setInputs(float voct1, float voct2, float fm, float tune, float oct);

        float getFrequency() { return param[FREQUENCY].value; }

        bool isLFO() {
            return lfoMode;
        }


        void setPulseWidth(float width);


        float getSawWave() {
            return getOutput(SAW);
        }


        float getPulseWave() {
            return getOutput(PULSE);
        }


        float getSineWave() {
            return getOutput(SINE);
        }


        float getTriWave() {
            return getOutput(TRI);
        }


        float getSuperWave() {
            return getOutput(NOISE);
        }


        void invalidate() override;
        void process() override;
    };



}