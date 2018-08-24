#include "LindenbergResearch.hpp"

using namespace rack;
using namespace lrt;


struct QuickMix : Module {
    enum ParamIds {
        LEVEL1_PARAM,
        LEVEL2_PARAM,
        LEVEL3_PARAM,
        LEVEL4_PARAM,
        LEVEL5_PARAM,
        LEVEL6_PARAM,
        LEVELM_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        M1_INPUT,
        M2_INPUT,
        M6_INPUT,
        M3_INPUT,
        M4_INPUT,
        M5_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        MASTER_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    QuickMix() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

    void step() override;

};


void QuickMix::step() {

}


/**
 * @brief Blank Panel Small
 */
struct QuickMixWidget : LRModuleWidget {
    QuickMixWidget(QuickMix *module);
};


QuickMixWidget::QuickMixWidget(QuickMix *module) : LRModuleWidget(module) {
    panel = new LRPanel(0,-90);
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/QuickMix.svg")));
    addChild(panel);

    box.size = panel->box.size;


    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(13.4, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(13.4, 366)));
    // ***** SCREWS **********
}


Model *modelQuickMix = Model::create<QuickMix, QuickMixWidget>("Lindenberg Research", "QuickMixer", "Q: Quick Mixer", MIXER_TAG);
