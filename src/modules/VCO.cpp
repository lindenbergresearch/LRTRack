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
#include <rack.hpp>
#include <dsp/common.hpp>
#include "../dsp/Oscillator.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"

using namespace rack;
using namespace lrt;

using lrt::DSPBLOscillator;

struct VCOWidget;


struct VCO : LRModule {
    enum ParamIds {
        FREQUENCY_PARAM,
        OCTAVE_PARAM,
        FM_CV_PARAM,
        PW_CV_PARAM,
        SAW_PARAM,
        PULSE_PARAM,
        SINE_PARAM,
        TRI_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        VOCT1_INPUT,
        FM_CV_INPUT,
        PW_CV_INPUT,
        VOCT2_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        SAW_OUTPUT,
        PULSE_OUTPUT,
        SINE_OUTPUT,
        TRI_OUTPUT,
        NOISE_OUTPUT,
        MIX_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        LFO_LIGHT,
        NUM_LIGHTS
    };

    VCOWidget *reflect;
    vector<DSPBLOscillator *> osc;


    VCO() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        configParam(FREQUENCY_PARAM, -1.f, 1.f, 0.f);
        configParam(OCTAVE_PARAM, -4.f, 3.f, 0.f);

        configParam(FM_CV_PARAM, -1.f, 1.f, 0.f);
        configParam(PW_CV_PARAM, -1, 1, 0.f);

        configParam(SAW_PARAM, -1.f, 1.f, 0.f);
        configParam(PULSE_PARAM, -1.f, 1.f, 0.f);
        configParam(SINE_PARAM, -1.f, 1.f, 0.f);
        configParam(TRI_PARAM, -1.f, 1.f, 0.f);

        osc.resize(16);

        osc[0] = new DSPBLOscillator(APP->engine->getSampleRate());
    }


    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
};


void VCO::onSampleRateChange() {
    Module::onSampleRateChange();

    for (int i = 0; i < inputs[VOCT1_INPUT].getChannels(); i++)
        if (osc[i] != nullptr)
            osc[i]->updateSampleRate(APP->engine->getSampleRate());
}

/*
void VCO::onRandomize() {
    Module::randomize();
}*/


/**
 * @brief Woldemar VCO Widget
 */
struct VCOWidget : LRModuleWidget {
    LRLCDWidget *lcd = new LRLCDWidget(10, "%00004.3f Hz", LRLCDWidget::NUMERIC, LCD_FONTSIZE);
    LRBigKnob *frqKnob = NULL;

    VCOWidget(VCO *module);
};


VCOWidget::VCOWidget(VCO *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/VCO.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Woldemar.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/WoldemarAged.svg")));

    panel->init();
    addChild(panel);
    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;


    // **** SETUP LCD ********
    lcd->box.pos = Vec(22, 222);
    lcd->format = "%00004.3f Hz";
    addChild(lcd);
    // **** SETUP LCD ********


    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(15, 1)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(createWidget<ScrewLight>(Vec(15, 366)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    frqKnob = createParam<LRBigKnob>(Vec(126.0, 64.7), module, VCO::FREQUENCY_PARAM);
    addParam(frqKnob);


    addParam(createParam<LRToggleKnob>(Vec(133, 170.5), module, VCO::OCTAVE_PARAM));

    addParam(createParam<LRSmallKnob>(Vec(69.5, 122), module, VCO::FM_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(69.5, 175), module, VCO::PW_CV_PARAM));


    addParam(createParam<LRSmallKnob>(Vec(22.8, 270.1), module, VCO::SAW_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(58.3, 270.1), module, VCO::PULSE_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(93.1, 270.1), module, VCO::SINE_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(128.1, 270.1), module, VCO::TRI_PARAM));
    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    addInput(createInput<LRIOPortCV>(Vec(20.8, 67.9), module, VCO::VOCT1_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(68.0, 67.9), module, VCO::VOCT2_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(20.8, 121.5), module, VCO::FM_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(20.8, 174.8), module, VCO::PW_CV_INPUT));
    // ***** INPUTS **********


    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(21, 305.8), module, VCO::SAW_OUTPUT));
    addOutput(createOutput<LRIOPortAudio>(Vec(56.8, 305.8), module, VCO::PULSE_OUTPUT));
    addOutput(createOutput<LRIOPortAudio>(Vec(91.6, 305.8), module, VCO::SINE_OUTPUT));
    addOutput(createOutput<LRIOPortAudio>(Vec(126.6, 305.8), module, VCO::TRI_OUTPUT));
    addOutput(createOutput<LRIOPortAudio>(Vec(162.0, 305.8), module, VCO::NOISE_OUTPUT));
    addOutput(createOutput<LRIOPortAudio>(Vec(162.0, 269.1), module, VCO::MIX_OUTPUT));
    // ***** OUTPUTS *********


    // ***** LIGHTS **********
    addChild(createLight<LRLight>(Vec(181.8, 210), module, VCO::LFO_LIGHT));
    // ***** LIGHTS **********
}


