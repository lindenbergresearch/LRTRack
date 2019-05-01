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

namespace dsp {

using std::vector;


/**
 * @brief Implements a standard IIR filter with a dynamic order in direct form I
 */
struct IIRFilter : DSPEffect {

    /**
     * @brief Construct an IIR Filter with the given coefficients
     * @param sr Samplerate
     * @param a A Coefficients
     * @param b B Coefficients
     */
    IIRFilter(float sr, vector<float> &a, vector<float> &b) : DSPEffect(sr), a(a), b(b) {}


    /**
     * @brief Construct an IIR Filter with the given coefficients (as Array)
     * @param sr Samplerate
     * @param a A Coefficients
     * @param b B Coefficients
     * @param size Total size of the coefficients (where a and b are symmetric)
     */
    IIRFilter(float sr, float *a, float *b, int size) : DSPEffect(sr) {
        vector<float> _a(a, a + size);
        vector<float> _b(a, b + size);

        this->a = _a;
        this->b = _b;
    }


    void init() override;
    void invalidate() override;
    void process() override;

    float in, out;
    vector<float> a, b;
    vector<float> x, y;

private:

    /**
     * @brief Shift all elements right in a vector
     * @param data Reference to a vector<float>
     */
    inline void shiftRight(vector<float> &data, float fill = 0) {
        for (int i = data.size() - 1; i > 0; i--) {
            data[i + 1] = data[i];
        }

        data[0] = fill;
    }
};

}


