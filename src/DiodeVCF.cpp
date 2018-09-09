#include "LindenbergResearch.hpp"
#include "dsp/DiodeLadder.hpp"
#include "dsp/Hardclip.hpp"

using namespace rack;
using namespace lrt;


struct DiodeVCF : Module {
    enum ParamIds {
        FREQUENCY_PARAM,
        RES_PARAM,
        SATURATE_PARAM,
        GAIN_PARAM,
        FREQUENCY_CV_PARAM,
        MODE_SWITCH_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        FILTER_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };


    DiodeVCF() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}


    LRLCDWidget *lcd = new LRLCDWidget(nvgRGBAf(0.2, 0.09, 0.03, 1.0), 12, "%00004.3f Hz", LRLCDWidget::NUMERIC);
    dsp::DiodeLadderFilter *lpf = new dsp::DiodeLadderFilter(engineGetSampleRate());
    dsp::Hardclip *hardclip = new dsp::Hardclip(engineGetSampleRate());

    LRAlternateBigLight *frqKnob = NULL;
    LRAlternateMiddleLight *resKnob = NULL;
    LRAlternateMiddleLight *saturateKnob = NULL;

    void step() override;
    void onSampleRateChange() override;
};


void DiodeVCF::step() {
    lpf->setFrequency(params[FREQUENCY_PARAM].value);
    lpf->setResonance(params[RES_PARAM].value * 17.1);
    lpf->setSaturation(quarticBipolar(params[SATURATE_PARAM].value) * 14 + 1);

    lpf->low = params[MODE_SWITCH_PARAM].value != 0;

    lcd->value = lpf->getFreqHz();

    lpf->setIn(inputs[FILTER_INPUT].value / 10.f);
    lpf->invalidate();
    lpf->process();

    outputs[FILTER_OUTPUT].value = lpf->getOut() * 10.f;
}


void DiodeVCF::onSampleRateChange() {
    Module::onSampleRateChange();
    lpf->setSamplerate(engineGetSampleRate());
}


/**
 * @brief Blank Panel with Logo
 */
struct DiodeVCFWidget : LRModuleWidget {
    DiodeVCFWidget(DiodeVCF *module);
};


DiodeVCFWidget::DiodeVCFWidget(DiodeVCF *module) : LRModuleWidget(module) {
    panel = new LRPanel(-10, -10);
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/DiodeLadderVCF.svg")));
    addChild(panel);

    box.size = panel->box.size;

    panel->setInner(nvgRGBAf(0.1, 0.1, 0.f, 0.09f));
    panel->setOuter(nvgRGBAf(0.f, 0.f, 0.f, 0.28f));

    // ***** SCREWS **********
    /*   addChild(Widget::create<AlternateScrewLight>(Vec(15, 1)));
       addChild(Widget::create<AlternateScrewLight>(Vec(box.size.x - 30, 1)));
       addChild(Widget::create<AlternateScrewLight>(Vec(15, 366)));
       addChild(Widget::create<AlternateScrewLight>(Vec(box.size.x - 30, 366)));*/
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    module->frqKnob = LRKnob::create<LRAlternateBigLight>(Vec(149.4, 75.8), module, DiodeVCF::FREQUENCY_PARAM, 0.f, 1.f, 1.f);
    module->resKnob = LRKnob::create<LRAlternateMiddleLight>(Vec(159.2, 171.5), module, DiodeVCF::RES_PARAM, 0.0f, 1.0, 0.0f);
    module->saturateKnob = LRKnob::create<LRAlternateMiddleLight>(Vec(159.2, 251.5), module, DiodeVCF::SATURATE_PARAM, 0.f, 1.0, 0.0f);

    addParam(module->frqKnob);
    addParam(module->resKnob);
    addParam(module->saturateKnob);
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(ParamWidget::create<LRAlternateSmallLight>(Vec(84.9, 91.3), module, DiodeVCF::FREQUENCY_CV_PARAM, -1.f, 1.0f, 0.f));
    // ***** CV INPUTS *******


    // ***** INPUTS **********
    addInput(Port::create<LRIOPortBLight>(Vec(17.999, 326.05), Port::INPUT, module, DiodeVCF::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPortBLight>(Vec(58.544, 326.05), Port::OUTPUT, module, DiodeVCF::FILTER_OUTPUT));
    // ***** OUTPUTS *********

    // **** SETUP LCD ********
    module->lcd->box.pos = Vec(14, 155);
    module->lcd->format = "%4.3f Hz";
    addChild(module->lcd);
    // **** SETUP LCD ********

    addParam(ParamWidget::create<LRSwitch>(Vec(119, 331), module, DiodeVCF::MODE_SWITCH_PARAM, 0.0, 1.0, 1.0));


}


Model *modelDiodeVCF = Model::create<DiodeVCF, DiodeVCFWidget>("Lindenberg Research", "DIODE VCF", "Laika Diode-Ladder Filter", FILTER_TAG);
