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

#include <dsp/functions.hpp>
#include "../dsp/IIRFilter.hpp"
#include "../dsp/DelayLine.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"

using namespace rack;
using namespace lrt;


struct TestDriver : LRModule {
    enum ParamIds {
        A1_PARAM,
        A2_PARAM,
        B1_PARAM,
        B2_PARAM,
        S1_PARAM,
        T1_PARAM,
        A1_CV_PARAM,
        A2_CV_PARAM,
        B1_CV_PARAM,
        B2_CV_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        INPUT_A,
        INPUT_B,
        A1_CV_INPUT,
        A2_CV_INPUT,
        B1_CV_INPUT,
        B2_CV_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        OUTPUT_A,
        OUTPUT_B,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    LRKnob *a1Knob, *a2Knob, *b1Knob, *b2Knob;

    LRLCDWidget *lcd = new LRLCDWidget(10, "%s", LRLCDWidget::LIST, 10);

    lrt::DelayLine *ddlL = new lrt::DelayLine(args.sampleRate);
    lrt::DelayLine *ddlR = new lrt::DelayLine(args.sampleRate);

    lrt::IIRFilter *iir, *iir2;


    TestDriver() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        a1Knob = LRKnob::create<LRBigKnob>(Vec(32.9, 68.6 + 7), this, TestDriver::A1_PARAM, 0.f, 1.f, 1.f);
        a2Knob = LRKnob::create<LRMiddleKnob>(Vec(39.9, 174.1 + 7), this, TestDriver::A2_PARAM, 0.f, 1.f, 0.f);
        b1Knob = LRKnob::create<LRBigKnob>(Vec(196.2, 68.6 + 7), this, TestDriver::B1_PARAM, 0.f, 1.f, 0.f);
        b2Knob = LRKnob::create<LRMiddleKnob>(Vec(203.1, 174.1 + 7), this, TestDriver::B2_PARAM, 0.f, 1.f, 0.f);

        float antiAliasACoefVals[8] = {0.00002808162654240159, 0.00324260184959550063, 0.01229358405255225224, 0.01781338547909389058,
                                       0.01191661427670600605, 0.00351674566780364349, 0.00034109599239625331, 0.00000665645917792087};

        float antiAliasBCoefVals[8] = {1.00000000000000000000, 0.27002485476511250972, -2.35210121584816533868, -0.47624252782494447267,
                                       1.85825180320240179732, 0.30789821629786440216, -0.48145639585378052772, -0.07721597509005941051};

        iir = new lrt::IIRFilter(args.sampleRate, antiAliasACoefVals, antiAliasBCoefVals, 8);

        /* float reconstruction1BCoefVals[6] = {0.00051516013318437094, 0.03041821522444834724, 0.06752981661722373685, 0
                                               .04770540623300938837,
                                              0.01033396947561467973, 0.00001106873510238544};
         float reconstruction1ACoefVals[6] = {1.00000000000000000000, -0.63300309066683380088, -1.16456643091420186664,
                                              0.76529508607787266605, 0.33776127751256762588, -0.14897208486870763822};*/

        float reconstruction2BCoefVals[3] = {0.01440296150822061375, 0.12374170373521460597, 0.01322614050481232296};
        float reconstruction2ACoefVals[3] = {1.00000000000000000000, -1.75672492751137410139, 0.90805921207607520618};


        iir2 = new lrt::IIRFilter(args.sampleRate, reconstruction2BCoefVals, reconstruction2ACoefVals, 3);


    }


