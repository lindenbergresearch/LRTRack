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

#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"
#include "../dsp/Korg35Filter.hpp"


using namespace rack;
using namespace lrt;

using dsp::Korg35Filter;


struct Type35 : LRModule {
    enum ParamIds {
        FREQ1_PARAM,
        PEAK1_PARAM,
        SAT1_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        LP_OUTPUT,
        HO_OUT,
        MIX_OUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    LRKnob *frqKnob, *peakKnob, *saturateKnob;
    Korg35Filter *filter = new Korg35Filter(engineGetSampleRate(), Korg35Filter::LPF);


    Type35() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override {
        filter->fc = params[FREQ1_PARAM].value;
        filter->peak = params[PEAK1_PARAM].value;
        filter->sat = quadraticBipolar(params[SAT1_PARAM].value);

        filter->in = inputs[FILTER_INPUT].value;
        filter->invalidate();
        filter->process();

        outputs[LP_OUTPUT].value = filter->out;
    }


    void onSampleRateChange() override {
        Module::onSampleRateChange();
        filter->setSamplerate(engineGetSampleRate());
    }
};


/**
 * @brief Blank Panel with Logo
 */
struct Type35Widget : LRModuleWidget {
    Type35Widget(Type35 *module);
};


Type35Widget::Type35Widget(Type35 *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestalt::DARK, SVG::load(assetPlugin(plugin, "res/panels/Type35VCF.svg")));
    panel->addSVGVariant(LRGestalt::LIGHT, SVG::load(assetPlugin(plugin, "res/panels/Type35VCF.svg")));
    panel->addSVGVariant(LRGestalt::AGED, SVG::load(assetPlugin(plugin, "res/panels/Type35VCF.svg")));

    panel->init();
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewLight>(Vec(15, 1)));
    addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewLight>(Vec(15, 366)));
    addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    module->frqKnob = LRKnob::create<LRBigKnob>(Vec(36.4, 68.3), module, Type35::FREQ1_PARAM, 0.f, 1.f, 1.f);
    module->peakKnob = LRKnob::create<LRMiddleKnob>(Vec(43.4, 174.8), module, Type35::PEAK1_PARAM, 0.001f, 2.0, 0.001f);
    module->saturateKnob = LRKnob::create<LRMiddleKnob>(Vec(129.5, 149.2), module, Type35::SAT1_PARAM, 1.f, 2.5, 1.0f);

    module->frqKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->peakKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->saturateKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    addParam(module->frqKnob);
    addParam(module->peakKnob);
    addParam(module->saturateKnob);/*

      addParam(ParamWidget::create<LRSmallKnob>(Vec(39.9, 251.4), module, DiodeVCF::FREQUENCY_CV_PARAM, -1.f, 1.0f, 0.f));
      addParam(ParamWidget::create<LRSmallKnob>(Vec(177, 251.4), module, DiodeVCF::RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f));
      addParam(ParamWidget::create<LRSmallKnob>(Vec(108.5, 251.4), module, DiodeVCF::SATURATE_CV_PARAM, -1.f, 1.0f, 0.f));*/
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    /*  addInput(Port::create<LRIOPortCV>(Vec(37.4, 284.4), Port::INPUT, module, DiodeVCF::FREQUCENCY_CV_INPUT));
      addInput(Port::create<LRIOPortCV>(Vec(175.3, 284.4), Port::INPUT, module, DiodeVCF::RESONANCE_CV_INPUT));
      addInput(Port::create<LRIOPortCV>(Vec(106.4, 284.4), Port::INPUT, module, DiodeVCF::SATURATE_CV_INPUT));*/
    // ***** CV INPUTS *******


    // ***** INPUTS **********
    addInput(Port::create<LRIOPortAudio>(Vec(118, 313), Port::INPUT, module, Type35::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPortAudio>(Vec(156, 313), Port::OUTPUT, module, Type35::LP_OUTPUT));
    // ***** OUTPUTS *********
}


Model *modelKorg35 = Model::create<Type35, Type35Widget>("Lindenberg Research", "TYPE35 VCF", "Sallen-Key Type 35 Dual Filter", FILTER_TAG);
