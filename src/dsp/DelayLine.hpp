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
#include "DSPMath.hpp"
#include "HQTrig.hpp"
#include "BiquadFilter.hpp"

namespace lrt {


/**
 * @brief Interpolated delay buffer
 */
struct DelayBuffer {
    /* information lost on BBD */
    static constexpr float NOISE_GAIN = 0.01;

    float *buffer;
    int counter, MAX_WG_DELAY;
    double delay;
    float fb;

    Biquad *filter;
    Noise noise;


    explicit DelayBuffer() {
        filter = new Biquad(NOTCH, 200.0, 0.123, 0.0, 44100.0);
    }


    virtual ~DelayBuffer() { delete buffer; };


    /**
     * @brief Clear delay buffer
     */
    void clear() {
        counter = 0;
        for (int s = 0; s < MAX_WG_DELAY; s++)
            buffer[s] = 0;
    }


    /**
     * @brief Template to create a delay line buffer
     * @tparam TDelayBuffer Buffer class
     * @param size Size of delay buffer in samples
     * @param fb initial feedback
     * @param delay initial delay
     * @return New instance of a delay buffer
     */
    template<class TDelayBuffer>
    static TDelayBuffer *create(int size, float fb, double delay) {
        auto *delbuf = new DelayBuffer();

        delbuf->buffer = new float[size + 1];
        delbuf->fb = fb;
        delbuf->delay = delay;
        delbuf->MAX_WG_DELAY = size;

        return delbuf;
    }


    float feed(float in);
};


/**
 * @brief Simple Digital Delay Line
 */
struct DelayLine : DSPEffect {

    /* feedback */
    float fb;
    double delay;

    DelayBuffer *buffer;
    float in, out;

public:
    explicit DelayLine(float sr);

    virtual void init();
    virtual void invalidate();
    virtual void process();
};


}