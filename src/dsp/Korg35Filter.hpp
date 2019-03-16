/*                                                                     *\
**       __   ___  ______                                              **
**      / /  / _ \/_  __/                                              **
**     / /__/ , _/ / /    Lindenberg                                   **
**    /____/_/|_| /_/  Research Tec.                                   **
**                                                                     **
**                                                                     **
**	  https://github.com/lindenbergresearch/LRTRack	                   **
**    heapdump@icloud.com                                              **
**		                                                               **
**    Sound Modules for VCV Rack                                       **
**    Copyright 2017/2018 by Patrick Lindenberg / LRT                  **
**                                                                     **
**    For Redistribution and use in source and binary forms,           **
**    with or without modification please see LICENSE.                 **
**                                                                     **
\*                                                                     */

#pragma once


#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"

namespace dsp {

struct Korg35FilterStage : DSPEffect {
    enum FilterType {
        LP_STAGE,   // lowpass stage
        HP_STAGE    // highpass stage
    };

    bool dedicated = false;

    float fc;
    FilterType type;
    float alpha, beta;
    float zn1;

    float in, out;

    Korg35FilterStage(float sr, FilterType type);


    inline float getFeedback() {
        return zn1 * beta;
    }


    void init() override;
    void invalidate() override;
    void process() override;
};


struct Korg35Filter : DSPEffect {
    static constexpr float MAX_FREQUENCY = 20000.f;

    Korg35FilterStage *lpf, *hpf1, *hpf2;
    float Ga;

    float in, out;

    // cutofffrq, peak (resonance) and saturation level
    float fc, peak, sat;


    Korg35Filter(float sr) : DSPEffect(sr) {
        lpf = new Korg35FilterStage(sr, Korg35FilterStage::LP_STAGE);
        hpf1 = new Korg35FilterStage(sr, Korg35FilterStage::HP_STAGE);
        hpf2 = new Korg35FilterStage(sr, Korg35FilterStage::HP_STAGE);
    }


    void init() override;
    void invalidate() override;
    void process() override;
    void setSamplerate(float sr) override;
};

}


