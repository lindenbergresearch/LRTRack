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

struct BlankPanelSmall : Module {
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


    BlankPanelSmall() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}


    void step() override;
};


void BlankPanelSmall::step() {
}

/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelWidgetM1 : LRModuleWidget {
    BlankPanelWidgetM1(BlankPanelM1 *module);
};

/**
 * @brief Blank Panel Small
 */
struct BlankPanelWidgetSmall : LRModuleWidget {
    BlankPanelWidgetSmall(BlankPanelSmall *module);
};

BlankPanelWidgetM1::BlankPanelWidgetM1(BlankPanelM1 *module) : LRModuleWidget(module) {
    // setPanel(SVG::load(assetPlugin(plugin, "res/BlankPanelM1.svg")));

    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelM1.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********
}

BlankPanelWidgetSmall::BlankPanelWidgetSmall(BlankPanelSmall *module) : LRModuleWidget(module) {
    // setPanel(SVG::load(assetPlugin(plugin, "res/BlankPanelM1.svg")));

    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelSmall.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(25, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(25, 366)));
    // ***** SCREWS **********
}

Model *modelBlankPanelM1 = Model::create<BlankPanelM1, BlankPanelWidgetM1>("Lindenberg Research", "BlankPanel 02", "Blank Panel 12TE",
                                                                           BLANK_TAG);

Model *modelBlankPanelSmall = Model::create<BlankPanelSmall, BlankPanelWidgetSmall>("Lindenberg Research", "BlankPanel Small", "Blank Panel Small",
                                                                           BLANK_TAG);
