#pragma once


#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"

namespace rack {

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
    struct MS20zdf : DSPEffect {
        static const int OVERSAMPLE = 1;    // factor of internal oversampling

    private:
        // cutoff frequency and peak
        float frequency, peak;

        float in;
        float g, g2, b, k;
        float ky, y;
        float freqHz;

        ZDF zdf1, zdf2;

        float lpOut, hpOut;

    public:
        float getFrequency() const;
        void setFrequency(float frequency);
        float getFreqHz() const;
        float getPeak() const;
        void setPeak(float peak);
        void setIn(float in);


        void invalidate() override;
        void process() override;

        float getLpOut() const;
        float getHpOut() const;
    };


}