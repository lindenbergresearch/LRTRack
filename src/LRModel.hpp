#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"


namespace lrt {

/**
 * @brief Standard LR Module definition
 */
struct LRModule : public Module {
    LRGestalt *gestalt = nullptr;

    /**
     * @brief Default constructor derived from rack
     * @param numParams
     * @param numInputs
     * @param numOutputs
     * @param numLights
     */
    explicit LRModule(int numParams, int numInputs, int numOutputs, int numLights);
};


/**
 * @brief Standard LR ModuleWidget definition
 */
struct LRModuleWidget : ModuleWidget {

    /* standard module panel */
    LRPanel *panel = nullptr;

    /* Gestalt ID which represents the current theme */
    LRGestalt gestalt = dark;

    /**
     * @brief Default constructor derived from rack
     * @param module LRModule instance
     */
    explicit LRModuleWidget(LRModule *module) : ModuleWidget(module) {
        module->gestalt = &gestalt;
    }
};


}