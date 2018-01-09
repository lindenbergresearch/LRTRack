#include "dsp/Oscillator.hpp"
#include "LindenbergResearch.hpp"


struct VCO : LRTModule {
    enum ParamIds {
        FREQUENCY_PARAM,
        OCTAVE_PARAM,
        FM_CV_PARAM,
        SHAPE_CV_PARAM,
        PW_CV_PARAM,
        SHAPE_PARAM,
        PW_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        VOCT_INPUT,
        FM_CV_INPUT,
        PW_CV_INPUT,
        SHAPE_CV_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
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

    float fm = clampf(inputs[FM_CV_INPUT].value, -10.f, 10.f) * 400.f * quadraticBipolar(params[FM_CV_PARAM].value);

    osc.updatePitch(inputs[VOCT_INPUT].value, clampf(fm, 0.f, 20.000f), params[FREQUENCY_PARAM].value, params[OCTAVE_PARAM].value);

    float saturate = params[SHAPE_PARAM].value;
    float pw = params[PW_CV_PARAM].value;

    if (osc.saturate != saturate) {
        osc.setSaturate(quadraticBipolar(saturate));
    }

    if (osc.pw != pw) {
        osc.setPulseWidth(pw);
    }

    osc.proccess();

    outputs[SAW_OUTPUT].value = osc.saw;
    //   outputs[SAW_OUTPUT].value = osc.saw;

    outputs[PULSE_OUTPUT].value = osc.pulse;
    outputs[SAWTRI_OUTPUT].value = osc.sawtri;

    outputs[TRI_OUTPUT].value = osc.tri;

    /*if (cnt % 1200 == 0) {
        label1->text = stringf("%.2f Hz", osc.freq);
    }*/
}


VCOWidget::VCOWidget() {
    VCO *module = new VCO();

    setModule(module);
    box.size = Vec(OSCILLATOR_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

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

    /*auto *lw = new LRLightWidget();
    lw->box.pos = Vec(100, 100);
    lw->box.size = Vec(2, 2);
    addChild(lw);*/

    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(83, 172.0), module, VCO::FREQUENCY_PARAM, -15.f, 15.f, 0.f));
    addParam(createParam<LRToggleKnob>(Vec(85, 240), module, VCO::OCTAVE_PARAM, -3.f, 3.f, 0.f));

    addParam(createParam<LRSmallKnob>(Vec(118, 111.5), module, VCO::PW_PARAM, -.1f, 1.f, 1.f));
    addParam(createParam<LRSmallKnob>(Vec(65, 60), module, VCO::SHAPE_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(15, 267), module, VCO::FM_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(65, 111.5), module, VCO::PW_CV_PARAM, 0.02f, 1.f, 1.f));
    addParam(createParam<LRSmallKnob>(Vec(118, 59), module, VCO::SHAPE_PARAM, 0.4f, 1.f, 1.f));


    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(15, 182), module, VCO::VOCT_INPUT));
    addInput(createInput<IOPort>(Vec(15, 228), module, VCO::FM_CV_INPUT));
    addInput(createInput<IOPort>(Vec(15, 112), module, VCO::PW_CV_INPUT));
    addInput(createInput<IOPort>(Vec(15, 60), module, VCO::SHAPE_CV_INPUT));

    //  addInput(createInput<IOPort>(Vec(71, 60), module, VCO::RESHAPER_CV_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    // addOutput(createOutput<IOPort>(Vec(20, 320), module, VCO::SAW_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(11, 319), module, VCO::SAW_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(49, 319), module, VCO::PULSE_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(86, 319), module, VCO::SAWTRI_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(124, 319), module, VCO::TRI_OUTPUT));
    // ***** OUTPUTS *********

    //module->label1->box.pos = Vec(30, 310);

    //addChild(module->label1);
}
