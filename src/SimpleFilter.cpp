#include "SimpleFilter.hpp"
#include "LindenbergResearch.hpp"


struct SimpleFilter : Module {
    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        DRIVE_PARAM,
        CUTOFF_CV_PARAM,
        RESONANCE_CV_PARAM,
        DRIVE_CV_PARAM,
        DRIVE_CV,
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
        LP24_OUTPUT,
        HP6_OUTPUT,
        BP6_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    float f, p, q;
    float b0, b1, b2, b3, b4;
    float t1, t2;
    float frequency, resonance, in;


    SimpleFilter() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {
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


void SimpleFilter::step() {
    // Moog 24 dB/oct resonant lowpass VCF
    // References: CSound source code, Stilson/Smith CCRMA paper.
    // Modified by paul.kellett@maxim.abel.co.uk July 2000
    //
    // Adapted for VCV Rack by Lindenberg Research
    // http://musicdsp.org/showArchiveComment.php?ArchiveID=25

    // calculate CV inputs
    float cutoffCVValue = (inputs[CUTOFF_CV_INPUT].value * 0.05f * params[CUTOFF_CV_PARAM].value);
    float resonanceCVValue = (inputs[RESONANCE_CV_INPUT].value * 0.1f * params[RESONANCE_CV_PARAM].value);

    // translate frequency to logarithmic scale
    float freqHz = 20.f * powf(1000.f, params[CUTOFF_PARAM].value + cutoffCVValue);
    frequency = clampf(freqHz * (1.f / (engineGetSampleRate() / 2.0f)), -1.f, 1.f);
    resonance = clampf(params[RESONANCE_PARAM].value + resonanceCVValue, -1.f, 1.f);

    // normalize signal input to [-1.0...+1.0]
    // filter starts to be very unstable for input gain above 1.f and below 0.f
    in = clampf(inputs[FILTER_INPUT].value * 0.1f, -1.f, 1.f);

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
    outputs[LP24_OUTPUT].value = clampf(b4, -1.f, 1.0f) * 10.0f;
}


SimpleFilterWidget::SimpleFilterWidget() {
    SimpleFilter *module = new SimpleFilter();

    setModule(module);
    box.size = Vec(MODULE_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/VCF.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 365)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 365)));
    // ***** SCREWS **********


    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(62.5, 150.954), module, SimpleFilter::CUTOFF_PARAM, 0.f, 1.f, 0.f));
    addParam(createParam<LRMiddleKnob>(Vec(32.69, 233.872), module, SimpleFilter::RESONANCE_PARAM, -0.f, 1.f, 0.0f));
    addParam(createParam<LRMiddleKnob>(Vec(107.309, 233.872), module, SimpleFilter::DRIVE_PARAM, -0.f, 1.f, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(27.874, 106.365), module, SimpleFilter::RESONANCE_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(78.5, 106.365), module, SimpleFilter::CUTOFF_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(127.647, 106.365), module, SimpleFilter::DRIVE_CV_PARAM, -1.f, 1.f, 0.f));

    addInput(createInput<IOPort>(Vec(26.589, 51.821), module, SimpleFilter::RESONANCE_CV_INPUT));
    addInput(createInput<IOPort>(Vec(76.5, 51.821), module, SimpleFilter::CUTOFF_CV_INPUT));
    addInput(createInput<IOPort>(Vec(125.647, 51.821), module, SimpleFilter::DRIVE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(22.621, 326.252), module, SimpleFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(57.5, 326.252), module, SimpleFilter::LP24_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(95.141, 326.252), module, SimpleFilter::BP6_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(129.616, 326.252), module, SimpleFilter::HP6_OUTPUT));
    // ***** OUTPUTS *********
}
