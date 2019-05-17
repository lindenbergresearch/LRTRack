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



