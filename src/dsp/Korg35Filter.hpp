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
        LPF1,   // lowpass stage
        HPF1    // highpass stage
    };

private:
    float fc;
    FilterType type;
    float alpha, beta;
    float zn1;

    float in, out;

public:
    Korg35FilterStage(float sr);


    inline float getFeedback() {
        return zn1 * beta;
    }


    void init() override;
    void invalidate() override;
    void process() override;

};

}


