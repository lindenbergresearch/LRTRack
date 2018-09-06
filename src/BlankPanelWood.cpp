#include "dsp/DSPMath.hpp"
#include "LindenbergResearch.hpp"

using namespace rack;
using namespace lrt;


struct BlankPanelWood : Module {
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


    BlankPanelWood() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    SVGWidget *patina;

    void step() override;
    void randomize() override;
};


void BlankPanelWood::step() {
}


void BlankPanelWood::randomize() {
    Module::randomize();
    patina->box.pos = Vec(-randomUniform() * 1000, -randomUniform() * 200);
}


struct BlankPanelWidgetWood : LRModuleWidget {
    BlankPanelWidgetWood(BlankPanelWood *module);
};


BlankPanelWidgetWood::BlankPanelWidgetWood(BlankPanelWood *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/WoodLeftTop.svg")));
    addChild(panel);

    box.size = panel->box.size;

    panel->setInner(nvgRGBAf(1.5f * .369f, 1.5f * 0.357f, 1.5f * 0.3333f, 0.05f));
    panel->setOuter(nvgRGBAf(0.f, 0.f, 0.f, 0.15f));

    module->patina = new SVGWidget();
    module->patina->setSVG(SVG::load(assetPlugin(plugin, "res/WoodPatina.svg")));

    module->randomize();
    panel->addChild(module->patina);

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(23, 6)));
    //  addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(23, box.size.y - 20)));
    //  addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********
}


Model *modelBlankPanelWood = Model::create<BlankPanelWood, BlankPanelWidgetWood>(
        "Lindenberg Research",
        "BlankPanel Wood",
        "Blank: Wood ",
        BLANK_TAG);