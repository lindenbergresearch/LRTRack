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

#include "IIRFilter.hpp"
#include "DSPEffect.hpp"


void dsp::IIRFilter::init() {
    DSPEffect::init();
}


void dsp::IIRFilter::invalidate() {
    DSPEffect::invalidate();
}


void dsp::IIRFilter::process() {
    DSPEffect::process();
}

