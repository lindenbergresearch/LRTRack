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
        FREQUENCY_CV_PARAM,
        RESONANCE_CV_PARAM,
        SATURATE_CV_PARAM,
        MODE_SWITCH_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        FREQUCENCY_CV_INPUT,
        RESONANCE_CV_INPUT,
        SATURATE_CV_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        FILTER_OUTPUT,
        HP_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };


    DiodeVCF() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}


    void onRandomize() override;
    void updateComponents();

    LRLCDWidget *lcd = new LRLCDWidget(nvgRGBAf(0.2, 0.09, 0.03, 1.0), 12, "%00004.3f Hz", LRLCDWidget::NUMERIC);
    dsp::DiodeLadderFilter *lpf = new dsp::DiodeLadderFilter(engineGetSampleRate());
    dsp::Hardclip *hardclip = new dsp::Hardclip(engineGetSampleRate());

    LRAlternateBigLight *frqKnob = NULL;
    LRAlternateBigLight *resKnob = NULL;
    LRAlternateMiddleLight *saturateKnob = NULL;

    SVGWidget *patina;
    LRPanel *panel, *panelAged;
    bool aged = true;


    json_t *toJson() override {
        json_t *rootJ = json_object();
        json_object_set_new(rootJ, "aged", json_boolean(aged));
        return rootJ;
    }


    void fromJson(json_t *rootJ) override {
        json_t *agedJ = json_object_get(rootJ, "aged");
        if (agedJ)
            aged = json_boolean_value(agedJ);

        updateComponents();
    }


    void step() override;
    void onSampleRateChange() override;
};


void DiodeVCF::step() {
    float freqcv = 0, rescv = 0, satcv = 0;

    /* not connected */
    if (!inputs[FILTER_INPUT].active) {
        outputs[FILTER_OUTPUT].value = 0.f;

        return;
    }

    if (inputs[FREQUCENCY_CV_INPUT].active) {
        freqcv = inputs[FREQUCENCY_CV_INPUT].value / 10 * quadraticBipolar(params[FREQUENCY_CV_PARAM].value);
    }


    if (inputs[RESONANCE_CV_INPUT].active) {
        rescv = inputs[RESONANCE_CV_INPUT].value / 10 * quadraticBipolar(params[RESONANCE_CV_PARAM].value);
    }

    if (inputs[SATURATE_CV_INPUT].active) {
        satcv = inputs[SATURATE_CV_INPUT].value / 10 * quadraticBipolar(params[SATURATE_CV_PARAM].value);
    }

    float frq = clamp(params[FREQUENCY_PARAM].value + freqcv, 0.f, 1.f);
    float res = clamp((params[RES_PARAM].value + rescv) * 17.28f, 0.f, 17.28f);
    float sat = clamp(quarticBipolar((params[SATURATE_PARAM].value) + satcv) * 14 + 1, 0.f, 15.f);

    if (frqKnob != nullptr && resKnob != nullptr && saturateKnob != nullptr) {
        frqKnob->setIndicatorActive(inputs[FREQUCENCY_CV_INPUT].active);
        resKnob->setIndicatorActive(inputs[RESONANCE_CV_INPUT].active);
        saturateKnob->setIndicatorActive(inputs[SATURATE_CV_INPUT].active);

        frqKnob->setIndicatorValue(params[FREQUENCY_PARAM].value + freqcv);
        resKnob->setIndicatorValue(params[RES_PARAM].value + rescv);
        saturateKnob->setIndicatorValue(params[SATURATE_PARAM].value + satcv);
    }


    if (res < 0) {
        debug("%f", res);
    }

    lpf->setFrequency(frq);
    lpf->setResonance(res);
    lpf->setSaturation(sat);

    lpf->low = params[MODE_SWITCH_PARAM].value != 0;

    lcd->value = lpf->getFreqHz();

    lpf->setIn(inputs[FILTER_INPUT].value / 10.f);
    lpf->invalidate();
    lpf->process();

    outputs[HP_OUTPUT].value = lpf->out12db * 10.f;
    outputs[FILTER_OUTPUT].value = lpf->getOut() * 10.f;
}


void DiodeVCF::updateComponents() {
    patina->visible = aged;
    panelAged->visible = aged;
    panel->visible = !aged;

    panelAged->dirty = true;
    panel->dirty = true;
}


void DiodeVCF::onSampleRateChange() {
    Module::onSampleRateChange();
    lpf->setSamplerate(engineGetSampleRate());
}


void DiodeVCF::onRandomize() {
    Module::randomize();
    //  patina->box.pos = Vec(-randomUniform() * 1000, -randomUniform() * 200);
    panel->dirty = true;
}


/**
 * @brief Blank Panel with Logo
 */
struct DiodeVCFWidget : LRModuleWidget {
    DiodeVCFWidget(DiodeVCF *module);
    void appendContextMenu(Menu *menu) override;
};


