#pragma once

#include "DSPSystem.hpp"
#include "DSPMath.hpp"
#include "DSPEffect.hpp"


namespace dsp {

    /**
     * @brief Basic WaveShaper class with build-in dynamic oversampling
     * @tparam OVERSAMPLE
     */
    template<int OVERSAMPLE>
    struct WaveShaper : DSPEffect {
        /* oversampling channel */
        static const int STD_CHANNEL = 1;

    private:
        Resampler<OVERSAMPLE, 1> rs;

        float in, gain, bias, k;
        float out;
        Vec amp;

    public:

        float getIn() const;
        void setIn(float in);
        float getGain() const;
        void setGain(float gain);
        float getBias() const;
        void setBias(float bias);
        float getK() const;
        void setK(float k);
        float getOut() const;
        void setOut(float out);


        void setAmplitude(float kpos, float kneg) {
            amp = Vec(kpos, kneg);
        }


        const Vec &getAmplitude() const;


        void process() override;


        void init() override {
            gain = 0;
            out = 0;
            k = 0;
            bias = 0;
            amp = Vec(0, 0);
        }


        /**
         * @brief To be implemented by subclass, automaticaly oversampled
         *
         * @param x
         * @return
         */
        virtual float compute(float x) { return x; }
    };


    struct LockhartWavefolder : WaveShaper<1> {


        void init() override;
        void invalidate() override;
        void process() override;

    };


}

