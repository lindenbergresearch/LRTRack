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


#include "DelayLine.hpp"
#include "DSPEffect.hpp"


void dsp::DelayLine::init() {
    /* one second max delay */
    buffer = DelayBuffer::create<DelayBuffer>((int) sr, fb, delay);
    buffer->clear();
}


void dsp::DelayLine::invalidate() {
    DSPEffect::invalidate();

    buffer->fb = fb;
    buffer->delay = delay;
}


void dsp::DelayLine::process() {
    DSPEffect::process();

    out = buffer->feed(in);
}


dsp::DelayLine::DelayLine(float sr) : DSPEffect(sr) {
    init();
}


float dsp::DelayBuffer::feed(float in) {
    // calculate delay offset
    double back = (double) counter - delay;

    // clip lookback buffer-bound
    if (back < 0.0)
        back = MAX_WG_DELAY + back;


    // compute interpolation left-floor
    int const index0 = (int) floor(back);

    // compute interpolation right-floor
    int index_1 = index0 - 1;
    int index1 = index0 + 1;
    int index2 = index0 + 2;

    // clip interp. buffer-bound
    if (index_1 < 0)index_1 = MAX_WG_DELAY - 1;
    if (index1 >= MAX_WG_DELAY)index1 = 0;
    if (index2 >= MAX_WG_DELAY)index2 = 0;

    //debug("index_1 %d  index0 %d  index1 %d  index2 %d  count %d  max %d  delay %f  back %f", index_1, index0, index1, index2, counter,
    //      MAX_WG_DELAY, delay, back);

    // get neighbourgh samples
    float const y_1 = buffer[index_1] + noise.getNext(NOISE_GAIN) - NOISE_GAIN / 2;
    float const y0 = buffer[index0] + noise.getNext(NOISE_GAIN) - NOISE_GAIN / 2;
    float const y1 = buffer[index1] + noise.getNext(NOISE_GAIN) - NOISE_GAIN / 2;
    float const y2 = buffer[index2] + noise.getNext(NOISE_GAIN) - NOISE_GAIN / 2;


    // compute interpolation x
    float const x = (float) back - (float) index0;

    // calculate
    float const c0 = y0;
    float const c1 = 0.5f * (y1 - y_1);
    float const c2 = y_1 - 2.5f * y0 + 2.0f * y1 - 0.5f * y2;
    float const c3 = 0.5f * (y2 - y_1) + 1.5f * (y0 - y1);

    float const output = ((c3 * x + c2) * x + c1) * x + c0;


    // add to delay buffer
    filter->in = in + output * fb;
    filter->process();
    buffer[counter] = filter->out;

    // increment delay counter
    counter++;

    // clip delay counter
    if (counter >= MAX_WG_DELAY)
        counter = 0;

    // return output
    return output;
}