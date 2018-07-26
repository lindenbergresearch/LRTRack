#pragma once

#include "DSPSystem.hpp"
#include "DSPMath.hpp"
#include "DSPEffect.hpp"


namespace dsp {


    struct LockhartWavefolder : DSPEffect {
        float gain;


        void init() override;
        void invalidate() override;
        void process() override;

    };


    struct WaveShaper : DSPSystem<1, 1, 4> {

        static const int OVERSAMPLE = 8;

        enum Inputs {
            IN
        };

        enum Params {
            GAIN,
            KPOS,
            KNEG,
            BIAS
        };

        enum Outputs {
            OUT
        };


    private:

        Resampler<OVERSAMPLE, 1> rs;

    public:

        explicit WaveShaper(float sr) {}


        void setGain(float gain) {
            setParam(GAIN, gain, true);
        }


        void setBias(float bias) {
            setParam(BIAS, bias);
        }


        void setShape(float kpos, float kneg) {
            setParam(KPOS, kpos);
            setParam(KNEG, kneg);
        }


        float getGain() {
            return getParam(GAIN);
        }


        void setIn(float in) {
            setInput(IN, in);
        }


        float getOut() {
            return getOutput(OUT);
        }


        void invalidate() override;


        void process() override;


    };


}

