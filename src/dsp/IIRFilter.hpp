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

using std::vector;


/**
 * @brief Implements a standard IIR filter with a dynamic order in direct form I
 *
 *  In particular, this class implements the standard difference
 *  equation:
 *
 *  a[0]*y[n] = b[0]*x[n] + ... + b[nb]*x[n-nb] -
 *              a[1]*y[n-1] - ... - a[na]*y[n-na]
 *
 */
struct IIRFilter : DSPEffect {

    /**
     * @brief Construct an IIR Filter with the given coefficients
     * @param sr Samplerate
     * @param a A Coefficients
     * @param b B Coefficients
     */
    //TODO: does not work currently
    IIRFilter(float sr, vector<float> &a, vector<float> &b) : DSPEffect(sr), a(a), b(b) {
        vector<float> _x(a.size());
        vector<float> _y(a.size());

        x = _x;
        y = _y;
    }


    /**
     * @brief Construct an IIR Filter with the given coefficients (as Array)
     * @param sr Samplerate
     * @param a A Coefficients
     * @param b B Coefficients
     * @param size Total size of the coefficients (where a and b are symmetric)
     */
    IIRFilter(float sr, float *a, float *b, int size) : DSPEffect(sr) {
        vector<float> _a(a, a + size);
        vector<float> _b(b, b + size);
        vector<float> _x(size);
        vector<float> _y(size);

        this->a = _a;
        this->b = _b;
        x = _x;
        y = _y;
    }


    /**
     * @brief Perform IIR computations
     */
    void process() override {
        unsigned int i;
        out = 0;

        // put new input sample to buffer
        shiftRight(x, in);

        for (i = 0; i < a.size(); i++) {
            out += a[i] * x[i];
        }

        shiftRight(y);

        for (i = 1; i < b.size(); i++) {
            out -= b[i] * y[i];
        }

        y[0] = out;
    }


    float in, out;
    vector<float> a, b;
    vector<float> x, y;

private:

    /**
     * @brief Shift all elements right in a vector
     * @param data Reference to a vector<float>
     */
    static inline void shiftRight(vector<float> &data, float fill = 0) {
        for (int i = data.size() - 2; i >= 0; i--) {
            data[i + 1] = data[i];
        }

        data[0] = fill;
    }
};

}


