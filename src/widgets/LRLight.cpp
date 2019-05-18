/*                                                                     *\
**       __   ___  ______                                              **
**      / /  / _ \/_  __/                                              **
**     / /__/ , _/ / /    Lindenberg                                   **
**    /____/_/|_| /_/  Research Tec.                                   **
**                                                                     **
**                                                                     **
**	  https://github.com/lindenbergresearch/LRTRack	                   **
**    heapdump@icloud.com                                              **
**		                                                               **
**    Sound Modules for VCV Rack                                       **
**    Copyright 2017-2019 by Patrick Lindenberg / LRT                  **
**                                                                     **
**    For Redistribution and use in source and binary forms,           **
**    with or without modification please see LICENSE.                 **
**                                                                     **
\*                                                                     */
#include "../LRComponents.hpp"

namespace lrt {

void LRLight::draw(const Widget::DrawArgs &args) {
    float radius = box.size.x / 1.5f;
    float oradius = radius + 24.0f;

    // Solid
    nvgBeginPath(args.vg);
    nvgCircle(args.vg, radius, radius, radius);
    nvgFillColor(args.vg, bgColor);
    nvgFill(args.vg);

    // Border
    nvgStrokeWidth(args.vg, 1.0f);
    // NVGcolor borderColor = nvgRGBAf(0.01, 0.03, 0.09, 0.9);
    borderColor.a *= 0.5f;
    nvgStrokeColor(args.vg, borderColor);
    nvgStroke(args.vg);

    // Inner glow
    nvgGlobalCompositeOperation(args.vg, NVG_LIGHTER);
    nvgFillColor(args.vg, color);
    nvgFill(args.vg);

    // Outer glow
    nvgBeginPath(args.vg);
    nvgRect(args.vg, radius - oradius, radius - oradius, 2 * oradius, 2 * oradius);
    NVGpaint paint;
    NVGcolor icol = color;
    icol.a *= glowIntensity;
    NVGcolor ocol = color;
    ocol.a = 0.00f;
    paint = nvgRadialGradient(args.vg, radius, radius, radius, oradius, icol, ocol);
    nvgFillPaint(args.vg, paint);
    nvgFill(args.vg);
}


void LRLight::setColor(NVGcolor color) {
    LRLight::color = color;

    if (baseColors.empty()) addBaseColor(color);
    else baseColors[0] = color;

    borderColor = nvgRGBAf(color.r / 100, color.g / 100, color.b / 100, 1.0);
    bgColor = nvgRGBAf(color.r / 70, color.g / 70, color.b / 70, 1.0);
}


/**
 * @brief Constructor
 */
LRLight::LRLight() {
    box.size = Vec(7.5f, 7.5f);

    setColor(LED_DEFAULT_COLOR_DARK);
}


void LRLight::onGestaltChange(LREventGestaltChange &e) {
    LRGestaltChangeAction::onGestaltChange(e);

    switch (*gestalt) {
        case LRGestalt::DARK:
            setColor(LED_DEFAULT_COLOR_DARK);
            glowIntensity = 0.25; // does better effect on dark surfaces
            break;
        case LRGestalt::LIGHT:
            setColor(LED_DEFAULT_COLOR_LIGHT);
            glowIntensity = 0.25;
            break;
        case LRGestalt::AGED:
            setColor(LED_DEFAULT_COLOR_AGED);
            glowIntensity = 0.15;
            break;
        default:
            setColor(LED_DEFAULT_COLOR_DARK);
            break;
    }
}

}