#include "../dsp/DSPMath.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;


struct BlankPanelWood : LRModule {
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


    BlankPanelWood() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


};


struct BlankPanelWidgetWood : LRModuleWidget {
    SvgWidget *patina, *logoStamp;
    ScrewDarkB *screw1, *screw2;
    LRPanel *panel;

    bool aged = true;
    bool screws = true;
    bool logo = true;

    BlankPanelWidgetWood(BlankPanelWood *module);
    void appendContextMenu(Menu *menu) override;

    void randomizeAction();


    void updateComponents() {
        screw1->visible = screws;
        screw2->visible = screws;
        logoStamp->visible = logo;
        patina->visible = aged;
        panel->dirty = true;
    }


    json_t *toJson() override {
        json_t *rootJ = json_object();

        json_object_set_new(rootJ, "AGED", json_boolean(aged));
        json_object_set_new(rootJ, "screws", json_boolean(screws));
        json_object_set_new(rootJ, "logo", json_boolean(logo));
        return rootJ;
    }


    void fromJson(json_t *rootJ) override {
        LRModuleWidget::fromJson(rootJ);

        json_t *agedJ = json_object_get(rootJ, "AGED");
        if (agedJ)
            aged = json_boolean_value(agedJ);

        json_t *screwsJ = json_object_get(rootJ, "screws");
        if (screwsJ)
            screws = json_boolean_value(screwsJ);

        json_t *logoJ = json_object_get(rootJ, "logo");
        if (logoJ)
            logo = json_boolean_value(logoJ);

        //updateComponents();
    }
};


BlankPanelWidgetWood::BlankPanelWidgetWood(BlankPanelWood *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestalt::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/WoodLeftTop.svg")));
    // panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/WoodLeftTop.svg")));
    // panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/WoodLeftTop.svg")));

    noVariants = true;
    gestalt = LRGestalt::DARK;
    // patina = false;
    gradient = false;

    panel->init();
    addChild(panel);


    box.size = panel->box.size;

    auto gradientDark = new LRGradientWidget(box.size, nvgRGBAf(1.4f * .369f, 1.4f * 0.357f, 1.4f * 0.3333f, 0.05f),
                                             nvgRGBAf(0.f, 0.f, 0.f, 0.15f), Vec(-10, 10));
    gradientDark->visible = true;

    panel->addChild(gradientDark);

    patina = new SvgWidget();
    patina->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/WoodPatina.svg")));
    panel->addChild(patina);

    logoStamp = new SvgWidget();
    logoStamp->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/LogoSmallPlate.svg")));
    logoStamp->box.pos = Vec(8.5, 348.8);
    addChild(logoStamp);

    randomizeAction();


    // ***** SCREWS **********
    screw1 = createWidget<ScrewDarkB>(Vec(23, 6));
    addChild(screw1);

    screw2 = createWidget<ScrewDarkB>(Vec(23, box.size.y - 20));
    addChild(screw2);
    // ***** SCREWS **********
}


struct BlankPanelWoodAged : MenuItem {
    BlankPanelWidgetWood *blankPanelWood;


    void onAction(const event::Action &e) override {
        blankPanelWood->aged ^= true;
        blankPanelWood->updateComponents();
    }


    void step() override {
        rightText = CHECKMARK(blankPanelWood->aged);
    }
};


struct BlankPanelWoodScrews : MenuItem {
    BlankPanelWidgetWood *blankPanelWood;


    void onAction(const event::Action &e) override {
        blankPanelWood->screws ^= true;
        blankPanelWood->updateComponents();
    }


    void step() override {
        rightText = CHECKMARK(blankPanelWood->screws);
    }
};


struct BlankPanelWoodLogo : MenuItem {
    BlankPanelWidgetWood *blankPanelWood;


    void onAction(const event::Action &e) override {
        blankPanelWood->logo ^= true;
        blankPanelWood->updateComponents();
    }


    void step() override {
        rightText = CHECKMARK(blankPanelWood->logo);
    }
};


void BlankPanelWidgetWood::appendContextMenu(Menu *menu) {
    auto *pBlankPanelWidgetWood = dynamic_cast<BlankPanelWidgetWood *>(module);
    assert(pBlankPanelWidgetWood);

    auto *mergeItemAged = createMenuItem<BlankPanelWoodAged>("Use AGED look");
    mergeItemAged->blankPanelWood = pBlankPanelWidgetWood;
    menu->addChild(mergeItemAged);

    auto *mergeItemScrews = createMenuItem<BlankPanelWoodScrews>("Show Screws");
    mergeItemScrews->blankPanelWood = pBlankPanelWidgetWood;
    menu->addChild(mergeItemScrews);

    auto *mergeItemLogo = createMenuItem<BlankPanelWoodLogo>("Show Logo Plate");
    mergeItemLogo->blankPanelWood = pBlankPanelWidgetWood;
    menu->addChild(mergeItemLogo);
}


void BlankPanelWidgetWood::randomizeAction() {
    ModuleWidget::randomizeAction();
    patina->box.pos = Vec(-random::uniform() * 1000, -random::uniform() * 200);
}


Model *modelBlankPanelWood = createModel<BlankPanelWood, BlankPanelWidgetWood>("BlankPanel Wood");