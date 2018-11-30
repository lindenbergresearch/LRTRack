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

#include "Korg35Filter.hpp"
#include "DSPEffect.hpp"


dsp::Korg35FilterStage::Korg35FilterStage(float sr, FilterType type) : DSPEffect(sr) {
    this->type = type;
}


void dsp::Korg35FilterStage::init() {
    type = LPF1;
    alpha = 1.f;
    beta = 1.f;
    zn1 = 0;
    fc = 0;
}


void dsp::Korg35FilterStage::invalidate() {
    float wd = 2 * M_PI * fc;
    float T = 1 / sr;
    float wa = (2 / T) * tan(wd * T / 2);
    float g = wa * T / 2;

    alpha = g / (1.f + g);
}


void dsp::Korg35FilterStage::process() {
    // v(n)
    float vn = (in - zn1) * alpha;

    float lpf = vn + zn1;

    zn1 = vn + lpf;

    // switch filter type
    if (type == LPF1) {
        out = lpf;
    } else {
        float hpf = in - lpf;
        out = hpf;
    }

}


dsp::Korg35Filter::Korg35Filter(float sr) : DSPEffect(sr) {}
