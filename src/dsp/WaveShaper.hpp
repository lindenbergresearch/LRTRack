#pragma once

#include "DSPMath.hpp"
#include "DSPEffect.hpp"

namespace dsp {

    /**
     * @brief Basic WaveShaper class with build-in dynamic oversampling
     * @tparam OVERSAMPLE
     */
    struct WaveShaper : DSPEffect {
        /* oversampling channel */
        static const int STD_CHANNEL = 0;
        static constexpr float MAX_BIAS_LEVEL = 5.0; // +/- 5V

    protected:
        Resampler<1> *rs;

        float in, gain, bias, k;
        float out;
        Vec amp;

    public:

        WaveShaper(float sr);

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


        /**
         * @brief Returns the actual sample-rate which is used by oversampled computation
         * @return
         */
        float getOversampledRate() {
            return sr * rs->getFactor();
        }


        void setAmplitude(float kpos, float kneg) {
            amp = Vec(kpos, kneg);
        }


        const Vec &getAmplitude() const;


        /**
         * @brief Implements the oversamping of compute method
         */
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
         * @param x Input sample
         * @return Output sample
         */
        virtual double compute(double x) { return x; }
    };


}

