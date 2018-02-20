#include "dsp/MS20zdf.hpp"
#include "LindenbergResearch.hpp"


struct MS20Filter : LRTModule {

    enum ParamIds {
        FREQUENCY_PARAM,
        PEAK_PARAM,
        DRIVE_PARAM,
        CUTOFF_CV_PARAM,
        PEAK_CV_PARAM,
        GAIN_CV_PARAM,
        MODE_SWITCH_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        FILTER_INPUT,
        CUTOFF_CV_INPUT,
        PEAK_CV_INPUT,
        GAIN_CV_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        FILTER_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        OVERLOAD_LIGHT,
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


    float frqcv = inputs[CUTOFF_CV_INPUT].value * 0.1f * quadraticBipolar(params[CUTOFF_CV_PARAM].value);
    float peakcv = inputs[PEAK_CV_INPUT].value * 0.1f * quadraticBipolar(params[PEAK_CV_PARAM].value);
    float gaincv = inputs[GAIN_CV_INPUT].value * 0.1f * quadraticBipolar(params[GAIN_CV_PARAM].value);

    ms20zdf->setFrequency(params[FREQUENCY_PARAM].value + frqcv);
    ms20zdf->setPeak(params[PEAK_PARAM].value + peakcv);
    ms20zdf->setDrive(params[DRIVE_PARAM].value + gaincv);

    ms20zdf->setType(params[MODE_SWITCH_PARAM].value);

    // lcd1->text = stringf("%.1f %.4f", ms20zdf->getFrequencyHz(), ms20zdf->getPeak());

    ms20zdf->setIn(inputs[FILTER_INPUT].value);

    ms20zdf->process();

    outputs[FILTER_OUTPUT].value = ms20zdf->getLPOut();
    //lights[OVERLOAD_LIGHT].value = ms20zdf->getLPOut() / 100.f;
}


void MS20Filter::onSampleRateChange() {
    Module::onSampleRateChange();
    ms20zdf->updateSampleRate(engineGetSampleRate());
}


MS20FilterWidget::MS20FilterWidget() {
    auto *module = new MS20Filter();
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
    addParam(createParam<LRBigKnob>(Vec(102, 65), module, MS20Filter::FREQUENCY_PARAM, 0.f, 1.f, 1.f));
    addParam(createParam<LRMiddleKnob>(Vec(110, 161), module, MS20Filter::PEAK_PARAM, 0.0f, 1.0, 0.0f));
    addParam(createParam<LRMiddleKnob>(Vec(110, 230.7), module, MS20Filter::DRIVE_PARAM, 0.f, 1.0, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(61, 169), module, MS20Filter::PEAK_CV_PARAM, -1.f, 1.0f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(61, 82), module, MS20Filter::CUTOFF_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(61, 238.555), module, MS20Filter::GAIN_CV_PARAM, -1.f, 1.f, 0.f));

    addInput(createInput<IOPort>(Vec(18, 168.5), module, MS20Filter::PEAK_CV_INPUT));
    addInput(createInput<IOPort>(Vec(18, 81.5), module, MS20Filter::CUTOFF_CV_INPUT));
    addInput(createInput<IOPort>(Vec(18, 239), module, MS20Filter::GAIN_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(17.999, 326.05), module, MS20Filter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(58.544, 326.05), module, MS20Filter::FILTER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    addParam(createParam<LRTSwitch>(Vec(119, 331), module, MS20Filter::MODE_SWITCH_PARAM, 0.0, 1.0, 1.0));
    // ***** SWITCH  *********

    // ***** LIGHTS **********
    //  addChild(createLight<LRRedLight>(Vec(85, 300), module, MS20Filter::OVERLOAD_LIGHT));
    // ***** LIGHTS **********

    // ***** LCD *************
    // module->lcd1->box.pos = Vec(34, 55);
    // addChild(module->lcd1);
    // ***** LCD *************

}
