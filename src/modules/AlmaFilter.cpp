#include "../dsp/LadderFilter.hpp"
#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;

struct AlmaFilterWidget;


struct AlmaFilter : LRModule {

    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        DRIVE_PARAM,
        SLOPE_PARAM,
        CUTOFF_CV_PARAM,
        RESONANCE_CV_PARAM,
        DRIVE_CV_PARAM,
        NUM_PARAMS
    };

    enum InputIds {
        FILTER_INPUT,
        CUTOFF_CV_INPUT,
        RESONANCE_CV_INPUT,
        DRIVE_CV_INPUT,
        NUM_INPUTS
    };

    enum OutputIds {
        LP_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        OVERLOAD_LIGHT,
        NUM_LIGHTS
    };

    AlmaFilterWidget *reflect;

    lrt::LadderFilter *filter = new lrt::LadderFilter(APP->engine->getSampleRate());


    AlmaFilter() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        configParam(CUTOFF_PARAM, 0.f, 1.f, 0.8f);
        configParam(RESONANCE_PARAM, -0.f, 1.5, 0.0f);
        configParam(DRIVE_PARAM, 0.0f, 1.f, 0.0f);

        configParam(RESONANCE_CV_PARAM, -1.f, 1.0f, 0.f);
        configParam(CUTOFF_CV_PARAM, -1.f, 1.f, 0.f);
        configParam(DRIVE_CV_PARAM, -1.f, 1.f, 0.f);
        configParam(SLOPE_PARAM, 0.0f, 4.f, 2.0f);


    }


    void process(const ProcessArgs &args) override;
    void onSampleRateChange() override;
};


void AlmaFilter::onSampleRateChange() {
    Module::onSampleRateChange();
    filter->setSamplerate(APP->engine->getSampleRate());
}


/**
 * @brief ALMA filter
 */
struct AlmaFilterWidget : LRModuleWidget {
    LRBigKnob *frqKnob;
    LRMiddleKnob *peakKnob;
    LRMiddleKnob *driveKnob;

    AlmaFilterWidget(AlmaFilter *module);
};


AlmaFilterWidget::AlmaFilterWidget(AlmaFilter *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/VCF.svg")));
    panel->addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/AlmaLight.svg")));
    panel->addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/AlmaAged.svg")));

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

    frqKnob = createParam<LRBigKnob>(Vec(62, 150), module, AlmaFilter::CUTOFF_PARAM);
    peakKnob = createParam<LRMiddleKnob>(Vec(23, 228), module, AlmaFilter::RESONANCE_PARAM);
    driveKnob = createParam<LRMiddleKnob>(Vec(115, 227), module, AlmaFilter::DRIVE_PARAM);

    addParam(frqKnob);
    addParam(peakKnob);
    addParam(driveKnob);

    addParam(createParam<LRMiddleKnob>(Vec(69, 287), module, AlmaFilter::SLOPE_PARAM));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(27.5, 106), module, AlmaFilter::RESONANCE_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(78, 106), module, AlmaFilter::CUTOFF_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(127.1, 106), module, AlmaFilter::DRIVE_CV_PARAM));

    addInput(createInput<LRIOPortCV>(Vec(26, 50), module, AlmaFilter::RESONANCE_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(76, 50), module, AlmaFilter::CUTOFF_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(125, 50), module, AlmaFilter::DRIVE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(25, 326.5), module, AlmaFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(124.5, 326.5), module, AlmaFilter::LP_OUTPUT));
    // ***** OUTPUTS *********

    // ***** LIGHTS **********
    addChild(createLight<LRLight>(Vec(85, 247), module, AlmaFilter::OVERLOAD_LIGHT));
    // ***** LIGHTS **********
}


void AlmaFilter::process(const ProcessArgs &args) {
    float frqcv = inputs[CUTOFF_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[CUTOFF_CV_PARAM].getValue());
    float rescv = inputs[RESONANCE_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[RESONANCE_CV_PARAM].getValue());
    float drvcv = inputs[DRIVE_CV_INPUT].getVoltage() * 0.1f * dsp::quadraticBipolar(params[DRIVE_CV_PARAM].getValue());

    filter->setFrequency(params[CUTOFF_PARAM].getValue() + frqcv);
    filter->setResonance(params[RESONANCE_PARAM].getValue() + rescv);
    filter->setDrive(params[DRIVE_PARAM].getValue() + drvcv);
    filter->setSlope(params[SLOPE_PARAM].getValue());


    /* pass modulated parameter to knob widget for cv indicator */
    reflect->frqKnob->setIndicatorActive(inputs[CUTOFF_CV_INPUT].isConnected());
    reflect->peakKnob->setIndicatorActive(inputs[RESONANCE_CV_INPUT].isConnected());
    reflect->driveKnob->setIndicatorActive(inputs[DRIVE_CV_INPUT].isConnected());

    reflect->frqKnob->setIndicatorValue(params[CUTOFF_PARAM].getValue() + frqcv);
    reflect->peakKnob->setIndicatorValue(params[RESONANCE_PARAM].getValue() + rescv);
    reflect->driveKnob->setIndicatorValue(params[DRIVE_PARAM].getValue() + drvcv);


    float y = inputs[FILTER_INPUT].getVoltage();

    filter->setIn(y);
    filter->process();

    outputs[LP_OUTPUT].setVoltage(filter->getLpOut());

    lights[OVERLOAD_LIGHT].value = filter->getLightValue();
}


Model *modelAlmaFilter = createModel<AlmaFilter, AlmaFilterWidget>("VCF");
