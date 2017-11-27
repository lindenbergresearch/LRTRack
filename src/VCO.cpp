#include "dsp/Oscillator.hpp"
#include "VCO.hpp"
#include "LindenbergResearch.hpp"


struct VCO : LRTModule {
    enum ParamIds {
        FREQUENCY_PARAM,
        OCTAVE_PARAM,
        HARMONICS_PARAM,
        SATURATE_PARAM,
        PW_PARAM,
        NUM_PARAMS
    };
    enum InputIds {

        NUM_INPUTS
    };
    enum OutputIds {
        RAMP_OUTPUT,
        SAW_OUTPUT,
        PULSE_OUTPUT,
        SAWTRI_OUTPUT,
        TRI_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    BLITOscillator osc;
    LCDWidget *label1 = new LCDWidget(LCD_COLOR_FG, 10);

    VCO() : LRTModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override;
};


void VCO::step() {
    LRTModule::step();

    if (osc.freq != (params[FREQUENCY_PARAM].value * params[OCTAVE_PARAM].value)) {
        osc.setFrequency(params[FREQUENCY_PARAM].value * params[OCTAVE_PARAM].value);
    }

    float harmonics = params[HARMONICS_PARAM].value * BLIT_HARMONICS;
    float pw = params[PW_PARAM].value;

    if (osc.harmonics != harmonics) {
        osc.setHarmonics(harmonics);
    }

    if (osc.pw != pw) {
        osc.setPulseWidth(pw);
    }

    osc.proccess();

    outputs[RAMP_OUTPUT].value = osc.ramp;
    outputs[SAW_OUTPUT].value = osc.saw;

    outputs[PULSE_OUTPUT].value = osc.pulse;
    outputs[SAWTRI_OUTPUT].value = osc.sawtri;

    outputs[TRI_OUTPUT].value = osc.tri;

    if (cnt % 1000 == 0) {
        label1->text = stringf("%.2f Hz", osc.freq);
    }
}


VCOWidget::VCOWidget() {
    VCO *module = new VCO();

    setModule(module);
    box.size = Vec(MODULE_WIDTH * RACK_GRID_WIDTH, MODULE_HEIGHT);

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
    addParam(createParam<LRBigKnobWhite>(Vec(35, 216), module, VCO::FREQUENCY_PARAM, 32.f, 165.f, 44.f));
    addParam(createParam<LRBigKnobWhite>(Vec(35, 120), module, VCO::OCTAVE_PARAM, 1.f, 32.f, 0.f));

    addParam(createParam<LRBasicKnobWhite>(Vec(155, 216), module, VCO::HARMONICS_PARAM, 0.1f, 1.f, 1.f));
    addParam(createParam<LRBasicKnobWhite>(Vec(155, 130), module, VCO::SATURATE_PARAM, 0.f, 1.f, 1.f));
    addParam(createParam<LRBasicKnobWhite>(Vec(155, 80), module, VCO::PW_PARAM, 0.1f, 1.f, 1.f));


    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    //  addInput(createInput<IOPort>(Vec(21, 60), module, VCO::RESHAPER_INPUT));
    //  addInput(createInput<IOPort>(Vec(71, 60), module, VCO::RESHAPER_CV_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(20, 320), module, VCO::SAW_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(50, 320), module, VCO::RAMP_OUTPUT));

    addOutput(createOutput<IOPort>(Vec(80, 320), module, VCO::PULSE_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(110, 320), module, VCO::SAWTRI_OUTPUT));

    addOutput(createOutput<IOPort>(Vec(140, 320), module, VCO::TRI_OUTPUT));
    // ***** OUTPUTS *********

    module->label1->box.pos = Vec(10, 100);

    addChild(module->label1);
}
