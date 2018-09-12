#include "dsp/DSPMath.hpp"
#include "LindenbergResearch.hpp"

using namespace rack;
using namespace lrt;


struct BlankPanelEmpty : Module {
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


    BlankPanelEmpty() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {
        icons = new FontIconWidget();
    }


    FontIconWidget *icons;

    void step() override;
};


void BlankPanelEmpty::step() {
}


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelEmptyWidget : LRModuleWidget {
    BlankPanelEmptyWidget(BlankPanelEmpty *module);
};


BlankPanelEmptyWidget::BlankPanelEmptyWidget(BlankPanelEmpty *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/panels/BlankPanelM1.svg")));
    addChild(panel);

    addChild(module->icons);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********
}


Model *modelBlankPanelEmpty = Model::create<BlankPanelEmpty, BlankPanelEmptyWidget>(
        "Lindenberg Research",
        "BlankPanel 02",
        "Blank: Empty",
        BLANK_TAG);


