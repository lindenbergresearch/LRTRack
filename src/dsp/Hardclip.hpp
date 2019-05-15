#pragma once

#include "WaveShaper.hpp"

#define HARDCLIP_GAIN 0.05

namespace lrt {

struct Hardclip : WaveShaper {

    Noise *noise;
    HQClip *hqclip;


public:

    explicit Hardclip(float sr);

    void init() override;
    void invalidate() override;
    void process() override;
    double compute(double x) override;

};

}