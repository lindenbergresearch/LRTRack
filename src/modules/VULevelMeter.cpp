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
#include "../dsp/DSPMath.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"

using namespace rack;
using namespace lrt;


struct VULevelMeter : LRModule {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        NUM_INPUTS
    };
    enum OutputIds {
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };


    VULevelMeter() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    }


    void process(const ProcessArgs &args) override;
};


void VULevelMeter::process(const ProcessArgs &args) {
}


/**
 * @brief Blank Panel Mark I
 */
struct VULevelMeterWidget : LRModuleWidget {
    VULevelMeterWidget(VULevelMeter *module);
};


VULevelMeterWidget::VULevelMeterWidget(VULevelMeter *module) : LRModuleWidget(module) {

    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Light.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Aged.svg")));

    panel->init();
    addChild(panel);


    box.size = panel->box.size;
}


Model *modelVULevelMeter = createModel<VULevelMeter, VULevelMeterWidget>("VULevelMeter");


