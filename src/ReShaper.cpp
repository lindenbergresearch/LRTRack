#include "LindenbergResearch.hpp"

struct ReShaper : Module {
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

    ReShaper() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

    void step() override;
};


void ReShaper::step() {
    // normalize signal input to [-1.0...+1.0]
    float x = clampf(inputs[RESHAPER_INPUT].value * 0.1f, -1.f, 1.f);
    float cv = inputs[RESHAPER_CV_INPUT].value * params[RESHAPER_CV_AMOUNT].value;
    float a = clampf(params[RESHAPER_AMOUNT].value + cv, 1.f, 50.f);

    // do the acid!
    float out = x * (fabs(x) + a) / (x * x + (a - 1) * fabs(x) + 1);
  
    outputs[RESHAPER_OUTPUT].value = out * 5.0f;
}


ReShaperWidget::ReShaperWidget() {
    ReShaper *module = new ReShaper();

    setModule(module);
    box.size = Vec(RESHAPER_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/ReShaper.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 366)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(32, 216), module, ReShaper::RESHAPER_AMOUNT, 1.f, 50.f, 1.f));
    addParam(createParam<LRSmallKnob>(Vec(48, 126), module, ReShaper::RESHAPER_CV_AMOUNT, 0.f, 5.f, 0.f));
    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(21, 60), module, ReShaper::RESHAPER_INPUT));
    addInput(createInput<IOPort>(Vec(71, 60), module, ReShaper::RESHAPER_CV_INPUT));   
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(46, 320), module, ReShaper::RESHAPER_OUTPUT));
    // ***** OUTPUTS *********
}
