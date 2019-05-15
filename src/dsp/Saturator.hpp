#pragma once

#include "WaveShaper.hpp"

#define SATURATOR_GAIN 0.05

namespace lrt {

struct Saturator : WaveShaper {

    Noise *noise;


public:

    explicit Saturator(float sr);

    void init() override;
    void invalidate() override;
    void process() override;
    double compute(double x) override;

};

}