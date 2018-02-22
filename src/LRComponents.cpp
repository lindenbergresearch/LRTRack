#include "LRComponents.hpp"
#include "LindenbergResearch.hpp"


/**
 * @brief Constructor of LCD Widget
 */
LCDWidget::LCDWidget(NVGcolor fg, unsigned char length) {
    /** load LCD ttf font */
    gLCDFont_DIG7 = Font::load(assetPlugin(plugin, LCD_FONT_DIG7));

    auto r = (unsigned char) (fg.r * 0xFF);
    auto g = (unsigned char) (fg.g * 0xFF);
    auto b = (unsigned char) (fg.b * 0xFF);

    LCDWidget::length = length;

    LCDWidget::fg = fg;
    LCDWidget::bg = nvgRGBA(r - 0x40, g - 0x40, b - 0x40, 0x40);
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
 * @brief Constructor
 */
LRRedLight::LRRedLight() {
    addBaseColor(COLOR_RED);
}


/**
 * @brief Draw routine for cv indicator
 * @param vg
 */
void Indicator::draw(NVGcontext *vg) {
    if (active) {
        float a = -angle + cv * (angle * 2);
        Vec p = Vec(15 - cos(a * M_PI) * radius, 15 - sin(a * M_PI) * radius);

        nvgBeginPath(vg);
        nvgCircle(vg, p.x, p.y, 2);
        nvgFillColor(vg, color);
        nvgFill(vg);
    }
}
