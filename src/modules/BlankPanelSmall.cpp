#include "../dsp/DSPMath.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;
using std::vector;

struct BlankPanelWidgetSmall;


struct BlankPanelSmall : LRModule {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        M1_INPUT,
        M2_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        M1_OUTPUT,
        M2_OUTPUT,
        M3_OUTPUT,
        M4_OUTPUT,
        M5_OUTPUT,
        M6_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };


    BlankPanelWidgetSmall *reflect;


    BlankPanelSmall() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    bool multiple = false;

    void process(const ProcessArgs &args) override;


    void onReset() override {
        multiple = false;
    }

};


/**
 * @brief Blank Panel Small
 */
struct BlankPanelWidgetSmall : LRModuleWidget {
    vector<LRIOPortCV *> ioports;

    BlankPanelWidgetSmall(BlankPanelSmall *module);
    void appendContextMenu(Menu *menu) override;


    inline void showPorts() {
        /* set all to visible */
        for (int i = 0; i < 8; i++) {
            ioports[i]->visible = true;
        }
    }


    inline void hidePorts() {
        /* set all to invisible */
        for (int i = 0; i < 8; i++) {
            ioports[i]->visible = false;
        }
    }


    json_t *toJson() override;
    void fromJson(json_t *rootJ) override;
};


BlankPanelWidgetSmall::BlankPanelWidgetSmall(BlankPanelSmall *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelSmall.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelSmallLight.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/BlankPanelSmallAged.svg")));

    panel->init();
    addChild(panel);
    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;

    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(23.4, 1)));
    addChild(createWidget<ScrewLight>(Vec(23.4, 366)));
    // ***** SCREWS **********

    ioports.resize(8);

    /* INPUTS */
    ioports[0] = createInput<LRIOPortCV>(Vec(16.5, 19.5), module, BlankPanelSmall::M1_INPUT);
    ioports[1] = createInput<LRIOPortCV>(Vec(16.5, 228.5), module, BlankPanelSmall::M2_INPUT);

    /* OUTPUTS */
    ioports[2] = createOutput<LRIOPortCV>(Vec(16.5, 53.5), module, BlankPanelSmall::M1_OUTPUT);
    ioports[3] = createOutput<LRIOPortCV>(Vec(16.5, 87.5), module, BlankPanelSmall::M2_OUTPUT);
    ioports[4] = createOutput<LRIOPortCV>(Vec(16.5, 120.5), module, BlankPanelSmall::M3_OUTPUT);
    ioports[5] = createOutput<LRIOPortCV>(Vec(16.5, 262.5), module, BlankPanelSmall::M4_OUTPUT);
    ioports[6] = createOutput<LRIOPortCV>(Vec(16.5, 296.5), module, BlankPanelSmall::M5_OUTPUT);
    ioports[7] = createOutput<LRIOPortCV>(Vec(16.5, 329.5), module, BlankPanelSmall::M6_OUTPUT);

    hidePorts();

    // ***** IO-PORTS **********
    addInput(ioports[0]);
    addInput(ioports[1]);

    addOutput(ioports[2]);
    addOutput(ioports[3]);
    addOutput(ioports[4]);
    addOutput(ioports[5]);
    addOutput(ioports[6]);
    addOutput(ioports[7]);
    // ***** IO-PORTS **********
}


struct BlankPanelMultiple : MenuItem {
    BlankPanelSmall *blankPanelSmall;


    void onAction(const event::Action &e) override {
        if (blankPanelSmall->multiple) {
            for (int i = 0; i < 2; i++) {
                if (blankPanelSmall->inputs[i].isConnected()) {
                    blankPanelSmall->multiple = true;
                    return;
                }
            }

            for (int i = 0; i < 6; i++) {
                if (blankPanelSmall->outputs[i].isConnected()) {
                    blankPanelSmall->multiple = true;
                    return;
                }

                blankPanelSmall->multiple = false;
            }
        } else {
            blankPanelSmall->multiple = true;
        }
    }


    void step() override {
        rightText = CHECKMARK(blankPanelSmall->multiple);
    }
};


void BlankPanelWidgetSmall::appendContextMenu(Menu *menu) {
    BlankPanelSmall *blankPanelSmall = dynamic_cast<BlankPanelSmall *>(module);
    assert(blankPanelSmall);

    BlankPanelMultiple *mergeItem = createMenuItem<BlankPanelMultiple>("Dual Multiple");
    mergeItem->blankPanelSmall = blankPanelSmall;
    menu->addChild(mergeItem);
}


json_t *BlankPanelWidgetSmall::toJson() {
    json_t *rootJ = LRModuleWidget::toJson();

    BlankPanelSmall *blankPanelSmall = dynamic_cast<BlankPanelSmall *>(module);

    json_object_set_new(rootJ, "multiple", json_boolean(blankPanelSmall->multiple));
    return rootJ;
}


void BlankPanelWidgetSmall::fromJson(json_t *rootJ) {
    BlankPanelSmall *blankPanelSmall = dynamic_cast<BlankPanelSmall *>(module);

    LRModuleWidget::fromJson(rootJ);

    json_t *multJ = json_object_get(rootJ, "multiple");

    if (multJ)
        blankPanelSmall->multiple = json_boolean_value(multJ);
}


void BlankPanelSmall::process(const ProcessArgs &args) {

    for (int i = 0; i < 8; i++) {
        if (reflect->ioports[i] == nullptr) return;
    }

    if (multiple) {
        if (!reflect->ioports[0]->visible) {
            reflect->showPorts();
        }

        if (inputs[M1_INPUT].isConnected()) {
            float sig = inputs[M1_INPUT].getVoltage();
            outputs[M1_OUTPUT].setVoltage(sig);
            outputs[M2_OUTPUT].setVoltage(sig);
            outputs[M3_OUTPUT].setVoltage(sig);
        }


        if (inputs[M2_INPUT].isConnected()) {
            float sig = inputs[M2_INPUT].getVoltage();
            outputs[M4_OUTPUT].setVoltage(sig);
            outputs[M5_OUTPUT].setVoltage(sig);
            outputs[M6_OUTPUT].setVoltage(sig);
        }
    } else {
        if (reflect->ioports[0]->visible) {
            reflect->hidePorts();
        }
    }
}


Model *modelBlankPanelSmall = createModel<BlankPanelSmall, BlankPanelWidgetSmall>("BlankPanel Small");