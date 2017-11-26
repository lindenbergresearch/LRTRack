#include "LindenbergResearch.hpp"

using namespace rack;

Plugin *plugin;

/**
 * @brief
 * @param p
 */
void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "Lindenberg Research";
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<SimpleFilterWidget>("Lindenberg Research", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("Lindenberg Research", "BlankPanel", "Blank Panel 20TE", UTILITY_TAG));
    p->addModel(createModel<ReShaperWidget>("Lindenberg Research", "ReShaper", "ReShaper Wavefolder", FILTER_TAG));
    p->addModel(createModel<VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG));
}

/**
 * @brief Draw method of custom LCD widget
 * @param vg
 */
void LCDWidget::draw(NVGcontext *vg) {
    nvgFontSize(vg, 18);
    nvgFontFaceId(vg, gLCDFont_DIG7->handle);
    nvgTextLetterSpacing(vg, 0);

    nvgFillColor(vg, bg);

    std::string s1;
    std::string s2;

    for (int i = 0; i < LCDWidget::length; ++i) {
        s1.append("8");
        s2.append(":");
    }

    nvgTextBox(vg, 0, 0, 220, s1.c_str(), nullptr);
    nvgTextBox(vg, 0, 0, 220, s2.c_str(), nullptr);

    nvgFillColor(vg, fg);
    nvgTextBox(vg, 0, 0, 220, text.c_str(), nullptr);
}

/**
 * @brief Custom step implementation for LRT Modules
 */
void LRTModule::step() {
    Module::step();

    // increment counter
    cnt++;
}
