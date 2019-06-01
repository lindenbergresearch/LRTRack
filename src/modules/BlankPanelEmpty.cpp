#include "../dsp/DSPMath.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;


struct BlankPanelEmpty : LRModule {
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


    BlankPanelEmpty() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    }


    void process(const ProcessArgs &args) override;
};


void BlankPanelEmpty::process(const ProcessArgs &args) {
}


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelEmptyWidget : LRModuleWidget {
    ScrewLight *screw1, *screw2;

    BlankPanelEmptyWidget(BlankPanelEmpty *module);
};


BlankPanelEmptyWidget::BlankPanelEmptyWidget(BlankPanelEmpty *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Light.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Aged.svg")));

    panel->init();
    addChild(panel);

    //module->icons = new FontIconWidget(60.f);
    //addChild(module->icons);

    box.size = panel->box.size;

    /* resizeWidget = new ModuleResizeWidget(box.size.x);
     resizeWidgetRight = new ModuleResizeWidget(box.size.x);
     resizeWidgetRight->right = true;
     addChild(resizeWidget);
     addChild(resizeWidgetRight);*/


    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(15, 1)));
    addChild(createWidget<ScrewLight>(Vec(15, 366)));

    screw1 = createWidget<ScrewLight>(Vec(box.size.x - 30, 1));
    screw2 = createWidget<ScrewLight>(Vec(box.size.x - 30, 366));

    addChild(screw1);
    addChild(screw2);
    // ***** SCREWS **********
}


Model *modelBlankPanelEmpty = createModel<BlankPanelEmpty, BlankPanelEmptyWidget>("BlankPanel_02");


