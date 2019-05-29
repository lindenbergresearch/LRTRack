#include "../dsp/MS20zdf.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;

struct MS20FilterWidget;


struct MS20Filter : LRModule {

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
        NUM_LIGHTS
    };

    lrt::MS20zdf *ms20zdf = new lrt::MS20zdf(APP->engine->getSampleRate());

    MS20FilterWidget *reflect;


    MS20Filter() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        configParam(FREQUENCY_PARAM, 0.f, 1.f, 1.f);
        configParam(PEAK_PARAM, 0.0f, 1.0, 0.0f);
        configParam(DRIVE_PARAM, 0.f, 1.0, 0.0f);

        configParam(CUTOFF_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(PEAK_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(GAIN_CV_PARAM, -1.f, 1.0f, 0.f);

        configParam(MODE_SWITCH_PARAM, 0.0, 1.0, 1.0);
    }


    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
};


void MS20Filter::onSampleRateChange() {
    Module::onSampleRateChange();
    ms20zdf->updateSampleRate(APP->engine->getSampleRate());
}


/**
 * @brief Valerie MS20 filter
 */
struct MS20FilterWidget : LRModuleWidget {
    LRBigKnob *frqKnob;
    LRMiddleKnob *peakKnob;
    LRMiddleKnob *driveKnob;

    MS20FilterWidget(MS20Filter *module);
};


MS20FilterWidget::MS20FilterWidget(MS20Filter *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/MS20.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/MS20Light.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/MS20Aged.svg")));

    panel->init();
    addChild(panel);
    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;

    // ***** SCREWS **********
    addChild(createWidget<ScrewLight>(Vec(15, 1)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
    addChild(createWidget<ScrewLight>(Vec(15, 366)));
    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    frqKnob = createParam<LRBigKnob>(Vec(102, 64.9), module, MS20Filter::FREQUENCY_PARAM);
    peakKnob = createParam<LRMiddleKnob>(Vec(109, 159.8), module, MS20Filter::PEAK_PARAM);
    driveKnob = createParam<LRMiddleKnob>(Vec(109, 229.6), module, MS20Filter::DRIVE_PARAM);

    addParam(frqKnob);
    addParam(peakKnob);
    addParam(driveKnob);
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(61, 169.3), module, MS20Filter::PEAK_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(61, 82.4), module, MS20Filter::CUTOFF_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(61, 239), module, MS20Filter::GAIN_CV_PARAM));

    addInput(createInput<LRIOPortCV>(Vec(18, 168.5), module, MS20Filter::PEAK_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(18, 81.5), module, MS20Filter::CUTOFF_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(18, 239), module, MS20Filter::GAIN_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(17.999, 326.05), module, MS20Filter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(58.544, 326.05), module, MS20Filter::FILTER_OUTPUT));
    // ***** OUTPUTS *********

    // ***** SWITCH  *********
    addParam(createParam<LRSwitch>(Vec(119, 331), module, MS20Filter::MODE_SWITCH_PARAM));
    // ***** SWITCH  *********
}


void MS20Filter::process(const ProcessArgs &args) {
    /* compute control voltages */
    float frqcv = inputs[CUTOFF_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[CUTOFF_CV_PARAM].getValue());
    float peakcv = inputs[PEAK_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[PEAK_CV_PARAM].getValue());
    float gaincv = inputs[GAIN_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[GAIN_CV_PARAM].getValue());

    /* set cv modulated parameters */
    ms20zdf->setFrequency(params[FREQUENCY_PARAM].getValue() + frqcv);
    ms20zdf->setPeak(params[PEAK_PARAM].getValue() + peakcv);
    ms20zdf->setDrive(params[DRIVE_PARAM].getValue() + gaincv);

    /* pass modulated parameter to knob widget for cv indicator */
    reflect->frqKnob->setIndicatorActive(inputs[CUTOFF_CV_INPUT].isConnected());
    reflect->peakKnob->setIndicatorActive(inputs[PEAK_CV_INPUT].isConnected());
    reflect->driveKnob->setIndicatorActive(inputs[GAIN_CV_INPUT].isConnected());

    reflect->frqKnob->setIndicatorValue(params[FREQUENCY_PARAM].getValue() + frqcv);
    reflect->peakKnob->setIndicatorValue(params[PEAK_PARAM].getValue() + peakcv);
    reflect->driveKnob->setIndicatorValue(params[DRIVE_PARAM].getValue() + gaincv);

    /* process signal */
    ms20zdf->setType(params[MODE_SWITCH_PARAM].getValue());
    ms20zdf->setIn(inputs[FILTER_INPUT].getVoltage());
    ms20zdf->process();

    outputs[FILTER_OUTPUT].setVoltage(ms20zdf->getLPOut());
}


Model *modelMS20Filter = createModel<MS20Filter, MS20FilterWidget>("MS20 VCF");
