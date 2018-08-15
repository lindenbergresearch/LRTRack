#include "dsp/Serge.hpp"
#include "dsp/Lockhart.hpp"


#include "LindenbergResearch.hpp"


struct Westcoast : LRModule {

    enum ParamIds {
        GAIN_PARAM,
        CV_GAIN_PARAM,
        CV_BIAS_PARAM,
        BIAS_PARAM,
        TYPE_PARAM,
        DCBLOCK_PARAM,
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
    dsp::SergeWavefolder *sg = new dsp::SergeWavefolder(engineGetSampleRate());

    void step() override;
    void onSampleRateChange() override;
};


void Westcoast::step() {
    hs->setGain((params[GAIN_PARAM].value));
    hs->setBias(params[BIAS_PARAM].value);
    hs->setIn(inputs[SHAPER_INPUT].value);

    sg->setGain((params[GAIN_PARAM].value));
    sg->setBias(params[BIAS_PARAM].value);
    sg->setIn(inputs[SHAPER_INPUT].value);


    hs->setBlockDC(params[DCBLOCK_PARAM].value == 1);


    float out;

    switch (lround(params[TYPE_PARAM].value)) {
        case 1: // Lockhart Model
            hs->process();
            out = (float) hs->getOut();
            break;
        case 2: // Serge Model
            sg->process();
            out = (float) sg->getOut();
            break;
        default: // invalid state, should not happen
            out = 0;
            break;
    }

    outputs[SHAPER_OUTPUT].value = out;
}


void Westcoast::onSampleRateChange() {
    Module::onSampleRateChange();
    hs->setSamplerate(engineGetSampleRate());
    sg->setSamplerate(engineGetSampleRate());
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
    addChild(Widget::create<ScrewLight>(Vec(15, 1)));
    addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewLight>(Vec(15, 366)));
    addChild(Widget::create<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(LRKnob::create<LRAlternateBigKnob>(Vec(128.7, 63.0), module, Westcoast::GAIN_PARAM, 0.25, 20.f, 1.f));
    addParam(LRKnob::create<LRAlternateMiddleKnob>(Vec(136.4, 153.3), module, Westcoast::BIAS_PARAM, -3.f, 3.f, 0.f));
    addParam(LRKnob::create<LRMiddleIncremental>(Vec(85, 274.3), module, Westcoast::TYPE_PARAM, 1, 6, 1));

    addParam(LRKnob::create<LRAlternateSmallKnob>(Vec(83.4, 101.00), module, Westcoast::CV_GAIN_PARAM, -1.f, 1.f, 0.f));
    addParam(LRKnob::create<LRAlternateSmallKnob>(Vec(83.4, 183.0), module, Westcoast::CV_BIAS_PARAM, -1.f, 1.f, 0.f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addInput(Port::create<LRIOPortC>(Vec(32.4, 99.0), Port::INPUT, module, Westcoast::CV_GAIN_INPUT));
    addInput(Port::create<LRIOPortC>(Vec(32.4, 179.8), Port::INPUT, module, Westcoast::CV_BIAS_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(Port::create<LRIOPortC>(Vec(22.4, 324.6), Port::INPUT, module, Westcoast::SHAPER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPortC>(Vec(159.4, 324.6), Port::OUTPUT, module, Westcoast::SHAPER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    addParam(ParamWidget::create<LRSwitch>(Vec(119, 331), module, Westcoast::DCBLOCK_PARAM, 0.0, 1.0, 1.0));
    // ***** SWITCH  *********
}


Model *modelWestcoast = Model::create<Westcoast, WestcoastWidget>("Lindenberg Research", "Westcoast VCS",
                                                                  "Westcoast Complex Shaper", WAVESHAPER_TAG);