void VCO::process(const ProcessArgs &args) {

    for (int i = 0; i < inputs[VOCT1_INPUT].getChannels(); i++) {
        // if no oscillator for that channel found => create it
        if (osc[i] == nullptr) {
            DEBUG("osc created. channel: %d sum: %d", i, inputs[VOCT1_INPUT].getChannels());
            osc[i] = new DSPBLOscillator(args.sampleRate);
        }

        float fm = clamp(inputs[FM_CV_INPUT].getVoltage(), -CV_BOUNDS, CV_BOUNDS) * 0.4f * dsp::quadraticBipolar(params[FM_CV_PARAM].getValue());
        float tune = params[FREQUENCY_PARAM].getValue();
        float pw;

        if (inputs[PW_CV_INPUT].isConnected()) {
            pw = clamp(inputs[PW_CV_INPUT].getVoltage(), -CV_BOUNDS, CV_BOUNDS) * 0.6f *
                 dsp::quadraticBipolar(params[PW_CV_PARAM].getValue() / 2.f) + 1;
            pw = clamp(pw, 0.01, 1.99);
        } else {
            pw = params[PW_CV_PARAM].getValue() * 0.99f + 1;
        }

        reflect->frqKnob->setIndicatorActive(inputs[FM_CV_INPUT].isConnected());
        reflect->frqKnob->setIndicatorValue((params[FREQUENCY_PARAM].getValue() + 1) / 2 + (fm / 2));

        osc[i]->setInputs(inputs[VOCT1_INPUT].getVoltage(i), inputs[VOCT2_INPUT].getVoltage(), fm, tune, lround(params[OCTAVE_PARAM].getValue()));
        osc[i]->setPulseWidth(pw);

        osc[i]->process();

        outputs[SAW_OUTPUT].setVoltage(osc[i]->getSawWave(), i);
        outputs[PULSE_OUTPUT].setVoltage(osc[i]->getPulseWave(), i);
        outputs[SINE_OUTPUT].setVoltage(osc[i]->getSineWave(), i);
        outputs[TRI_OUTPUT].setVoltage(osc[i]->getTriWave(), i);
        outputs[NOISE_OUTPUT].setVoltage(osc[i]->getNoise(), i);


        if (outputs[MIX_OUTPUT].isConnected()) {
            float mix = 0.f;

            mix += osc[i]->getSawWave() * params[SAW_PARAM].getValue();
            mix += osc[i]->getPulseWave() * params[PULSE_PARAM].getValue();
            mix += osc[i]->getSineWave() * params[SINE_PARAM].getValue();
            mix += osc[i]->getTriWave() * params[TRI_PARAM].getValue();

            outputs[MIX_OUTPUT].setVoltage(mix, i);
        }
    }

    /* for LFO mode */
    if (osc[0]->isLFO()) lights[LFO_LIGHT].setSmoothBrightness(osc[0]->getSineWave() / 10.f + 0.3f, args.sampleTime);
    else lights[LFO_LIGHT].value = 0.f;

    reflect->lcd->active = osc[0]->isLFO();
    reflect->lcd->value = osc[0]->getFrequency();
}


Model *modelVCO = createModel<VCO, VCOWidget>("VCO");
