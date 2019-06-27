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


struct VULevelMeterWidget;


struct VULevelMeter : LRModule {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        AUDIO_LEFT_INPUT,
        AUDIO_RIGHT_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    VULevelMeterWidget *reflect;


    VULevelMeter() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    }


    void process(const ProcessArgs &args) override;
};


/**
 * @brief Blank Panel Mark I
 */
struct VULevelMeterWidget : LRModuleWidget {
    LRLevelWidget<LRRoundRectLevelLED> *levelWidgetL = new LRLevelWidget<LRRoundRectLevelLED>(Vec(30, 30), Vec(19, 7), 2.0f, 4.f, 30);
    LRLevelWidget<LRRoundRectLevelLED> *levelWidgetR = new LRLevelWidget<LRRoundRectLevelLED>(Vec(93, 30), Vec(19, 7), 2.0f, 4.f, 30);

    VULevelMeterWidget(VULevelMeter *module);
};


VULevelMeterWidget::VULevelMeterWidget(VULevelMeter *module) : LRModuleWidget(module) {

    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/VULevelMeter.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Light.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Aged.svg")));

    panel->init();
    addChild(panel);

    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;

    addChild(levelWidgetL);
    addChild(levelWidgetR);

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(30, 330), module, VULevelMeter::AUDIO_LEFT_INPUT));
    addInput(createInput<LRIOPortAudio>(Vec(93, 330), module, VULevelMeter::AUDIO_RIGHT_INPUT));
    // ***** INPUTS **********
}


void VULevelMeter::process(const ProcessArgs &args) {
    if (reflect && reflect->levelWidgetL && inputs[AUDIO_LEFT_INPUT].isConnected())
        reflect->levelWidgetL->setValue(fabs(inputs[AUDIO_LEFT_INPUT].getVoltage() / 10.f));

    if (reflect && reflect->levelWidgetR && inputs[AUDIO_RIGHT_INPUT].isConnected())
        reflect->levelWidgetR->setValue(fabs(inputs[AUDIO_RIGHT_INPUT].getVoltage() / 10.f));
}


Model *modelVULevelMeter = createModel<VULevelMeter, VULevelMeterWidget>("VULevelMeter");


