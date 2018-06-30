#include "dsp/Oscillator.hpp"
#include "LindenbergResearch.hpp"


struct VCO : LRModule {
    enum ParamIds {
        FREQUENCY_PARAM,
        OCTAVE_PARAM,
        FM_CV_PARAM,
        PW_CV_PARAM,
        SHAPE_PARAM,
        PW_PARAM,
        SAW_PARAM,
        PULSE_PARAM,
        SINE_PARAM,
        TRI_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        VOCT1_INPUT,
        FM_CV_INPUT,
        PW_CV_INPUT,
        VOCT2_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        SAW_OUTPUT,
        PULSE_OUTPUT,
        SINE_OUTPUT,
        TRI_OUTPUT,
        NOISE_OUTPUT,
        MIX_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    dsp::BLITOscillator *osc = new dsp::BLITOscillator();
    LCDWidget *label1 = new LCDWidget(COLOR_CYAN, 10);


    VCO() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override;
};


void VCO::step() {
    LRModule::step();

    float fm = clamp(inputs[FM_CV_INPUT].value, -10.f, 10.f) * 400.f * quadraticBipolar(params[FM_CV_PARAM].value);

    osc->updatePitch(inputs[VOCT1_INPUT].value, clamp(fm, -10000.f, 10000.f), params[FREQUENCY_PARAM].value, params[OCTAVE_PARAM].value);

    float shape = quadraticBipolar(params[SHAPE_PARAM].value);
    float pw = params[PW_CV_PARAM].value;

    if (osc->shape != shape) {
        osc->setShape(shape);
    }

    if (osc->pw != pw) {
        osc->setPulseWidth(pw);
    }

    osc->proccess();

    outputs[SAW_OUTPUT].value = osc->saw;

    outputs[PULSE_OUTPUT].value = osc->pulse;
    outputs[SINE_OUTPUT].value = osc->sine;

    outputs[TRI_OUTPUT].value = osc->tri;

    if (cnt % 1200 == 0) {
        label1->text = stringf("%.2f Hz", osc->getFrequency());
    }
}


/**
 * @brief Woldemar VCO
 */
struct VCOWidget : LRModuleWidget {
    VCOWidget(VCO *module);
};


VCOWidget::VCOWidget(VCO *module) : LRModuleWidget(module) {
    //  setPanel(SVG::load(assetPlugin(plugin, "res/VCO.svg")));

    panel = new LRPanel(20, 40);
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/VCO.svg")));
    addChild(panel);

    box.size = panel->box.size;

    // ***** SCREWS **********
    addChild(Widget::create<ScrewDarkA>(Vec(15, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(Widget::create<ScrewDarkA>(Vec(15, 366)));
    addChild(Widget::create<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    addParam(ParamWidget::create<LRBigKnob>(Vec(126.0, 61.9), module, VCO::FREQUENCY_PARAM, -15.f, 15.f, 0.f));
    addParam(ParamWidget::create<LRToggleKnob>(Vec(134.7, 172.0), module, VCO::OCTAVE_PARAM, -3.f, 3.f, 0.f));

    //addParam(ParamWidget::create<LRSmallKnob>(Vec(81.5, 195), module, VCO::PW_PARAM, -.1f, 1.f, 1.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(69.5, 125.5), module, VCO::FM_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(69.5, 181.8), module, VCO::PW_CV_PARAM, 0.02f, 1.f, 1.f));


    addParam(ParamWidget::create<LRSmallKnob>(Vec(22.8, 270.1), module, VCO::SAW_PARAM, -1.f, 1.f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(58.3, 270.1), module, VCO::PULSE_PARAM, -1.f, 1.f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(93.1, 270.1), module, VCO::SINE_PARAM, -1.f, 1.f, 0.f));
    addParam(ParamWidget::create<LRSmallKnob>(Vec(128.1, 270.1), module, VCO::TRI_PARAM, -1.f, 1.f, 0.f));
    // ***** MAIN KNOBS ******


    // ***** INPUTS **********
    addInput(Port::create<IOPort>(Vec(20.8, 67.9), Port::INPUT, module, VCO::VOCT1_INPUT));
    addInput(Port::create<IOPort>(Vec(68.0, 67.9), Port::INPUT, module, VCO::VOCT2_INPUT));
    addInput(Port::create<IOPort>(Vec(20.8, 125.5), Port::INPUT, module, VCO::FM_CV_INPUT));
    addInput(Port::create<IOPort>(Vec(20.8, 181.8), Port::INPUT, module, VCO::PW_CV_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(Port::create<IOPort>(Vec(21, 305.8), Port::OUTPUT, module, VCO::SAW_OUTPUT));
    addOutput(Port::create<IOPort>(Vec(56.8, 305.8), Port::OUTPUT, module, VCO::PULSE_OUTPUT));
    addOutput(Port::create<IOPort>(Vec(91.6, 305.8), Port::OUTPUT, module, VCO::SINE_OUTPUT));
    addOutput(Port::create<IOPort>(Vec(126.6, 305.8), Port::OUTPUT, module, VCO::TRI_OUTPUT));
    addOutput(Port::create<IOPort>(Vec(162.0, 305.8), Port::OUTPUT, module, VCO::NOISE_OUTPUT));
    addOutput(Port::create<IOPort>(Vec(162.0, 269.1), Port::OUTPUT, module, VCO::MIX_OUTPUT));
    // ***** OUTPUTS *********

    /*module->label1->box.pos = Vec(110, 250);

    addChild(module->label1);*/
}


Model *modelVCO = Model::create<VCO, VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG);
