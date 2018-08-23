#pragma once

#include "WaveShaper.hpp"

#define SATURATOR_NOISE 0.0017234327
#define SATURATOR_GAIN 0.05

namespace dsp {

    struct Overdrive : WaveShaper {

        Noise *noise;


    public:

        explicit Overdrive(float sr);

        void init() override;
        void invalidate() override;
        void process() override;
        double compute(double x) override;

    };

}