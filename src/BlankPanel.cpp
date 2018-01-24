#include "LindenbergResearch.hpp"
#include "Release.h"


struct BlankPanel : Module {
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

    LCDWidget *lcd1 = new LCDWidget(LCD_COLOR_FG, 15);


    BlankPanel() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}


    void step() override;
};


void BlankPanel::step() {
}


BlankPanelWidget::BlankPanelWidget() {
    BlankPanel *module = new BlankPanel();

    setModule(module);
    box.size = Vec(BLANKPANEL_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanel.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 366)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** LCD *************
    module->lcd1->box.pos = Vec(34, 365);
    addChild(module->lcd1);
    module->lcd1->text = VERSION_STR;
    // ***** LCD *************
}
