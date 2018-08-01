#pragma once

#include "WaveShaper.hpp"

// constants for Lockhart waveshaper model
#define LOCKHART_RL 7.5e3
#define LOCKHART_R 15e3
#define LOCKHART_VT 26e-3
#define LOCKHART_Is 10e-16
#define LOCKHART_THRESHOLD 10e-10

namespace dsp {

/**
 * @brief Represents one stage of the Lockhart Wavefolder
 */
    struct LockhartWFStage {
    private:
        double ln1, fn1, xn1;
        double a, b, d;

    public:

        LockhartWFStage();

        double compute(double x);
    };


/**
 * Lockhart Wavefolder class
 */
    struct LockhartWavefolder : WaveShaper {

    private:
        LockhartWFStage lh;


    public:
        explicit LockhartWavefolder(float sr);

        void init() override;
        void invalidate() override;
        void process() override;
        double compute(double x) override;

    };

}
