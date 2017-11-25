#include "LindenbergResearch.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;


void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "Lindenberg Research";
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<SimpleFilterWidget>("Lindenberg Research", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("Lindenberg Research", "BlankPanel", "Blank Panel 20TE", UTILITY_TAG));
    p->addModel(createModel<ReShaperWidget>("Lindenberg Research", "ReShaper", "ReShaper Wavefolder", FILTER_TAG));
    p->addModel(createModel<VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG));
}


void LCDWidget::draw(NVGcontext *vg) {
    nvgFontSize(vg, 18);
    nvgFontFaceId(vg, gLCDFont->handle);
    nvgTextLetterSpacing(vg, 0);

    nvgFillColor(vg, bg);

    std::string s1;
    std::string s2;
    std::string s3;

    int length = sizeof(text.c_str());

    for (int i = 0; i < length; ++i) {
        s1.append("8");
        s2.append(":");
        //s3.append("X");
    }

    nvgTextBox(vg, 0, 0, 220, s1.c_str(), nullptr);
    nvgTextBox(vg, 0, 0, 220, s2.c_str(), nullptr);
    nvgTextBox(vg, 0, 0, 220, s3.c_str(), nullptr);

    nvgFillColor(vg, fg);
    nvgTextBox(vg, 0, 0, 220, text.c_str(), nullptr);
}