    void process(const ProcessArgs &args) override {
        // compute all cv values
        float a1value = inputs[A1_CV_INPUT].getVoltage() * 0.1f * quadraticBipolar(params[A1_CV_PARAM].getValue());
        float a2value = inputs[A2_CV_INPUT].getVoltage() * 0.1f * quadraticBipolar(params[A2_CV_PARAM].getValue());

        float frq2cv = inputs[B1_CV_INPUT].getVoltage() * 0.1f * quadraticBipolar(params[B1_CV_PARAM].getValue());
        float peak2cv = inputs[B2_CV_INPUT].getVoltage() * 0.1f * quadraticBipolar(params[B2_CV_PARAM].getValue());


        // set vc parameter and knob values
        auto a1 = params[A1_PARAM].getValue() + a1value;
        auto a2 = params[A2_PARAM].getValue() + a2value;
        auto b1 = params[B1_PARAM].getValue() + frq2cv;
        auto b2 = params[B2_PARAM].getValue() + peak2cv;

        auto s1 = params[S1_PARAM].getValue() == 1;


        if (a1Knob != nullptr && b1Knob != nullptr && a2Knob != nullptr && b2Knob != nullptr) {
            a1Knob->setIndicatorActive(inputs[A1_CV_INPUT].isConnected());
            a2Knob->setIndicatorActive(inputs[A2_CV_INPUT].isConnected());
            b1Knob->setIndicatorActive(inputs[B1_CV_INPUT].isConnected());
            b2Knob->setIndicatorActive(inputs[B2_CV_INPUT].isConnected());

            a1Knob->setIndicatorValue(params[A1_PARAM].getValue() + a1value);
            a2Knob->setIndicatorValue(params[A2_PARAM].getValue() + a2value);
            b1Knob->setIndicatorValue(params[B1_PARAM].getValue() + frq2cv);
            b2Knob->setIndicatorValue(params[B2_PARAM].getValue() + peak2cv);
        }

        /* computations */
        /*auto dL = params[A1_PARAM].getValue() * (args.sampleRate - 1) + 1;
        auto fbL = params[A2_PARAM].getValue();

        auto dR = params[B1_PARAM].getValue() * (args.sampleRate - 1) + 1;
        auto fbR = params[B2_PARAM].getValue();

        ddlL->delay = dL;
        ddlL->fb = fbL;
        ddlL->invalidate();
        ddlL->in = inputs[INPUT_A].getVoltage();
        ddlL->process();

        outputs[OUTPUT_A].setVoltage(ddlL->out + inputs[INPUT_A].getVoltage());

        ddlR->delay = dR;
        ddlR->fb = fbR;
        ddlR->invalidate();
        ddlR->in = inputs[INPUT_B].isConnected() ? inputs[INPUT_B].getVoltage() : inputs[INPUT_A].getVoltage();
        ddlR->process();

        outputs[OUTPUT_B].setVoltage(inputs[INPUT_B].isConnected() ? ddlR->out + inputs[INPUT_B].getVoltage() : ddlR->out + inputs[INPUT_A].getVoltage());*/

        iir->in = inputs[INPUT_A].getVoltage();
        iir->process();
        outputs[OUTPUT_A].setVoltage(iir->out);

        iir2->in = inputs[INPUT_A].getVoltage();
        iir2->process();
        outputs[OUTPUT_B].setVoltage(iir2->out);
    }


    json_t *dataToJson() override {
        json_t *rootJ = json_object();
        json_object_set_new(rootJ, "lcdindex", json_integer((int) lround(lcd->value)));

        return rootJ;
    }


    void dataFromJson(json_t *rootJ) override {
        LRModule::dataFromJson(rootJ);

        json_t *mode = json_object_get(rootJ, "lcdindex");

        if (mode)
            lcd->value = json_integer_value(mode);// json_real_value(mode);

        lcd->dirty = true;
    }


    void onSampleRateChange() override {
        LRModule::onSampleRateChange();
    }
};


/**
 * @brief Blank Panel with Logo
 */
struct TestDriverWidget : LRModuleWidget {
    TestDriverWidget(TestDriver *module);
};


TestDriverWidget::TestDriverWidget(TestDriver *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/TestDriver.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/TestDriver.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/TestDriver.svg")));

    panel->init();
    addChild(panel);

    box.size = panel->box.size;

    // **** SETUP LCD ********
    module->lcd->box.pos = Vec(100, 194);
    module->lcd->items = {"MODE A", "MODE B", "MODE C", "MODE D", "MODE E"};
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
    module->a1Knob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->a2Knob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->b1Knob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->b2Knob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    addParam(module->a1Knob);
    addParam(module->a2Knob);
    addParam(module->b1Knob);
    addParam(module->b2Knob);

    addParam(ParamcreateWidget<LRSmallKnob>(Vec(36.5 - 7.5, 269.4), module, TestDriver::A1_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamcreateWidget<LRSmallKnob>(Vec(78.5 - 7.5, 269.4), module, TestDriver::A2_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamcreateWidget<LRSmallKnob>(Vec(197.5 - 7.5, 269.4), module, TestDriver::B1_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamcreateWidget<LRSmallKnob>(Vec(239.5 - 7.5, 269.4), module, TestDriver::B2_CV_PARAM, -1.f, 1.0f, 0.f));

    addParam(ParamcreateWidget<LRSmallToggleKnob>(Vec(126.6, 112.2), module, TestDriver::T1_PARAM, 0.f, 10.0f, 0.f));

    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addInput(createInput<LRIOPortCV>(Vec(34.4 - 7.5, 312), module, TestDriver::A1_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(76.4 - 7.5, 312), module, TestDriver::A2_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(195.4 - 7.5, 312), module, TestDriver::B1_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(237.4 - 7.5, 312), module, TestDriver::B2_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(118 - 8, 269), module, TestDriver::INPUT_A));
    addInput(createInput<LRIOPortAudio>(Vec(118 - 8, 312), module, TestDriver::INPUT_B));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(156 - 8, 269), module, TestDriver::OUTPUT_A));
    addOutput(createOutput<LRIOPortAudio>(Vec(156 - 8, 312), module, TestDriver::OUTPUT_B));
    // ***** OUTPUTS *********

    addParam(ParamcreateWidget<LRSwitch>(Vec(131.1, 245.2), module, TestDriver::S1_PARAM, 0, 1, 0));
}


Model *modelTestDriver = createModel<TestDriver, TestDriverWidget>("Lindenberg Research", "TestDriver", "TestDrive Module for "
                                                                                                          "ProtoTyping", UTILITY_TAG);
