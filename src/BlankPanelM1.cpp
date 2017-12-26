#include "LindenbergResearch.hpp"

struct BlankPanelM1 : Module {
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


    BlankPanelM1() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

    void step() override;
};


void BlankPanelM1::step() {
}


BlankPanelWidgetM1::BlankPanelWidgetM1() {
    BlankPanelM1 *module = new BlankPanelM1();

    setModule(module);
    box.size = Vec(BLANKPANEL_MARK_I_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelM1.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 365)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 365)));
    // ***** SCREWS **********
}
