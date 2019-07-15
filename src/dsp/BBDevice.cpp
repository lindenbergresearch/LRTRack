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
**    Copyright 2017-2019 by Patrick Lindenberg / LRT                  **
**                                                                     **
**    For Redistribution and use in source and binary forms,           **
**    with or without modification please see LICENSE.                 **
**                                                                     **
\*                                                                     */

#include "BBDevice.hpp"

using namespace lrt;


template<class T, int STAGES>
void BBDCore<T, STAGES>::init() {

}


template<class T, int STAGES>
void BBDCore<T, STAGES>::process() {
    T value = fvect[i];


    // increment fractional index with overflow check
    if (i + stepsize > STAGES) i = (i + stepsize) - STAGES;
    else i += stepsize;
}



