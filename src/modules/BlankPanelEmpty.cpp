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

    #ifdef LRT_DEBUG
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelDebugger.svg")));
    noVariants = false;
    gestalt = DARK;
    patina = false;
    gradient = false;
    #else
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1.svg")));
    #endif
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Light.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelM1Aged.svg")));

    panel->init();
    addChild(panel);

    //module->icons = new FontIconWidget(60.f);
    //addChild(module->icons);

    box.size = panel->box.size;

    #ifdef LRT_DEBUG
    auto *iw = new InformationWidget();
    iw->box = box;
    addChild(iw);
    #endif
    /* resizeWidget = new ModuleResizeWidget(box.size.x);
     resizeWidgetRight = new ModuleResizeWidget(box.size.x);
     resizeWidgetRight->right = true;
     addChild(resizeWidget);
     addChild(resizeWidgetRight);*/


    // ***** SCREWS **********
    panel->addChild(createWidget<ScrewLight>(Vec(15, 1)));
    panel->addChild(createWidget<ScrewLight>(Vec(15, 366)));

    screw1 = createWidget<ScrewLight>(Vec(box.size.x - 30, 1));
    screw2 = createWidget<ScrewLight>(Vec(box.size.x - 30, 366));

    panel->addChild(screw1);
    panel->addChild(screw2);
    // ***** SCREWS **********
}


Model *modelBlankPanelEmpty = createModel<BlankPanelEmpty, BlankPanelEmptyWidget>("BlankPanel_02");


