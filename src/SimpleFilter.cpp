#include "SimpleFilter.hpp"
#include "LindenbergResearch.hpp"

struct SimpleFilter : Module {
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
        FILTER2_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        BLINK_LIGHT,
        NUM_LIGHTS
    };

    float f, p, q;
    float b0, b1, b2, b3, b4;
    float t1, t2;
    float frequency, resonance, in;

    SimpleFilter() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
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
    }

    void step() override;


    // For more advanced Module features, read Rack's engine.hpp header file
    // - toJson, fromJson: serialization of internal data
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

void SimpleFilter::step() {
    // Moog 24 dB/oct resonant lowpass VCF
    // References: CSound source code, Stilson/Smith CCRMA paper.
    // Modified by paul.kellett@maxim.abel.co.uk July 2000
    //
    // Adapted for VCV Rack by Lindenberg Research
    // http://musicdsp.org/showArchiveComment.php?ArchiveID=25

    // calculate CV inputs
    float cutoffCVValue = (inputs[CUTOFF_CV_INPUT].value * 0.05f * params[CUTOFF_CV_PARAM].value);
    float resonanceCVValue = (inputs[RESONANCE_CV_INPUT].value * 0.05f * params[RESONANCE_CV_PARAM].value);

    // translate frequency to logarithmic scale
    float freqinhz = 20.f * powf(1000.f, params[CUTOFF_PARAM].value + cutoffCVValue);
    frequency = clip(freqinhz * (1.f / (engineGetSampleRate() / 2.0f)), 1.f);


    // determine correct parameters with CV values
    resonance = clip(params[RESONANCE_PARAM].value + resonanceCVValue, 1.f);

    // normalize signal input to [-1.0...+1.0]
    in = clip(inputs[FILTER_INPUT].value * 0.1f, 1.0f);

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
    outputs[FILTER_OUTPUT].value = clip(b4, 1.0f) * 5.0f;
    outputs[FILTER2_OUTPUT].value = clip(b4, 1.0f) * 5.0f;

    lights[BLINK_LIGHT].value = b4;
}


SimpleFilterWidget::SimpleFilterWidget() {
    SimpleFilter *module = new SimpleFilter();
    setModule(module);
    box.size = Vec(MODULE_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/Panel.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewBlack>(Vec(15, 0)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 30, 0)));
    addChild(createScrew<ScrewBlack>(Vec(15, 365)));
    addChild(createScrew<ScrewBlack>(Vec(box.size.x - 30, 365)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(createParam<LRBasicKnobWhite>(Vec(12, 200), module, SimpleFilter::CUTOFF_PARAM, 0.f, 1.f, 0.f));
    addParam(createParam<LRBasicKnobWhite>(Vec(100 - 32, 200), module, SimpleFilter::RESONANCE_PARAM, -0.f, 1.f, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRBasicKnobWhite>(Vec(12, 130), module, SimpleFilter::CUTOFF_CV_PARAM, 0.f, 1.f, 0.f));
    addParam(createParam<LRBasicKnobWhite>(Vec(100 - 32, 130), module, SimpleFilter::RESONANCE_CV_PARAM, 0.f, 1.f, 0.f));

    addInput(createInput<PJ301MPort>(Vec(20, 70), module, SimpleFilter::CUTOFF_CV_INPUT));
    addInput(createInput<PJ301MPort>(Vec(100 - 24, 70), module, SimpleFilter::RESONANCE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<PJ301MPort>(Vec(60 - 12, 285), module, SimpleFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<PJ301MPort>(Vec(20, 255), module, SimpleFilter::FILTER_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(100 - 24, 255), module, SimpleFilter::FILTER2_OUTPUT));
    // ***** OUTPUTS *********

    // ***** LED *************
    addChild(createLight<MediumLight<RedLight>>(Vec(55, 200), module, SimpleFilter::BLINK_LIGHT));
    // ***** LED *************
}
