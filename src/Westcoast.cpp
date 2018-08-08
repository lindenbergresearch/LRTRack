#include "dsp/Lockhart.hpp"
#include "LindenbergResearch.hpp"


struct Westcoast : LRModule {

    enum ParamIds {
        GAIN_PARAM,
        CV_GAIN_PARAM,
        CV_BIAS_PARAM,
        BIAS_PARAM,
        TYPE_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        SHAPER_INPUT,
        CV_GAIN_INPUT,
        CV_BIAS_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        SHAPER_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        NUM_LIGHTS
    };


    Westcoast() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    dsp::LockhartWavefolder *hs = new dsp::LockhartWavefolder(engineGetSampleRate());

    void step() override;
    void onSampleRateChange() override;
};


void Westcoast::step() {
    hs->setGain((params[GAIN_PARAM].value));
    hs->setBias(params[BIAS_PARAM].value);
    hs->setIn(inputs[SHAPER_INPUT].value);

    hs->process();

    outputs[SHAPER_OUTPUT].value = hs->getOut() * 5;
}


void Westcoast::onSampleRateChange() {
    Module::onSampleRateChange();
    hs->setSamplerate(engineGetSampleRate());
}


struct WestcoastWidget : LRModuleWidget {
    WestcoastWidget(Westcoast *module);
};


WestcoastWidget::WestcoastWidget(Westcoast *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/Westcoast.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(LRKnob::create<LRAlternateBigKnob>(Vec(28.4, 62.6), module, Westcoast::GAIN_PARAM, 0.25, 20.f, 1.f));
    addParam(LRKnob::create<LRAlternateMiddleKnob>(Vec(36.4, 155.7), module, Westcoast::BIAS_PARAM, -0.5f, 0.5f, 0.f));
    addParam(LRKnob::create<LRMiddleIncremental>(Vec(85, 269.3), module, Westcoast::TYPE_PARAM, -3, 3, 0));

    addParam(LRKnob::create<LRAlternateSmallKnob>(Vec(114, 78.7), module, Westcoast::CV_GAIN_PARAM, -1.f, 1.f, 0.f));
    addParam(LRKnob::create<LRAlternateSmallKnob>(Vec(114, 163.3), module, Westcoast::CV_BIAS_PARAM, -1.f, 1.f, 0.f));

    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addInput(Port::create<LRIOPort>(Vec(159.9, 78.6), Port::INPUT, module, Westcoast::CV_GAIN_INPUT));
    addInput(Port::create<LRIOPort>(Vec(159.9, 163.7), Port::INPUT, module, Westcoast::CV_BIAS_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(Port::create<LRIOPort>(Vec(22.9, 326.7), Port::INPUT, module, Westcoast::SHAPER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPort>(Vec(159.9, 326.7), Port::OUTPUT, module, Westcoast::SHAPER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    // ***** SWITCH  *********
}


Model *modelWestcoast = Model::create<Westcoast, WestcoastWidget>("Lindenberg Research", "Westcoast VCS",
                                                                  "Westcoast Complex Shaper", WAVESHAPER_TAG);
