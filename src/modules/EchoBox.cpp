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

#include "../dsp/DSPMath.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;


struct EchoBox : LRModule {
    enum ParamIds {
        FREQUENCY_PARAM,
        RES_PARAM,
        SATURATE_PARAM,
        FREQUENCY_CV_PARAM,
        RESONANCE_CV_PARAM,
        SATURATE_CV_PARAM,
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
        LP_OUTPUT,
        HP_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };


    EchoBox() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        // frqKnob = LRKnob::create<LRBigKnob>(Vec(32.5, 74.4), this, DiodeVCF::FREQUENCY_PARAM, 0.f, 1.f, 1.f);
        // resKnob = LRKnob::create<LRBigKnob>(Vec(151.5, 74.4), this, DiodeVCF::RES_PARAM, 0.0f, 1.0, 0.0f);
        // saturateKnob = LRKnob::create<LRMiddleKnob>(Vec(99.5, 164.4), this, DiodeVCF::SATURATE_PARAM, 0.f, 1.0, 0.0f);
    }


    void onRandomize() override;
    void updateComponents();


    LRPanel *panel;


    void step() override;
    void onSampleRateChange() override;
};


void EchoBox::step() {

}


void EchoBox::updateComponents() {

}


void EchoBox::onSampleRateChange() {
    Module::onSampleRateChange();
}


void EchoBox::onRandomize() {
    Module::randomize();
    updateComponents();
}


/**
 * @brief Blank Panel with Logo
 */
struct EchoBoxWidget : LRModuleWidget {
    EchoBoxWidget(EchoBox *module);
    // void appendContextMenu(Menu *menu) override;
};


EchoBoxWidget::EchoBoxWidget(EchoBox *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestalt::DARK, SVG::load(assetPlugin(plugin, "res/panels/EchoBox.svg")));
    // panel->addSVGVariant(LRGestalt::LIGHT, SVG::load(assetPlugin(plugin, "res/panels/DiodeLadderVCF.svg")));
    //  panel->addSVGVariant(LRGestalt::AGED, SVG::load(assetPlugin(plugin, "res/panels/DiodeLadderVCFAged.svg")));

    panel->init();
    addChild(panel);

    module->panel = panel;
    box.size = panel->box.size;

    // ***** SCREWS **********
    /* addChild(Widget::create<ScrewLight>(Vec(15, 1)));
     addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 1)));
     addChild(Widget::create<ScrewLight>(Vec(15, 366)));
     addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 366)));*/
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    /*  module->frqKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
      module->resKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
      module->saturateKnob->setIndicatorColors(nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));

      addParam(module->frqKnob);
      addParam(module->resKnob);
      addParam(module->saturateKnob);

      addParam(ParamWidget::create<LRSmallKnob>(Vec(39.9, 251.4), module, DiodeVCF::FREQUENCY_CV_PARAM, -1.f, 1.0f, 0.f));
      addParam(ParamWidget::create<LRSmallKnob>(Vec(177, 251.4), module, DiodeVCF::RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f));
      addParam(ParamWidget::create<LRSmallKnob>(Vec(108.5, 251.4), module, DiodeVCF::SATURATE_CV_PARAM, -1.f, 1.0f, 0.f));*/
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    //  addInput(Port::create<LRIOPortCV>(Vec(37.4, 284.4), Port::INPUT, module, DiodeVCF::FREQUCENCY_CV_INPUT));
    //  addInput(Port::create<LRIOPortCV>(Vec(175.3, 284.4), Port::INPUT, module, DiodeVCF::RESONANCE_CV_INPUT));
    //  addInput(Port::create<LRIOPortCV>(Vec(106.4, 284.4), Port::INPUT, module, DiodeVCF::SATURATE_CV_INPUT));
    // ***** CV INPUTS *******


    // ***** INPUTS **********
    //  addInput(Port::create<LRIOPortAudio>(Vec(37.4, 318.5), Port::INPUT, module, DiodeVCF::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    // addOutput(Port::create<LRIOPortAudio>(Vec(175.3, 318.5), Port::OUTPUT, module, DiodeVCF::LP_OUTPUT));
    //  addOutput(Port::create<LRIOPortAudio>(Vec(106.4, 318.5), Port::OUTPUT, module, DiodeVCF::HP_OUTPUT));
    // ***** OUTPUTS *********
}


/*
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
*/
/*
struct DiodeVCFHiDef : MenuItem {
    DiodeVCF *diodeVCF;


    void onAction(EventAction &e) override {
        if (diodeVCF->hidef) {
            diodeVCF->hidef = false;
        } else {
            diodeVCF->hidef = true;
        }

        diodeVCF->updateComponents();
    }


    void step() override {
        rightText = CHECKMARK(diodeVCF->hidef);
    }
};


void DiodeVCFWidget::appendContextMenu(Menu *menu) {
    menu->addChild(MenuEntry::create());

    DiodeVCF *diodeVCF = dynamic_cast<DiodeVCF *>(module);
    assert(diodeVCF);


    DiodeVCFHiDef *mergeItemHiDef = MenuItem::create<DiodeVCFHiDef>("Use 4x oversampling");
    mergeItemHiDef->diodeVCF = diodeVCF;
    menu->addChild(mergeItemHiDef);
}*/


Model *modelEchoBox = Model::create<EchoBox, EchoBoxWidget>("Lindenberg Research", "ECHOBOX DELAY", "))) echoBOX analog delay processor",
                                                            DELAY_TAG);
