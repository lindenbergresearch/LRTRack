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

namespace lrt {

enum BiquadType {
    LOWPASS = 0,
    HIGHPASS,
    BANDPASS,
    NOTCH,
    PEAK,
    LOWSHELF,
    HIGHSHELF
};


/**
 * @brief Common Biquad filters
 * based on: https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
 */
struct Biquad : DSPEffect {
public:
    Biquad(BiquadType type, double Fc, double Q, double peakGainDB, float sr);
    ~Biquad();
    void setType(BiquadType type);
    void setQ(double Q);
    void setFc(double Fc);
    void setPeakGain(double peakGainDB);


    void setBiquad(BiquadType type, double Fc, double Q, double peakGain);
    void process() override;
    void invalidate() override;
    void init() override;
    double in, out;

protected:


    int type;
    double a0, a1, a2, b1, b2;
    double Fc, Q, peakGain;
    double z1, z2;
};


inline void Biquad::process() {
    out = in * a0 + z1;
    z1 = in * a1 + z2 - b1 * out;
    z2 = in * a2 - b2 * out;
}


}
