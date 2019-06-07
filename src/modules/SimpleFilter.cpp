#include "../LindenbergResearch.hpp"
#include "../LRModel.hpp"


using namespace rack;
using namespace lrt;

struct SimpleFilterWidget;

struct SimpleFilter : LRModule {
    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        CUTOFF_CV_PARAM,
        RESONANCE_CV_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        FILTER_INPUT,
        CUTOFF_CV_INPUT,
        RESONANCE_CV_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        FILTER_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    float f, p, q;
    float b0, b1, b2, b3, b4;
    float t1, t2;
    float frequency, resonance, in;

    SimpleFilterWidget *reflect;


    SimpleFilter() : LRModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        f = 0;
        p = 0;
        q = 0;
        b0 = 0;
        b1 = 0;
        b2 = 0;
        b3 = 0;
        b4 = 0;
        t1 = 0;
        t2 = 0;
        frequency = 0;
        resonance = 0;
        in = 0;

        configParam(CUTOFF_PARAM, 0.f, 1.f, 0.f);
        configParam(RESONANCE_PARAM, 0.f, 1.f, 0.f);
        configParam(CUTOFF_CV_PARAM, 0.f, 1.f, 0.f);
        configParam(RESONANCE_CV_PARAM, 0.f, 1.f, 0.f);
    }


    void process(const ProcessArgs &args) override;


    // For more advanced Module features, read Rack's engine.hpp header file
    // - dataToJson, dataFromJson: serialization of internal data
    // - onSampleRateChange: event triggered by a change of sample rate
    // - reset, randomize: implements special behavior when user clicks these from the context menu
};


float clip(float in, float level) {
    // clipping high
    if (in > level) {
        in = level;
    }

    // clipping low
    if (in < -level) {
        in = -level;
    }

    return in;
}


/**
 * @brief Recover of old filer
 */
struct SimpleFilterWidget : LRModuleWidget {
    LRBigKnob *frqKnob;
    LRMiddleKnob *resKnob;

    SimpleFilterWidget(SimpleFilter *module);
};


SimpleFilterWidget::SimpleFilterWidget(SimpleFilter *module) : LRModuleWidget(module) {
    panel->addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/panels/SimpleFilter.svg")));
    //panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/SimpleFilter.svg")));
    // panel->addSVGVariant(APP->window->loadSvg(asset::plugin(plugin, "res/panels/SimpleFilter.svg")));

    auto newGestalt = DARK;

    noVariants = true;
    panel->init();
    gestalt = newGestalt;
    addChild(panel);
    box.size = panel->box.size;

    // reflect module widget
    if (!isPreview) module->reflect = this;

    // ***** SCREWS **********
//    addChild(createWidget<ScrewLight>(Vec(15, 1)));
//    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 1)));
//    addChild(createWidget<ScrewLight>(Vec(15, 366)));
//    addChild(createWidget<ScrewLight>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    frqKnob = createParam<LRBigKnob>(Vec(46.9, 171.5), module, SimpleFilter::CUTOFF_PARAM);
    resKnob = createParam<LRMiddleKnob>(Vec(54.0, 254.0), module, SimpleFilter::RESONANCE_PARAM);

    addParam(frqKnob);
    addParam(resKnob);
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(27, 122), module, SimpleFilter::CUTOFF_CV_PARAM));
    addParam(createParam<LRSmallKnob>(Vec(99, 122), module, SimpleFilter::RESONANCE_CV_PARAM));

    addInput(createInput<LRIOPortCV>(Vec(25.4, 52.9), module, SimpleFilter::CUTOFF_CV_INPUT));
    addInput(createInput<LRIOPortCV>(Vec(97.2, 52.9), module, SimpleFilter::RESONANCE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<LRIOPortAudio>(Vec(25.4, 324.4), module, SimpleFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<LRIOPortAudio>(Vec(97.2, 324.4), module, SimpleFilter::FILTER_OUTPUT));
    // ***** OUTPUTS *********
}


void SimpleFilter::process(const ProcessArgs &args) {
    // Moog 24 dB/oct resonant lowpass VCF
    // References: CSound source code, Stilson/Smith CCRMA paper.
    // Modified by paul.kellett@maxim.abel.co.uk July 2000
    //
    // Adapted for VCV Rack by Lindenberg Research
    // http://musicdsp.org/showArchiveComment.php?ArchiveID=25

    // calculate CV inputs
    float cutoffCVValue = (inputs[CUTOFF_CV_INPUT].getVoltage() * 0.05f * params[CUTOFF_CV_PARAM].getValue());
    float resonanceCVValue = (inputs[RESONANCE_CV_INPUT].getVoltage() * 0.1f * params[RESONANCE_CV_PARAM].getValue());

    // translate frequency to logarithmic scale
    float freqHz = 20.f * powf(1000.f, params[CUTOFF_PARAM].getValue() + cutoffCVValue);
    frequency = clip(freqHz * (1.f / (args.sampleRate / 2.0f)), 1.f);
    resonance = clip(params[RESONANCE_PARAM].getValue() + resonanceCVValue, 1.f);

    reflect->frqKnob->setIndicatorActive(inputs[CUTOFF_CV_INPUT].isConnected());
    reflect->resKnob->setIndicatorActive(inputs[RESONANCE_CV_INPUT].isConnected());

    reflect->frqKnob->setIndicatorValue(params[CUTOFF_PARAM].getValue() + cutoffCVValue);
    reflect->resKnob->setIndicatorValue(params[RESONANCE_PARAM].getValue() + resonanceCVValue);


    // normalize signal input to [-1.0...+1.0]
    // lpf starts to be very unstable for input gain above 1.f and below 0.f
    in = clip(inputs[FILTER_INPUT].getVoltage() * 0.1f, 1.0f);

    // Set coefficients given frequency & resonance [0.0...1.0]
    q = 1.0f - frequency;
    p = frequency + 0.8f * frequency * q;
    f = p + p - 1.0f;
    q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));


    in -= q * b4;

    t1 = b1;
    b1 = (in + b0) * p - b1 * f;

    t2 = b2;
    b2 = (b1 + t1) * p - b2 * f;

    t1 = b3;
    b3 = (b2 + t2) * p - b3 * f;

    b4 = (b3 + t1) * p - b4 * f;

    b4 = b4 - b4 * b4 * b4 * 0.166666667f;
    b0 = in;

    // Lowpass  output:  b4
    // Highpass output:  in - b4;
    // Bandpass output:  3.0f * (b3 - b4);


    // scale normalized output back to +/-5V
    outputs[FILTER_OUTPUT].setVoltage(clip(b4, 1.0f) * 5.0f);
}

Model *modelSimpleFilter = createModel<SimpleFilter, SimpleFilterWidget>("LPFilter24dB");
