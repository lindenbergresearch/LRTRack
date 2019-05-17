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

LRKnob::LRKnob() {
    minAngle = -ANGLE * (float) M_PI;
    maxAngle = ANGLE * (float) M_PI;

    //gestalt = module->gestalt;

    shader = new LRShadow();
    removeChild(shadow); // uninstall default

    font = APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));

    indicator = new LRCVIndicator(15.f, ANGLE);
    // addChild(indicator);
}


void LRKnob::setSVG(std::shared_ptr<SVG> svg) {
    SvgKnob::setSVG(svg);

    /** inherit dimensions after loaded svg */
    indicator->box.size = sw->box.size;
    indicator->middle = Vec(box.size.x / 2, box.size.y / 2);
    shader->setBox(box);
}


void LRKnob::draw(const Widget::DrawArgs &args) {
    /** shadow */
    shader->draw(args.vg);

    /** component */
    SvgKnob::draw(args);

    auto value = paramQuantity->getValue();

    if (lightning) {
        nvgBeginPath(args.vg);

        auto gradient = nvgLinearGradient(args.vg, box.size.x / 2 - radius / 2 * 1.1, box.size.y / 2 - radius / 2 * 1.1,
                                          box.size.x / 2 + radius / 2 * 1.1,
                                          box.size.y / 2 + radius / 2 * 1.1,
                                          startColor,
                                          endColor);

        nvgCircle(args.vg, box.size.x / 2, box.size.y / 2, radius);

        nvgFillPaint(args.vg, gradient);
        nvgFill(args.vg);
    }

    indicator->draw(args);

    /** debug numerical values */
    if (debug) {
        auto text = stringf("%4.3f", value);
        auto size = box.size.x / 2. > 15 ? 15 : box.size.x / 2.;
        nvgFontSize(args.vg, size);

        nvgFontFaceId(args.vg, font->handle);
        nvgTextAlign(args.vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        float bounds[4];

        nvgTextBounds(args.vg, 0, 0, text.c_str(), nullptr, bounds);

        nvgBeginPath(args.vg);
        nvgFillColor(args.vg, nvgRGBAf(0., 0.1, 0.2, 0.8));
        nvgRoundedRect(args.vg, bounds[0] - 4, bounds[1] - 2, (bounds[2] - bounds[0]) + 8, (bounds[3] - bounds[1]) + 4,
                       ((bounds[3] - bounds[1]) + 4) / 2 - 1);
        nvgFill(args.vg);

        nvgFillColor(args.vg, nvgRGBAf(1.0f, 1.0f, 1.0f, .99f));
        nvgText(args.vg, 0, 0, text.c_str(), NULL);
    }
}


void LRKnob::setSnap(float position, float sensitivity) {
    snap = true;
    snapSens = sensitivity;
    snapAt = position;
}


void LRKnob::unsetSnap() {
    snap = false;
}


void LRKnob::onChange(const event::Change &e) {
    auto value = paramQuantity->getValue();

    // if the value still inside snap-tolerance keep the value zero
    if (snap && value > -snapSens + snapAt && value < snapSens + snapAt) value = 0;
    SvgKnob::onChange(e);
}


void LRKnob::onGestaltChange(LREventGestaltChange &e) {
    auto svg = getSVGVariant(*gestalt);

    if (svg != nullptr) {
        setSVG(svg);
    }

    switch (*gestalt) {
        case DARK:
            indicator->lightMode = false;
            break;
        case LIGHT:
            indicator->lightMode = true;
            break;
        case AGED:
            indicator->lightMode = true;
            break;
        default:
            indicator->lightMode = false;
    }


    dirty = true;
    e.consumed = true;
}


}