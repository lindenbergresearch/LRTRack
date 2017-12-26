#include "dsp/LadderFilter.hpp"
#include "SimpleFilter.hpp"
#include "dsp/DSPMath.hpp"
#include "LindenbergResearch.hpp"


struct SimpleFilter : LRTModule {

    enum ParamIds {
        CUTOFF_PARAM,
        RESONANCE_PARAM,
        DRIVE_PARAM,
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
        HP_OUTPUT,
        BP_OUTPUT,
        NUM_OUTPUTS
    };

    enum LightIds {
        NUM_LIGHTS
    };

    LCDWidget *label1 = new LCDWidget(LCD_COLOR_FG, 12);
    LCDWidget *label2 = new LCDWidget(LCD_COLOR_FG, 12);

    LadderFilter filter;

    SimpleFilter() : LRTModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}


    void step() override;
};


void SimpleFilter::step() {
    cnt++;
    filter.setFrequency(params[CUTOFF_PARAM].value);
    filter.setResonance(params[RESONANCE_PARAM].value);
    filter.setDrive(params[DRIVE_PARAM].value * params[DRIVE_PARAM].value);

    float y = clampf(inputs[FILTER_INPUT].value / 50, -0.6, 0.6);

    filter.setIn(y);

    filter.process();

    outputs[LP_OUTPUT].value = filter.getLpOut() * 50;
    outputs[HP_OUTPUT].value = filter.getHpOut() * 50;
    outputs[BP_OUTPUT].value = filter.getBpOut() * 50;


    /*  double cut_cv = inputs[CUTOFF_CV_INPUT].value * 0.05;
      double res_cv = inputs[RESONANCE_CV_INPUT].value * 0.05;

      // compute CV inputs
      double cutoff = (cut_cv * quadraticBipolar(params[CUTOFF_CV_PARAM].value));
      double res = (res_cv * quadraticBipolar(params[RESONANCE_CV_PARAM].value));


      // translate frequency to logarithmic scale
      freqHz = 20. * pow(1000., params[CUTOFF_PARAM].value + cutoff);
      frequency = clampd(freqHz * (1. / (engineGetSampleRate() / 2.0)), 0., 1.);

      resonance = clampd(params[RESONANCE_PARAM].value + res, 0, 1.1);
      // add some curve to resonance to avoid aliasing at high frequency
      resonance *= (-0.3 * frequency + 1);

      // stringf("%f", freqHz);
      //  label2->text = stringf("%-.6f", resonance);

      // normalize signal input to [-1.0...+1.0]
      // filter starts to be very unstable for input gain above 1.f and below 0.f
      //in = clampd(inputs[FILTER_INPUT].value / 15., -0.7, 0.7);

      in = clip(inputs[FILTER_INPUT].value / 10, 0.8, -0.8);

      // Set coefficients given frequency & resonance [0.0...1.0]
      q = 1.0 - frequency;
      p = frequency + 0.8 * frequency * q;
      f = p + p - 1.0;
      q = resonance * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));


      in -= q * b4;

      t1 = b1;
      b1 = (in + b0) * p - b1 * f;

      t2 = b2;
      b2 = (b1 + t1) * p - b2 * f;

      t1 = b3;
      b3 = (b2 + t2) * p - b3 * f;

      b4 = (b3 + t1) * p - b4 * f;

      b4 = b4 - b4 * b4 * b4;
      b0 = in;

      double drv = params[DRIVE_PARAM].value;
      double out = 0;

      out = clip(b4, 0.8 + drv, -(drv + 0.8)) * 10;


      if (outputs[LP_OUTPUT].active) {
          outputs[LP_OUTPUT].value = out;
      }*/

    //   outputs[BP_OUTPUT].value = (float) clampd(((b3 - b4) * 60.0), -10, 10);
    //  outputs[HP_OUTPUT].value = (float) clampd((((in - 3.0 * (b3 - b4)) - b4) * 15.0), -10, 10);

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
    addChild(createScrew<ScrewDarkA>(Vec(15, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 1)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 366)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 366)));
    // ***** SCREWS **********

    // ***** MAIN KNOBS ******
    addParam(createParam<LRBigKnob>(Vec(62.5, 150.954), module, SimpleFilter::CUTOFF_PARAM, 0.f, 1.f, 0.f));
    addParam(createParam<LRMiddleKnob>(Vec(32.69, 233.872), module, SimpleFilter::RESONANCE_PARAM, -0.f, 1.5, 0.0f));
    addParam(createParam<LRMiddleKnob>(Vec(107.309, 233.872), module, SimpleFilter::DRIVE_PARAM, 0.0f, 1.f, 0.0f));
    // ***** MAIN KNOBS ******

    // ***** CV INPUTS *******
    addParam(createParam<LRSmallKnob>(Vec(27, 106), module, SimpleFilter::RESONANCE_CV_PARAM, -1.f, 1.5f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(78, 106), module, SimpleFilter::CUTOFF_CV_PARAM, -1.f, 1.f, 0.f));
    addParam(createParam<LRSmallKnob>(Vec(127, 106), module, SimpleFilter::DRIVE_CV_PARAM, -1.f, 1.f, 0.f));

    addInput(createInput<IOPort>(Vec(24, 52), module, SimpleFilter::RESONANCE_CV_INPUT));
    addInput(createInput<IOPort>(Vec(74, 52), module, SimpleFilter::CUTOFF_CV_INPUT));
    addInput(createInput<IOPort>(Vec(123, 52), module, SimpleFilter::DRIVE_CV_INPUT));
    // ***** CV INPUTS *******

    // ***** INPUTS **********
    addInput(createInput<IOPort>(Vec(23, 326), module, SimpleFilter::FILTER_INPUT));
    // ***** INPUTS **********

    // ***** OUTPUTS *********
    addOutput(createOutput<IOPort>(Vec(57, 326), module, SimpleFilter::HP_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(94, 326), module, SimpleFilter::BP_OUTPUT));
    addOutput(createOutput<IOPort>(Vec(129, 326), module, SimpleFilter::LP_OUTPUT));
    // ***** OUTPUTS *********

    module->label1->box.pos = Vec(56, 235);
    addChild(module->label1);

    module->label2->box.pos = Vec(23, 300);
    addChild(module->label2);
}
