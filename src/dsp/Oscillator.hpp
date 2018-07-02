#pragma once

#include "DSPMath.hpp"
#include "DSPSystem.hpp"


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
        static constexpr float BLIT_HARMONICS = 21000.f;
        static constexpr float NOTE_C4 = 261.626f;

        enum Inputs {
            VOCT1, VOCT2,
            FM_CV,
            PW_CV
        };

        enum Outputs {
            SAW,
            PULSE,
            SINE,
            TRI,
            NOISE,
            MIX
        };

        enum Params {
            FREQUENCY,
            PULSEWIDTH
        };

    private:
        float freq;      // oscillator frequency
        float pw;        // pulse-width value
        float phase;     // current phase
        float incr;      // current phase increment for PLL
        float detune;    // analogue detune
        float drift;     // oscillator drift
        float warmup;    // oscillator warmup detune
        Noise noise;     // randomizer

        float shape;
        int n;

        /* currents of waveforms
        float ramp;
        float saw;
        float pulse;
        float sine;
        float tri;*/

        void reset();

        /* saved frequency states */
        float _cv, _oct, _base, _coeff, _tune, _biqufm;


    public:
        DSPBLOscillator(float sr);


        void invalidate() override;
        void process() override;
    };


/**
 * @brief Oscillator base class
 */
    struct BLITOscillator {

    public:
        enum SIGNAL {
            SAW,
            PULSE,
            SINE,
            TRI
        };


        float freq;      // oscillator frequency
        float pw;        // pulse-width value
        float phase;     // current phase
        float incr;      // current phase increment for PLL
        float detune;    // analogue detune
        float drift;     // oscillator drift
        float warmup;    // oscillator warmup detune
        Noise noise;     // randomizer

        float shape;
        int n;

        /* currents of waveforms */
        float ramp;
        float saw;
        float pulse;
        float sine;
        float tri;

        /* saved frequency states */
        float _cv, _oct, _base, _coeff, _tune, _biqufm;

        /* leaky integrators */
        Integrator int1;
        Integrator int2;
        Integrator int3;

        BLITOscillator();
        ~BLITOscillator();

        /**
         * @brief Proccess next sample for output
         */
        void proccess();


        /**
         * @brief ReCompute states on change
         */
        void invalidate();


        /**
         * @brief Reset oscillator
         */
        void reset();


        void updatePitch(float cv, float fm, float tune, float oct);

        /* common getter and setter */
        float getFrequency() const;
        void setFrequency(float freq);
        float getPulseWidth() const;
        void setPulseWidth(float pw);

        float getRampWave() const;
        float getSawWave() const;
        float getPulseWave() const;
        float getSawTriWave() const;
        float getTriangleWave() const;
        float getSaturate() const;
        void setShape(float saturate);
    };

}