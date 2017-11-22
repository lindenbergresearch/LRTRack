#include "VCO.hpp"
#include "dsp/helper.hpp"
#include "LindenbergResearch.hpp"

struct VCO : Module {
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

    VCO() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

    void step() override;
};


void VCO::step() {
    float inc = getPhaseIncrement(440.f);
}


VCOWidget::VCOWidget() {
    VCO *module = new VCO();

    setModule(module);
    box.size = Vec(MODULE_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/VCO.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 365)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 365)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    //  addParam(createParam<LRBigKnobWhite>(Vec(35, 216), module, VCO::RESHAPER_AMOUNT, 1.f, 50.f, 1.f));
    //  addParam(createParam<LRBasicKnobWhite>(Vec(44, 120), module, VCO::RESHAPER_CV_AMOUNT, 0.f, 5.f, 0.f));
    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    //  addInput(createInput<IOPort>(Vec(21, 60), module, VCO::RESHAPER_INPUT));
    //  addInput(createInput<IOPort>(Vec(71, 60), module, VCO::RESHAPER_CV_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    //  addOutput(createOutput<IOPort>(Vec(46, 320), module, VCO::RESHAPER_OUTPUT));
    // ***** OUTPUTS *********

}
