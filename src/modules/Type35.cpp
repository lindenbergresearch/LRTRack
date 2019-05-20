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
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"
#include "../dsp/Type35Filter.hpp"


using namespace rack;
using namespace lrt;

using lrt::Type35Filter;


struct Type35 : LRModule {
    enum ParamIds {
        FREQ1_PARAM,
        PEAK1_PARAM,
        FREQ2_PARAM,
        PEAK2_PARAM,
        DRIVE_PARAM,
        CUTOFF1_CV_PARAM,
        PEAK1_CV_PARAM,
        CUTOFF2_CV_PARAM,
        PEAK2_CV_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        CUTOFF1_CV_INPUT,
        PEAK1_CV_INPUT,
        CUTOFF2_CV_INPUT,
        PEAK2_CV_INPUT,
        DRIVE_CV_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    LRKnob *frqKnobLP, *peakKnobLP, *frqKnobHP, *peakKnobHP, *driveKnob;
    Type35Filter *lpf = new Type35Filter(APP->engine->getSampleRate(), Type35Filter::LPF);
    Type35Filter *hpf = new Type35Filter(APP->engine->getSampleRate(), Type35Filter::HPF);
    LRLCDWidget *lcd = new LRLCDWidget(10, "%s", LRLCDWidget::LIST, 10);


    Type35() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        frqKnobLP = createParam<LRBigKnob>(Vec(32.9, 68.6 + 7), this, FREQ1_PARAM);
        peakKnobLP = createParam<LRMiddleKnob>(Vec(39.9, 174.1 + 7), this, PEAK1_PARAM);
        frqKnobHP = createParam<LRBigKnob>(Vec(196.2, 68.6 + 7), this, FREQ2_PARAM);
        peakKnobHP = createParam<LRMiddleKnob>(Vec(203.1, 174.1 + 7), this, PEAK2_PARAM);
        driveKnob = createParam<LRMiddleKnob>(Vec(122, 101.2), this, DRIVE_PARAM);


        configParam(FREQ1_PARAM, 0.f, 1.0f, 1.f);
        configParam(PEAK1_PARAM, 0.f, 1.0f, 0.f);
        configParam(FREQ2_PARAM, 0.f, 1.0f, 0.f);
        configParam(PEAK2_PARAM, 0.f, 1.0f, 0.f);
        configParam(DRIVE_PARAM, 1.f, 2.5f, 1.f);


