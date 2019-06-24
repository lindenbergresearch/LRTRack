#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;


struct BlankPanel : LRModule {
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


    BlankPanel() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
};


/**
 * @brief Blank Panel with Logo
 */
struct BlankPanelWidget : LRModuleWidget {
    BlankPanelWidget(BlankPanel *module);
};


BlankPanelWidget::BlankPanelWidget(BlankPanel *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelLight.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelLight.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelLight.svg")));

    gestalt = LRGestaltType::LIGHT;

    panel->init();
    addChild(panel);
    box.size = panel->box.size;


    float speed = 0.007;

    addChild(SVGRotator::create(Vec(105.5, 55), APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/CogBig.svg")), speed, 0.7, 0.9));
    addChild(SVGRotator::create(Vec(139, 43.7), APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/CogMiddle.svg")), speed * 1.9f, 0.7, 0.9));
    addChild(SVGRotator::create(Vec(120, 40), APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/CogSmall.svg")), -speed * 1.3f, 0.7, 0.9));


    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(15, 1)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(createWidget<ScrewLight>(Vec(15, 366)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********
}


Model *modelBlankPanel = createModel<BlankPanel, BlankPanelWidget>("BlankPanel_01");
