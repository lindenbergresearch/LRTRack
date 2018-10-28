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


    void step() override;
};


void BlankPanel::step() {

}


/**
 * @brief Blank Panel with Logo
 */
struct BlankPanelWidget : LRModuleWidget {
    BlankPanelWidget(BlankPanel *module);
};


BlankPanelWidget::BlankPanelWidget(BlankPanel *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setupGestalt(&gestalt, &gradient, &patina);

    panel->addSVGVariant(LRGestalt::DARK, SVG::load(assetPlugin(plugin, "res/panels/BlankPanel.svg")));
    panel->addSVGVariant(LRGestalt::LIGHT, SVG::load(assetPlugin(plugin, "res/panels/BlankPanelLight.svg")));
    panel->addSVGVariant(LRGestalt::AGED, SVG::load(assetPlugin(plugin, "res/panels/BlankPanelAged.svg")));

    gestalt = LRGestalt::LIGHT;
    noGestalt = true;

    panel->init();

    addChild(panel);

    box.size = panel->box.size;

    panel->patinaWidgetClassic->strength = .5f;
    panel->patinaWidgetWhite->strength = .5f;

    panel->gradients[DARK]->setInnerColor(nvgRGBAf(0.5, 0.5, 0.64f, 0.1f));
    panel->gradients[DARK]->setOuterColor(nvgRGBAf(0.f, 0.f, 0.f, 0.3f));

    panel->gradients[DARK]->setGradientOffset(Vec(0, 0), Vec(box.size.x * 0.8f, box.size.y * 0.8f));

    //panel->gradients[LIGHT] = new LRGradientWidget(box.size, nvgRGBAf(0.3, 0.3, 0.f, 0.09f), nvgRGBAf(0.f, 0.f, 0.f, 0.7f), Vec(-10,
    // -10));

    /* panel->setInner(nvgRGBAf(1.4f * .369f, 1.4f * 0.357f, 1.5f * 0.3333f, 0.17f));
     panel->setOuter(nvgRGBAf(0.f, 0.f, 0.f, 0.15f));*/

    float speed = 0.007;

    addChild(SVGRotator::create(Vec(105.5, 55), SVG::load(assetPlugin(plugin, "res/elements/CogBig.svg")), speed, 0.7, 0.4));
    addChild(SVGRotator::create(Vec(139, 43.7), SVG::load(assetPlugin(plugin, "res/elements/CogMiddle.svg")), speed * 1.9f, 0.7, 0.4));
    addChild(SVGRotator::create(Vec(120, 40), SVG::load(assetPlugin(plugin, "res/elements/CogSmall.svg")), -speed * 1.3f, 0.7, 0.4));


    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********
}


Model *modelBlankPanel = Model::create<BlankPanel, BlankPanelWidget>("Lindenberg Research", "BlankPanel 01", "Blank: Logo", BLANK_TAG);
