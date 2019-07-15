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

/**
 * @brief Helper class representing a fractional-index access to an array with interpolation
 * @tparam T Floating point type
 */
template<class T>
struct FracVector {
    std::vector<T> v;


    /**
     * @brief Constructs a new Flowting index vector
     * @param size
     */
    FracVector(int size) {
        v.resize(size);
    }


    /**
     * @brief Get interpolated value with fractional index
     * @param position fractional index
     * @return interpolated value
     */
    T get(float position) {
        // get the fractional part of the float
        int c0 = (int) truncf(position);
        T c1 = position - (T) c0;
        // get counterpart
        T c2 = 1 - c1;

        return c1 * v[c0] + c2 * v[c0 + 1];
    }


    /**
     * @brief Operator wrapper for frac access
     * @param position
     * @return
     */
    T operator[](float position) {
        return get(position);
    }


    /**
     * @brief Operator wrapper for normal access
     * @param position
     * @return
     */
    T operator[](int position) {
        return v[position];
    }

};


template<class T, int STAGES>
struct BBDCore : DSPEffect {
    FracVector<T> fvect = FracVector<T>(STAGES);

    T clk, stepsize, time;
    T i;
    T in, out;


    /**
     * @brief Update DDB parameters
     */
    void invalidate() override {
        clk = STAGES / (2 * time);  // clock-rate of the BBD
        stepsize = sr / clk;        // step size
    }


    void init() override;
    void process() override;
};


}
