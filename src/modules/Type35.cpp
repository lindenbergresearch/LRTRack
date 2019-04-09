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
#include "../dsp/Type35Filter.hpp"


using namespace rack;
using namespace lrt;

using dsp::Type35Filter;


struct Type35 : LRModule {
    enum ParamIds {
        FREQ1_PARAM,
        PEAK1_PARAM,
        FREQ2_PARAM,
        PEAK2_PARAM,
        SAT1_PARAM,
        CUTOFF1_CV_PARAM,
        PEAK1_CV_PARAM,
        CUTOFF2_CV_PARAM,
        PEAK2_CV_PARAM,
        MIX_PARAM,
        MODE_SWITCH_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        CUTOFF1_CV_INPUT,
        PEAK1_CV_INPUT,
        CUTOFF2_CV_INPUT,
        PEAK2_CV_INPUT,
        SATURATE_CV_INPUT,
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

    LRKnob *frqKnobLP, *peakKnobLP, *frqKnobHP, *peakKnobHP, *saturateKnob;
    Type35Filter *lpf = new Type35Filter(engineGetSampleRate(), Type35Filter::LPF);
    Type35Filter *hpf = new Type35Filter(engineGetSampleRate(), Type35Filter::HPF);


    Type35() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override {
        lpf->fc = params[FREQ1_PARAM].value;
        lpf->peak = params[PEAK1_PARAM].value;
        hpf->fc = params[FREQ2_PARAM].value;
        hpf->peak = params[PEAK2_PARAM].value;

        lpf->sat = params[SAT1_PARAM].value;
        hpf->sat = params[SAT1_PARAM].value;


        if (params[MODE_SWITCH_PARAM].value > 0) {
            hpf->in = inputs[FILTER_INPUT].value;
            hpf->invalidate();
            hpf->process2();

            // cascade
            lpf->in = hpf->out;//inputs[FILTER_INPUT].value;
            lpf->invalidate();
            lpf->process2();

            outputs[LP_OUTPUT].value = lpf->out;
        } else {
            lpf->in = inputs[FILTER_INPUT].value;
            lpf->invalidate();
            lpf->process2();

            // cascade
            hpf->in = lpf->out;//inputs[FILTER_INPUT].value;
            hpf->invalidate();
            hpf->process2();

            outputs[LP_OUTPUT].value = hpf->out;
        }
    }


    void onSampleRateChange() override {
        Module::onSampleRateChange();
        lpf->setSamplerate(engineGetSampleRate());
        hpf->setSamplerate(engineGetSampleRate());
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
    module->frqKnobLP = LRKnob::create<LRBigKnob>(Vec(36.4, 68.3), module, Type35::FREQ1_PARAM, 0.f, 1.f, 1.f);
    module->peakKnobLP = LRKnob::create<LRMiddleKnob>(Vec(43.4, 174.8), module, Type35::PEAK1_PARAM, 0.f, 1.f, 0.f);

    module->frqKnobHP = LRKnob::create<LRBigKnob>(Vec(207.7, 68.3), module, Type35::FREQ2_PARAM, 0.f, 1.f, 0.f);
    module->peakKnobHP = LRKnob::create<LRMiddleKnob>(Vec(214.6, 174.8), module, Type35::PEAK2_PARAM, 0.001f, 1.5, 0.001f);


    module->saturateKnob = LRKnob::create<LRMiddleKnob>(Vec(129.5, 149.2), module, Type35::SAT1_PARAM, 1.f, 2.5, 1.0f);

    module->frqKnobLP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->peakKnobLP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    module->frqKnobHP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->peakKnobHP->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    module->saturateKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    addParam(module->frqKnobLP);
    addParam(module->peakKnobLP);

    addParam(module->frqKnobHP);
    addParam(module->peakKnobHP);

    addParam(module->saturateKnob);

    addParam(ParamWidget::create<LRSmallKnob>(Vec(32.5, 269.4), module, Type35::CUTOFF1_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(74.5, 269.4), module, Type35::PEAK1_CV_PARAM, -1.f, 1.0f, 0.f));

    addParam(ParamWidget::create<LRSmallKnob>(Vec(201.5, 269.4), module, Type35::CUTOFF2_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(243.5, 269.4), module, Type35::PEAK2_CV_PARAM, -1.f, 1.0f, 0.f));

    addParam(ParamWidget::create<LRSmallKnob>(Vec(138.5, 84.4), module, Type35::MIX_PARAM, -1.f, 1.0f, 0.f));


    addInput(Port::create<LRIOPortCV>(Vec(30.4, 312), Port::INPUT, module, Type35::CUTOFF1_CV_INPUT));
    addInput(Port::create<LRIOPortCV>(Vec(72.4, 312), Port::INPUT, module, Type35::PEAK1_CV_INPUT));

    addInput(Port::create<LRIOPortCV>(Vec(199.4, 312), Port::INPUT, module, Type35::CUTOFF2_CV_INPUT));
    addInput(Port::create<LRIOPortCV>(Vec(241.4, 312), Port::INPUT, module, Type35::PEAK2_CV_INPUT));

    addInput(Port::create<LRIOPortCV>(Vec(136.4, 229), Port::INPUT, module, Type35::SATURATE_CV_INPUT));

    /*

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

    addParam(ParamWidget::create<LRSwitch>(Vec(135, 55), module, Type35::MODE_SWITCH_PARAM, 0, 1, 0));
}


Model *modelType35 = Model::create<Type35, Type35Widget>("Lindenberg Research", "TYPE35 VCF", "Sallen-Key Type 35 Dual Filter", FILTER_TAG);
