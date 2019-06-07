#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;

struct ReShaperWidget;

struct ReShaper : LRModule {
    enum ParamIds {
        RESHAPER_AMOUNT,
        RESHAPER_CV_AMOUNT,
        NUM_PARAMS
    };

    enum InputIds {
        RESHAPER_INPUT,
        RESHAPER_CV_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        RESHAPER_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        NUM_LIGHTS
    };

    ReShaperWidget *reflect;

    ReShaper() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        configParam(RESHAPER_AMOUNT, 1.f, 50.f, 1.f);
        configParam(RESHAPER_CV_AMOUNT, 0.f, 5.f, 0.f);
    }


    void process(const ProcessArgs &args) override;
};


/**
 * @brief Reshaper Panel
 */
struct ReShaperWidget : LRModuleWidget {
    LRBigKnob *amountKnob;

    ReShaperWidget(ReShaper *module);
};


ReShaperWidget::ReShaperWidget(ReShaper *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/ReShaper.svg")));
    // panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/ReShaper.svg")));
    // panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/ReShaper.svg")));

    auto newGestalt = DARK;

    noVariants = true;
    panel->init();
    gestalt = newGestalt;
    addChild(panel);
    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;

    // ***** SCREWS **********
//    addChild(createWidget<ScrewLight>(Vec(15, 1)));
//    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
//    addChild(createWidget<ScrewLight>(Vec(15, 366)));
//    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    amountKnob = createParam<LRBigKnob>(Vec(32.7, 228), module, ReShaper::RESHAPER_AMOUNT);

    addParam(amountKnob);
    addParam(createParam<LRSmallKnob>(Vec(48.9, 126), module, ReShaper::RESHAPER_CV_AMOUNT));
    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(21.5, 52.3), module, ReShaper::RESHAPER_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(71.2, 52.3), module, ReShaper::RESHAPER_CV_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(46.2, 311.6), module, ReShaper::RESHAPER_OUTPUT));
    // ***** OUTPUTS *********
}


void ReShaper::process(const ProcessArgs &args) {
    // normalize signal input to [-1.0...+1.0]
    float x = clamp(inputs[RESHAPER_INPUT].getVoltage() * 0.1f, -1.f, 1.f);
    float cv = inputs[RESHAPER_CV_INPUT].getVoltage() * params[RESHAPER_CV_AMOUNT].getValue();
    float a = clamp(params[RESHAPER_AMOUNT].getValue() + cv, 1.f, 50.f);

    reflect->amountKnob->setIndicatorActive(inputs[RESHAPER_CV_INPUT].isConnected());
    reflect->amountKnob->setIndicatorValue((params[RESHAPER_AMOUNT].getValue() + cv) / 50.f);

    // do the acid!
    float out = x * (fabs(x) + a) / (x * x + (a - 1) * fabs(x) + 1);

    outputs[RESHAPER_OUTPUT].setVoltage(out * 5.0f);
}

Model *modelReShaper = createModel<ReShaper, ReShaperWidget>("ReShaper");
