#include "dsp/MS20zdf.hpp"
#include "LindenbergResearch.hpp"


struct MS20Filter : LRTModule {

    enum ParamIds {
        FREQUENCY_PARAM,
        PEAK_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        FILTER_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        LP_OUTPUT,
        HP_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {

        NUM_LIGHTS
    };

    dsp::MS20zdf *ms20zdf = new dsp::MS20zdf(engineGetSampleRate());
    LCDWidget *lcd1 = new LCDWidget(LCD_COLOR_FG, 15);


    MS20Filter() : LRTModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override;
    void onSampleRateChange() override;
};


void MS20Filter::step() {
    /*  float frqcv = inputs[CUTOFF_CV_INPUT].value * 0.1f * quadraticBipolar(params[CUTOFF_CV_PARAM].value);
      float rescv = inputs[RESONANCE_CV_INPUT].value * 0.1f * quadraticBipolar(params[RESONANCE_CV_PARAM].value);
      float drvcv = inputs[DRIVE_CV_INPUT].value * 0.1f * quadraticBipolar(params[DRIVE_CV_PARAM].value);

      filter.setFrequency(params[CUTOFF_PARAM].value + frqcv);
      filter.setResonance(params[RESONANCE_PARAM].value + rescv);
      filter.setDrive(params[DRIVE_PARAM].value + drvcv);
      filter.setSlope(params[SLOPE_PARAM].value);

      float y = inputs[FILTER_INPUT].value;

      filter.setIn(y);
      filter.process();

      outputs[LP_OUTPUT].value = filter.getLpOut();

      lights[OVERLOAD_LIGHT].value = filter.getLightValue();*/

    ms20zdf->setFrequency(params[FREQUENCY_PARAM].value);
    ms20zdf->setPeak(params[PEAK_PARAM].value);

    lcd1->text = stringf("%f", ms20zdf->getFrequency());

    ms20zdf->setIn(inputs[FILTER_INPUT].value);

    ms20zdf->process();

    outputs[LP_OUTPUT].value = ms20zdf->getLPOut();
    outputs[HP_OUTPUT].value = ms20zdf->getHPOut();
}


void MS20Filter::onSampleRateChange() {
    Module::onSampleRateChange();
    ms20zdf->updateSampleRate(engineGetSampleRate());
}


MS20FilterWidget::MS20FilterWidget() {
    MS20Filter *module = new MS20Filter();

    setModule(module);
    box.size = Vec(MS20_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/MS20.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 366)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(62, 150), module, MS20Filter::FREQUENCY_PARAM, 0.f, 1.f, 0.8f));
    addParam(createParam<LRMiddleKnob>(Vec(24, 229), module, MS20Filter::PEAK_PARAM, 0.f, 1.0, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    /* addParam(createParam<LRSmallKnob>(Vec(27.5, 106), module, AlmaFilter::RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f));
     addParam(createParam<LRSmallKnob>(Vec(78, 106), module, AlmaFilter::CUTOFF_CV_PARAM, -1.f, 1.f, 0.f));
     addParam(createParam<LRSmallKnob>(Vec(127.1, 106), module, AlmaFilter::DRIVE_CV_PARAM, -1.f, 1.f, 0.f));

     addInput(createInput<IOPort>(Vec(26, 50), module, AlmaFilter::RESONANCE_CV_INPUT));
     addInput(createInput<IOPort>(Vec(76, 50), module, AlmaFilter::CUTOFF_CV_INPUT));
     addInput(createInput<IOPort>(Vec(125, 50), module, AlmaFilter::DRIVE_CV_INPUT));*/
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(26, 326.5), module, MS20Filter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(125, 326.5), module, MS20Filter::LP_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(60, 326.5), module, MS20Filter::HP_OUTPUT));
    // ***** OUTPUTS *********

    // ***** LIGHTS **********
    // addChild(createLight<LRRedLight>(Vec(85, 247), module, AlmaFilter::OVERLOAD_LIGHT));
    // ***** LIGHTS **********

    // ***** LCD *************
    module->lcd1->box.pos = Vec(34, 115);
    addChild(module->lcd1);
    // ***** LCD *************

}
