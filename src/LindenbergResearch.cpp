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
#ifdef VERSION
    p->version = TOSTRING(VERSION);
#endif
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<AlmaFilterWidget>("Lindenberg Research", "VCF", "ALMA Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("Lindenberg Research", "BlankPanel 01", "Blank Panel 20TE", BLANK_TAG));
    p->addModel(createModel<BlankPanelWidgetM1>("Lindenberg Research", "BlankPanel 02", "Blank Panel 12TE", BLANK_TAG));
    p->addModel(createModel<ReShaperWidget>("Lindenberg Research", "ReShaper", "ReShaper Wavefolder", WAVESHAPER_TAG));
    p->addModel(createModel<SimpleFilterWidget>("Lindenberg Research", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));

    // createModel<SimpleFilterWidget>(plugin, "LPFilter24dB", "24dB Lowpass Filter");
    // not ready jet
    //p->addModel(createModel<VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG));
}


/**
 * @brief Draw method of custom LCD widget
 * @param vg
 */
void LCDWidget::draw(NVGcontext *vg) {
    nvgFontSize(vg, LCD_FONTSIZE);
    nvgFontFaceId(vg, gLCDFont_DIG7->handle);
    nvgTextLetterSpacing(vg, LCD_LETTER_SPACING);

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


void LRRedLight::draw(NVGcontext *vg) {
    //LightWidget::draw(vg);

    float radius = box.size.x / 1.5f;
    float oradius = radius + 10.0f;

    /* color.r = clampf(color.r, 0.0f, 1.0f);
     color.g = clampf(color.g, 0.0f, 1.0f);
     color.b = clampf(color.b, 0.0f, 1.0f);
     color.a = clampf(color.a, 0.0f, 1.0f);*/

    // Solid
    nvgBeginPath(vg);
    nvgCircle(vg, radius, radius, radius);
    nvgFillColor(vg, bgColor);
    nvgFill(vg);

    // Border
    nvgStrokeWidth(vg, 1.0f);
    NVGcolor borderColor = bgColor;
    borderColor.a *= 0.5f;
    nvgStrokeColor(vg, borderColor);
    nvgStroke(vg);

    // Inner glow
    nvgGlobalCompositeOperation(vg, NVG_LIGHTER);
    nvgFillColor(vg, color);
    nvgFill(vg);

    // Outer glow
    nvgBeginPath(vg);
    nvgRect(vg, radius - oradius, radius - oradius, 2 * oradius, 2 * oradius);
    NVGpaint paint;
    NVGcolor icol = color;
    icol.a *= 0.40f;
    NVGcolor ocol = color;
    ocol.a = 0.00f;
    paint = nvgRadialGradient(vg, radius, radius, radius, oradius, icol, ocol);
    nvgFillPaint(vg, paint);
    nvgFill(vg);
}


/**
 * @brief
 */
LRRedLight::LRRedLight() {
    addBaseColor(COLOR_RED);
}