        configParam(CUTOFF1_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(PEAK1_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(CUTOFF2_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(PEAK2_CV_PARAM, -1.f, 1.0f, 0.f);
    }


    void process(const ProcessArgs &args) override {
        // compute all cv values
        float frq1cv = inputs[CUTOFF1_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[CUTOFF1_CV_PARAM].getValue());
        float peak1cv = inputs[PEAK1_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[PEAK1_CV_PARAM].getValue());

        float frq2cv = inputs[CUTOFF2_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[CUTOFF2_CV_PARAM].getValue());
        float peak2cv = inputs[PEAK2_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[PEAK2_CV_PARAM].getValue());

        float drivecv = inputs[DRIVE_CV_INPUT].getVoltage();//* 0.1f * quadraticBipolar(params[DRIVE_CV_PARAM].getValue());

        // set vc parameter and knob values
        lpf->fc = params[FREQ1_PARAM].getValue() + frq1cv;
        lpf->peak = params[PEAK1_PARAM].getValue() + peak1cv;
        hpf->fc = params[FREQ2_PARAM].getValue() + frq2cv;
        hpf->peak = params[PEAK2_PARAM].getValue() + peak2cv;

        lpf->sat = params[DRIVE_PARAM].getValue() + drivecv;
        hpf->sat = params[DRIVE_PARAM].getValue() + drivecv;


        if (frqKnobLP != nullptr && frqKnobHP != nullptr && peakKnobLP != nullptr && peakKnobHP != nullptr && driveKnob != nullptr) {
            frqKnobLP->setIndicatorActive(inputs[CUTOFF1_CV_INPUT].isConnected());
            peakKnobLP->setIndicatorActive(inputs[PEAK1_CV_INPUT].isConnected());
            frqKnobHP->setIndicatorActive(inputs[CUTOFF2_CV_INPUT].isConnected());
            peakKnobHP->setIndicatorActive(inputs[PEAK2_CV_INPUT].isConnected());
            driveKnob->setIndicatorActive(inputs[DRIVE_CV_INPUT].isConnected());

            frqKnobLP->setIndicatorValue(params[FREQ1_PARAM].getValue() + frq1cv);
            peakKnobLP->setIndicatorValue(params[PEAK1_PARAM].getValue() + peak1cv);
            frqKnobHP->setIndicatorValue(params[FREQ2_PARAM].getValue() + frq2cv);
            peakKnobHP->setIndicatorValue(params[PEAK2_PARAM].getValue() + peak2cv);
            driveKnob->setIndicatorValue(params[DRIVE_PARAM].getValue() + drivecv);
        }

        if (lround(lcd->value) == 0) {
            hpf->in = inputs[FILTER_INPUT].getVoltage();
            hpf->invalidate();
            hpf->process2();

            lpf->in = hpf->out;
            lpf->invalidate();
            lpf->process2();

            outputs[OUTPUT].setVoltage(lpf->out);
        } else if (lround(lcd->value) == 1) {
            lpf->in = inputs[FILTER_INPUT].getVoltage();
            lpf->invalidate();
            lpf->process2();

            outputs[OUTPUT].setVoltage(lpf->out);
        } else if (lround(lcd->value) == 2) {
            lpf->in = inputs[FILTER_INPUT].getVoltage();
            lpf->invalidate();
            lpf->process2();

            hpf->in = inputs[FILTER_INPUT].getVoltage();
            hpf->invalidate();
            hpf->process2();

            outputs[OUTPUT].setVoltage(hpf->out + lpf->out);
        } else if (lround(lcd->value) == 3) {
            hpf->in = inputs[FILTER_INPUT].getVoltage();
            hpf->invalidate();
            hpf->process2();

            outputs[OUTPUT].setVoltage(hpf->out);
        } else if (lround(lcd->value) == 4) {
            lpf->in = inputs[FILTER_INPUT].getVoltage();
            lpf->invalidate();
            lpf->process2();

            hpf->in = lpf->out;
            hpf->invalidate();
            hpf->process2();

            outputs[OUTPUT].setVoltage(hpf->out);
        }


    }


    json_t *dataToJson() override {
        json_t *rootJ = json_object();
        json_object_set_new(rootJ, "filtermode", json_integer((int) lround(lcd->value)));

        return rootJ;
    }


    void dataFromJson(json_t *rootJ) override {
        LRModule::dataFromJson(rootJ);

        json_t *mode = json_object_get(rootJ, "filtermode");

        if (mode)
            lcd->value = json_integer_value(mode);// json_real_value(mode);

        lcd->dirty = true;
    }


    void onSampleRateChange() override {
        LRModule::onSampleRateChange();
        lpf->setSamplerate(APP->engine->getSampleRate());
        hpf->setSamplerate(APP->engine->getSampleRate());
    }
};


/**
 * @brief Blank Panel with Logo
 */
struct Type35Widget : LRModuleWidget {
    Type35Widget(Type35 *module) : LRModuleWidget(module) {
        panel->addSVGVariant(LRGestalt::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Type35VCF.svg")));
        panel->addSVGVariant(LRGestalt::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Type35VCFLight.svg")));
        panel->addSVGVariant(LRGestalt::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Type35VCFAged.svg")));

        panel->init();
        addChild(panel);

        box.size = panel->box.size;

        // **** SETUP LCD ********
        module->lcd->box.pos = Vec(100, 221);
        module->lcd->items = {"1: LP->HP", "2: LP", "3: LP + HP", "4: HP", " 5: HP->LP"};
        module->lcd->format = "%s";
        addChild(module->lcd);
        // **** SETUP LCD ********

        // ***** SCREWS **********
        addChild(createWidget<ScrewLight>(Vec(15, 1)));
        addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
        addChild(createWidget<ScrewLight>(Vec(15, 366)));
        addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
        // ***** SCREWS **********

        // ***** MAIN KNOBS ******
        module->frqKnobLP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
        module->peakKnobLP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
        module->frqKnobHP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
        module->peakKnobHP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
        module->driveKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

        addParam(module->frqKnobLP);
        addParam(module->peakKnobLP);
        addParam(module->frqKnobHP);
        addParam(module->peakKnobHP);
        addParam(module->driveKnob);

        addParam(createParam<LRSmallKnob>(Vec(36.5 - 7.5, 269.4), module, Type35::CUTOFF1_CV_PARAM));
        addParam(createParam<LRSmallKnob>(Vec(78.5 - 7.5, 269.4), module, Type35::PEAK1_CV_PARAM));
        addParam(createParam<LRSmallKnob>(Vec(197.5 - 7.5, 269.4), module, Type35::CUTOFF2_CV_PARAM));
        addParam(createParam<LRSmallKnob>(Vec(239.5 - 7.5, 269.4), module, Type35::PEAK2_CV_PARAM));
        // ***** MAIN KNOBS ******


        // ***** CV INPUTS *******
        addInput(createInput<LRIOPortCV>(Vec(34.4 - 7.5, 312), module, Type35::CUTOFF1_CV_INPUT));
        addInput(createInput<LRIOPortCV>(Vec(76.4 - 7.5, 312), module, Type35::PEAK1_CV_INPUT));
        addInput(createInput<LRIOPortCV>(Vec(195.4 - 7.5, 312), module, Type35::CUTOFF2_CV_INPUT));
        addInput(createInput<LRIOPortCV>(Vec(237.4 - 7.5, 312), module, Type35::PEAK2_CV_INPUT));
        addInput(createInput<LRIOPortCV>(Vec(129.4, 172), module, Type35::DRIVE_CV_INPUT));
        // ***** CV INPUTS *******

        // ***** INPUTS **********
        addInput(createInput<LRIOPortAudio>(Vec(118 - 8, 269), module, Type35::FILTER_INPUT));
        // ***** INPUTS **********

        // ***** OUTPUTS *********
        addOutput(createOutput<LRIOPortAudio>(Vec(156 - 8, 269), module, Type35::OUTPUT));
        // ***** OUTPUTS *********

        // addParam(ParamcreateWidget<LRSwitch>(Vec(135, 55), module, Type35::MODE_SWITCH_PARAM, 0, 1, 0));
    }
};


Model *modelType35 = createModel<Type35, Type35Widget>("TYPE35 VCF");