DiodeVCFWidget::DiodeVCFWidget(DiodeVCF *module) : LRModuleWidget(module) {
    panel = new LRPanel(-10, -10);
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/DiodeLadderVCF.svg")));
    addChild(panel);

    module->panel = panel;

    module->panelAged = new LRPanel(-10, -10);
    module->panelAged->setBackground(SVG::load(assetPlugin(plugin, "res/panels/DiodeLadderVCFAged.svg")));
    module->panelAged->visible = false;
    addChild(module->panelAged);

    box.size = panel->box.size;

    module->patina = new SVGWidget();
    module->patina->setSVG(SVG::load(assetPlugin(plugin, "res/panels/LaikaPatina.svg")));
    module->panelAged->addChild(module->patina);

    panel->setInner(nvgRGBAf(0.3, 0.3, 0.f, 0.09f));
    panel->setOuter(nvgRGBAf(0.f, 0.f, 0.f, 0.63f));

    module->panelAged->setInner(nvgRGBAf(0.3, 0.3, 0.f, 0.09f));
    module->panelAged->setOuter(nvgRGBAf(0.f, 0.f, 0.f, 0.63f));

    // ***** SCREWS **********
    addChild(Widget::create<AlternateScrewLight>(Vec(15, 1)));
    addChild(Widget::create<AlternateScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<AlternateScrewLight>(Vec(15, 366)));
    addChild(Widget::create<AlternateScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    module->frqKnob = LRKnob::create<LRAlternateBigLight>(Vec(32.5, 74.4), module, DiodeVCF::FREQUENCY_PARAM, 0.f, 1.f, 1.f);
    module->resKnob = LRKnob::create<LRAlternateBigLight>(Vec(151.5, 74.4), module, DiodeVCF::RES_PARAM, 0.0f, 1.0, 0.0f);
    module->saturateKnob = LRKnob::create<LRAlternateMiddleLight>(Vec(99.5, 164.4), module, DiodeVCF::SATURATE_PARAM, 0.f, 1.0, 0.0f);

    module->frqKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->resKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
    module->saturateKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

    addParam(module->frqKnob);
    addParam(module->resKnob);
    addParam(module->saturateKnob);

    addParam(ParamWidget::create<LRAlternateSmallLight>(Vec(39.9, 251.4), module, DiodeVCF::FREQUENCY_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamWidget::create<LRAlternateSmallLight>(Vec(177, 251.4), module, DiodeVCF::RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(ParamWidget::create<LRAlternateSmallLight>(Vec(108.5, 251.4), module, DiodeVCF::SATURATE_CV_PARAM, -1.f, 1.0f, 0.f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addInput(Port::create<LRIOPortCLight>(Vec(37.4, 284.4), Port::INPUT, module, DiodeVCF::FREQUCENCY_CV_INPUT));
    addInput(Port::create<LRIOPortCLight>(Vec(175.3, 284.4), Port::INPUT, module, DiodeVCF::RESONANCE_CV_INPUT));
    addInput(Port::create<LRIOPortCLight>(Vec(106.4, 284.4), Port::INPUT, module, DiodeVCF::SATURATE_CV_INPUT));
    // ***** CV INPUTS *******


    // ***** INPUTS **********
    addInput(Port::create<LRIOPortBLight>(Vec(37.4, 325.5), Port::INPUT, module, DiodeVCF::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPortBLight>(Vec(175.3, 325.5), Port::OUTPUT, module, DiodeVCF::FILTER_OUTPUT));
    addOutput(Port::create<LRIOPortBLight>(Vec(145, 325.5), Port::OUTPUT, module, DiodeVCF::HP_OUTPUT));
    // ***** OUTPUTS *********

    /**** SETUP LCD ********
    module->lcd->box.pos = Vec(14, 155);
    module->lcd->format = "%4.3f Hz";
    addChild(module->lcd);
    // **** SETUP LCD ********/

    addParam(ParamWidget::create<LRSwitch>(Vec(121.1, 131.8), module, DiodeVCF::MODE_SWITCH_PARAM, 0.0, 1.0, 1.0));


}


struct DiodeVCFAged : MenuItem {
    DiodeVCF *diodeVCF;


    void onAction(EventAction &e) override {
        if (diodeVCF->aged) {
            diodeVCF->aged = false;
        } else {
            diodeVCF->aged = true;
        }

        diodeVCF->updateComponents();
    }


    void step() override {
        rightText = CHECKMARK(diodeVCF->aged);
    }
};


void DiodeVCFWidget::appendContextMenu(Menu *menu) {
    menu->addChild(MenuEntry::create());

    DiodeVCF *diodeVCF = dynamic_cast<DiodeVCF *>(module);
    assert(diodeVCF);

    DiodeVCFAged *mergeItemAged = MenuItem::create<DiodeVCFAged>("Aged");
    mergeItemAged->diodeVCF = diodeVCF;
    menu->addChild(mergeItemAged);
}


Model *modelDiodeVCF = Model::create<DiodeVCF, DiodeVCFWidget>("Lindenberg Research", "DIODE VCF", "Laika Diode-Ladder Filter", FILTER_TAG);
