#pragma once

#include "DSPMath.hpp"
#include "DSPEffect.hpp"

// constants for Lockhart wavesahper model
#define LOCKHART_RL 7.5e3f
#define LOCKHART_R 15e3f
#define LOCKHART_VT 26e-3f
#define LOCKHART_Is 10e-16f
#define LOCKHART_THRESHOLD 10e-10


namespace dsp {

    /**
     * @brief Basic WaveShaper class with build-in dynamic oversampling
     * @tparam OVERSAMPLE
     */
    struct WaveShaper : DSPEffect {
        /* oversampling channel */
        static const int STD_CHANNEL = 1;
        static const int OVERSAMPLE = 1;
        static constexpr float MAX_BIAS_LEVEL = 0.7;

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
            return sr * OVERSAMPLE;
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
        virtual float compute(float x) { return x; }
    };


    /**
     * @brief Represents one stage of the Lockhart Wavefolder
     */
    struct LockhartWFStage {
    private:
        float ln1, fn1, xn1;

    public:
        inline float compute(float x) {
            float out;
            float a, b, d, l, u, ln, fn;

            a = 2 * LOCKHART_RL / LOCKHART_R;
            b = (LOCKHART_R + 2 * LOCKHART_RL) / (LOCKHART_VT * LOCKHART_R);
            d = (LOCKHART_RL * LOCKHART_Is) / LOCKHART_VT;

            // Compute Antiderivative
            l = sign(x);
            u = d * pow((float) M_E, l * b * x);
            ln = lambert_W(u, ln1);
            fn = (0.5f * LOCKHART_VT / b) * (ln * (ln + 2.f)) - 0.5f * a * x * x;

            // Check for ill-conditioning
            if (abs(x - xn1) < LOCKHART_THRESHOLD) {
                // Compute Averaged Wavefolder Output
                float xn = 0.5f * (x + xn1);
                u = d * powf((float) M_E, l * b * xn);
                ln = lambert_W(u, ln1);
                out = l * LOCKHART_VT * ln - a * xn;

            } else {
                // Apply AA Form
                out = (fn - fn1) / (x - xn1);
            }

            ln1 = ln;
            fn1 = fn;
            xn1 = x;

            return out;
        }
    };


    /**
     * Lockhart Wavefolder class
     */
    struct LockhartWavefolder : WaveShaper {

    private:
        LockhartWFStage l1, l2, l3, l4;


    public:
        explicit LockhartWavefolder(float sr);

        void init() override;
        void invalidate() override;
        void process() override;
        float compute(float x) override;

    };


}

