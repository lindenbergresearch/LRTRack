#pragma once

#include "DSPEffect.hpp"

namespace dsp {


    struct HQTanh : DSPEffect {

        /* oversampling channel */
        static const int STD_CHANNEL = 0;

        int factor;
        float in, out;
        Resampler<1> *rs;


        HQTanh(float sr, int factor) : DSPEffect(sr) {
            HQTanh::factor = factor;

            rs = new Resampler<1>(factor);
        }


        /**
         * @brief Returns the actual sample-rate which is used by oversampled computation
         * @return
         */
        float getOversampledRate() {
            return sr * rs->getFactor();
        }


        void init() override {
            DSPEffect::init();
        }


        void invalidate() override {
            DSPEffect::invalidate();
        }


        /**
         * @brief Compute next value
         * @param x
         * @return
         */
        float next(float x) {
            in = x;
            process();

            return out;
        }


        /**
         * @brief Compute tanh
         */
        void process() override {
            rs->doUpsample(STD_CHANNEL, in);

            for (int i = 0; i < rs->getFactor(); i++) {
                float x = rs->getUpsampled(STD_CHANNEL)[i];
                rs->data[STD_CHANNEL][i] = tanh(x);
            }

            out = rs->getDownsampled(STD_CHANNEL);
        }

    };


}