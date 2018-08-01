#include <src/dsp/Lockhart.hpp>
#include "LindenbergResearch.hpp"


struct Westcoast : LRModule {

    enum ParamIds {
        GAIN_PARAM,
        KPOS_PARAM,
        KNEG_PARAM,
        BIAS_PARAM,
        TYPE_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        SHAPER_INPUT,
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
    LCDWidget *lcd = new LCDWidget(nvgRGBAf(0.9, 0.1, 0.3, 1.0), 10, "%s", LCDWidget::LIST, 14.f);


    void step() override;
    void onSampleRateChange() override;
};


void Westcoast::step() {
    hs->setGain((params[GAIN_PARAM].value));
    // hs->setAmplitude(params[KPOS_PARAM].value, params[KNEG_PARAM].value);
    hs->setBias(params[BIAS_PARAM].value);
    hs->setIn(inputs[SHAPER_INPUT].value);

    hs->process();

    outputs[SHAPER_OUTPUT].value = hs->getOut();

    lcd->value = params[TYPE_PARAM].value;
}


void Westcoast::onSampleRateChange() {
    Module::onSampleRateChange();
    // pass to dsp system
    hs->setSamplerate(engineGetSampleRate());
}


/**
 * @brief Valerie MS20 filter
 */
struct WestcoastWidget : LRModuleWidget {
    WestcoastWidget(Westcoast *module);
};


WestcoastWidget::WestcoastWidget(Westcoast *module) : LRModuleWidget(module) {
    panel = new LRPanel();
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/Westcoast.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // **** SETUP LCD ********
    module->lcd->box.pos = Vec(24, 239);
    module->lcd->addItem("LOCKHART");
    module->lcd->addItem("Chebyshev");
    module->lcd->addItem("SINE");
    module->lcd->addItem("SATURATE");
    module->lcd->addItem("HARDCLIP");


    addChild(module->lcd);
    // **** SETUP LCD ********

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(LRKnob::create<LRBigKnob>(Vec(102, 64.9), module, Westcoast::GAIN_PARAM, 0.25, 5.f, 0.25f));
    addParam(LRKnob::create<LRMiddleKnob>(Vec(22, 134.9), module, Westcoast::KPOS_PARAM, 0.1f, 2.f, 1.f));
    addParam(LRKnob::create<LRMiddleKnob>(Vec(122, 134.9), module, Westcoast::KNEG_PARAM, 0.1f, 2.f, 1.f));
    addParam(LRKnob::create<LRMiddleKnob>(Vec(22, 64), module, Westcoast::BIAS_PARAM, -5.f, 5.f, 0.f));

    addParam(LRKnob::create<LRMiddleIncremental>(Vec(42, 260), module, Westcoast::TYPE_PARAM, 0, 1, 0));

    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(Port::create<LRIOPort>(Vec(17.999, 326.05), Port::INPUT, module, Westcoast::SHAPER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<LRIOPort>(Vec(58.544, 326.05), Port::OUTPUT, module, Westcoast::SHAPER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    // ***** SWITCH  *********
}


Model *modelWestcoast = Model::create<Westcoast, WestcoastWidget>("Lindenberg Research", "Westcoast VCS",
                                                                  "Westcoast Complex Shaper", WAVESHAPER_TAG);
