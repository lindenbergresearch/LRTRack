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


    FontIconWidget *icons;

    void step() override;
};


void BlankPanelEmpty::step() {
}


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelEmptyWidget : LRModuleWidget {
    ModuleResizeWidget *resizeWidget, *resizeWidgetRight;
    ScrewDarkA *screw1, *screw2;

    BlankPanelEmptyWidget(BlankPanelEmpty *module);

    void step() override;
};


BlankPanelEmptyWidget::BlankPanelEmptyWidget(BlankPanelEmpty *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setupGestalt(&gestalt, &gradient, &patina);

    panel->addSVGVariant(LRGestalt::DARK, SVG::load(assetPlugin(plugin, "res/panels/BlankPanelM1.svg")));
    // panel->addSVGVariant(SVG::load(assetPlugin(plugin, "res/panels/BlankPanelM1.svg")));
    // panel->addSVGVariant(SVG::load(assetPlugin(plugin, "res/panels/BlankPanelM1.svg")));

    panel->init();
    addChild(panel);

    module->icons = new FontIconWidget(60.f);
    // addChild(module->icons);

    box.size = panel->box.size;

    /*resizeWidget = new ModuleResizeWidget(box.size.x);
    resizeWidgetRight = new ModuleResizeWidget(box.size.x);
    resizeWidgetRight->right = true;
    addChild(resizeWidget);
    addChild(resizeWidgetRight);*/


    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));

    screw1 = Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1));
    screw2 = Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366));

    addChild(screw1);
    addChild(screw2);
    // ***** SCREWS **********
}


void BlankPanelEmptyWidget::step() {
    /* panel->box.size = box.size;
     resizeWidgetRight->box.pos.x = box.size.x - resizeWidgetRight->box.size.x;
     resizeWidgetRight->box.pos.y = box.size.y - resizeWidgetRight->box.size.y;
     resizeWidget->box.pos.y = box.size.y - resizeWidget->box.size.y;

     screw1->box.pos.x = box.size.x - 30;
     screw2->box.pos.x = box.size.x - 30;*/

    ModuleWidget::step();
}


Model *modelBlankPanelEmpty = Model::create<BlankPanelEmpty, BlankPanelEmptyWidget>(
        "Lindenberg Research",
        "BlankPanel 02",
        "Blank: Empty",
        BLANK_TAG);


