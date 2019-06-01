#include "../dsp/FastTanWF.hpp"
#include "../dsp/Overdrive.hpp"
#include "../dsp/Hardclip.hpp"
#include "../dsp/RShaper.hpp"
#include "../dsp/Serge.hpp"
#include "../dsp/Lockhart.hpp"
#include "../dsp/Saturator.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;

struct WestcoastWidget;

struct Westcoast : LRModule {

    enum RotaryStages {
        SERGE = 1,
        LOCKHART,
        OVERDRIVE,
        SATURATE,
        RESHAPER,
        VALERIE,
        HARDCLIP
    };

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

    Westcoast() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        configParam(GAIN_PARAM, 0.0, 20.f, 1.f);
        configParam(BIAS_PARAM, -6.f, 6.f, 0.f);

        configParam(CV_GAIN_PARAM, -1.f, 1.f, 0.f);
        configParam(CV_BIAS_PARAM, -1.f, 1.f, 0.f);

        configParam(TYPE_PARAM, 1, 7, 1);
    }


    lrt::LockhartWavefolder *hs = new lrt::LockhartWavefolder(APP->engine->getSampleRate());
    lrt::SergeWavefolder *sg = new lrt::SergeWavefolder(APP->engine->getSampleRate());
    lrt::Saturator *saturator = new lrt::Saturator(APP->engine->getSampleRate());
    lrt::Hardclip *hardclip = new lrt::Hardclip(APP->engine->getSampleRate());
    lrt::ReShaper *reshaper = new lrt::ReShaper(APP->engine->getSampleRate());
    lrt::Overdrive *overdrive = new lrt::Overdrive(APP->engine->getSampleRate());
    lrt::FastTan *fastTan = new lrt::FastTan(APP->engine->getSampleRate());

    WestcoastWidget *reflect;

    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
};



void Westcoast::onSampleRateChange() {
    Module::onSampleRateChange();

    hs->setSamplerate(APP->engine->getSampleRate());
    sg->setSamplerate(APP->engine->getSampleRate());
    saturator->setSamplerate(APP->engine->getSampleRate());
}


struct WestcoastWidget : LRModuleWidget {
    LRBigKnob *gainBtn;
    LRMiddleKnob *biasBtn;

    WestcoastWidget(Westcoast *module);
};


WestcoastWidget::WestcoastWidget(Westcoast *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/Westcoast.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/WestcoastLight.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/WestcoastAged.svg")));

    panel->init();
    addChild(panel);
    box.size = panel->box.size;


    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(15, 1)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(createWidget<ScrewLight>(Vec(15, 366)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    gainBtn = createParam<LRBigKnob>(Vec(128.7, 63.0), module, Westcoast::GAIN_PARAM);
    biasBtn = createParam<LRMiddleKnob>(Vec(135.4, 152.3), module, Westcoast::BIAS_PARAM);

    addParam(gainBtn);
    addParam(biasBtn);

    auto *toggleknob = createParam<LRToggleKnob>(Vec(83.8, 277.8), module, Westcoast::TYPE_PARAM);
    toggleknob->minAngle = -0.5f * M_PI;
    addParam(toggleknob);

    addParam(createParam<LRSmallKnob>(Vec(83.4, 101.00), module, Westcoast::CV_GAIN_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(83.4, 183.0), module, Westcoast::CV_BIAS_PARAM));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addInput(createInput<LRIOPortCV>(Vec(32.4, 99.0), module, Westcoast::CV_GAIN_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(32.4, 179.8), module, Westcoast::CV_BIAS_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(22.4, 326.05), module, Westcoast::SHAPER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(159.4, 326.05), module, Westcoast::SHAPER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    //addParam(ParamcreateWidget<LRSwitch>(Vec(119, 331), module, Westcoast::DCBLOCK_PARAM, 0.0, 1.0, 1.0));
    // ***** SWITCH  *********
}


void Westcoast::process(const ProcessArgs &args) {
    float gaincv = 0;
    float biascv = 0;

    /* not connected */
    if (!inputs[SHAPER_INPUT].isConnected()) {
        outputs[SHAPER_OUTPUT].setVoltage(0.f);

        return;
    }

    if (inputs[CV_GAIN_INPUT].isConnected()) {
        gaincv = inputs[CV_GAIN_INPUT].getVoltage() * dsp::quadraticBipolar(params[CV_GAIN_PARAM].getValue()) * 4.0f;
    }

    if (inputs[CV_BIAS_INPUT].isConnected()) {
        biascv = inputs[CV_BIAS_INPUT].getVoltage() * dsp::quadraticBipolar(params[CV_BIAS_PARAM].getValue()) * 2.0f;
    }

    reflect->gainBtn->setIndicatorActive(inputs[CV_GAIN_INPUT].isConnected());
    reflect->biasBtn->setIndicatorActive(inputs[CV_BIAS_INPUT].isConnected());

    reflect->gainBtn->setIndicatorValue((params[GAIN_PARAM].getValue() + gaincv) / 20);
    reflect->biasBtn->setIndicatorValue((params[BIAS_PARAM].getValue() + (biascv + 6)) / 12);

    float out;
    float gain = params[GAIN_PARAM].getValue() + gaincv;
    float bias = params[BIAS_PARAM].getValue() + biascv;

    switch (lround(params[TYPE_PARAM].getValue())) {
        case LOCKHART:  // Lockhart Model
            hs->setGain(gain);
            hs->setBias(bias);
            hs->setIn(inputs[SHAPER_INPUT].getVoltage());

            hs->process();
            out = (float) hs->getOut();
            break;

        case SERGE:     // Serge Model
            sg->setGain(gain);
            sg->setBias(bias);
            sg->setIn(inputs[SHAPER_INPUT].getVoltage());

            sg->process();
            out = (float) sg->getOut();
            break;

        case SATURATE: // Saturator
            saturator->setGain(gain);
            saturator->setBias(bias);
            saturator->setIn(inputs[SHAPER_INPUT].getVoltage());

            saturator->process();
            out = (float) saturator->getOut();
            break;

        case HARDCLIP: // Hardclip
            hardclip->setGain(gain);
            hardclip->setBias(bias);
            hardclip->setIn(inputs[SHAPER_INPUT].getVoltage());

            hardclip->process();
            out = (float) hardclip->getOut();
            break;

        case RESHAPER: // ReShaper
            reshaper->setGain(gain);
            reshaper->setBias(bias);
            reshaper->setIn(inputs[SHAPER_INPUT].getVoltage());

            reshaper->process();
            out = (float) reshaper->getOut();
            break;

        case OVERDRIVE: // Overdrive
            overdrive->setGain(gain);
            overdrive->setBias(bias);
            overdrive->setIn(inputs[SHAPER_INPUT].getVoltage());

            overdrive->process();
            out = (float) overdrive->getOut();
            break;

        case VALERIE: // Overdrive
            fastTan->setGain(gain);
            fastTan->setBias(bias);
            fastTan->setIn(inputs[SHAPER_INPUT].getVoltage());

            fastTan->process();
            out = (float) fastTan->getOut();
            break;

        default: // invalid state, should not happen
            out = 0;
            break;
    }


    outputs[SHAPER_OUTPUT].setVoltage(out);
}


Model *modelWestcoast = createModel<Westcoast, WestcoastWidget>("Westcoast_VCS");
