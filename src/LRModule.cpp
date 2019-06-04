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
#include "LRComponents.hpp"
#include "LRModel.hpp"

using namespace rack;
using namespace lrt;


/**
 * Compatibility constructor to pass the standard config via constructor in old 0.6 - style
 */
LRModule::LRModule(int numParams, int numInputs, int numOutputs, int numLights) {
    config(numParams, numInputs, numOutputs, numLights);
}


/**
 * @brief Dispatch randomize event to module widget
 */
void LRModule::onRandomize() {
    Module::onRandomize();
    reflect->randomize();
}



