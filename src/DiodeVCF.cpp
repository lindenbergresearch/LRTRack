#include "LindenbergResearch.hpp"
#include "dsp/DiodeLadder.hpp"

using namespace rack;
using namespace lrt;


struct DiodeVCF : Module {
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


    DiodeVCF() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}


    void step() override;
};


void DiodeVCF::step() {

}


/**
 * @brief Blank Panel with Logo
 */
struct DiodeVCFWidget : LRModuleWidget {
    DiodeVCFWidget(DiodeVCF *module);
};


DiodeVCFWidget::DiodeVCFWidget(DiodeVCF *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/DiodeVCF.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********


}


Model *modelDiodeVCF = Model::create<DiodeVCF, DiodeVCFWidget>("Lindenberg Research", "DIODE VCF", "Laika DiodeLadder ", BLANK_TAG);
