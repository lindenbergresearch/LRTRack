#include "dsp/LadderFilter.hpp"
#include "LindenbergResearch.hpp"


struct SimpleFilter : LRTModule {

    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        DRIVE_PARAM,
        CUTOFF_CV_PARAM,
        RESONANCE_CV_PARAM,
        DRIVE_CV_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        FILTER_INPUT,
        CUTOFF_CV_INPUT,
        RESONANCE_CV_INPUT,
        DRIVE_CV_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        LP_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        NUM_LIGHTS
    };

    LadderFilter filter;


    SimpleFilter() : LRTModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override;
};


void SimpleFilter::step() {
    float frqcv = inputs[CUTOFF_CV_INPUT].value * 0.1f * quadraticBipolar(params[CUTOFF_CV_PARAM].value);
    float rescv = inputs[RESONANCE_CV_INPUT].value * 0.1f * quadraticBipolar(params[RESONANCE_CV_PARAM].value);
    float drvcv = inputs[DRIVE_CV_INPUT].value * 0.1f * quadraticBipolar(params[DRIVE_CV_PARAM].value);

    filter.setFrequency(params[CUTOFF_PARAM].value + frqcv);
    filter.setResonance(params[RESONANCE_PARAM].value + rescv);
    filter.setDrive(params[DRIVE_PARAM].value + drvcv);

    float y = fastatan(inputs[FILTER_INPUT].value / 20.f);

    filter.setIn(y);
    filter.process();

    outputs[LP_OUTPUT].value = filter.getLpOut() * 20.f;
}


SimpleFilterWidget::SimpleFilterWidget() {
    SimpleFilter *module = new SimpleFilter();

    setModule(module);
    box.size = Vec(FILTER_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/VCF.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 366)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(62.5, 150.954), module, SimpleFilter::CUTOFF_PARAM, 0.f, 1.f, 0.f));
    addParam(createParam<LRMiddleKnob>(Vec(32.69, 233.872), module, SimpleFilter::RESONANCE_PARAM, -0.f, 1.5, 0.0f));
    addParam(createParam<LRMiddleKnob>(Vec(107.309, 233.872), module, SimpleFilter::DRIVE_PARAM, 0.0f, 1.f, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(27, 106), module, SimpleFilter::RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(78, 106), module, SimpleFilter::CUTOFF_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(127, 106), module, SimpleFilter::DRIVE_CV_PARAM, -1.f, 1.f, 0.f));

    addInput(createInput<IOPort>(Vec(24, 52), module, SimpleFilter::RESONANCE_CV_INPUT));
    addInput(createInput<IOPort>(Vec(74, 52), module, SimpleFilter::CUTOFF_CV_INPUT));
    addInput(createInput<IOPort>(Vec(123, 52), module, SimpleFilter::DRIVE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(23, 326), module, SimpleFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(129, 326), module, SimpleFilter::LP_OUTPUT));
    // ***** OUTPUTS *********
}
