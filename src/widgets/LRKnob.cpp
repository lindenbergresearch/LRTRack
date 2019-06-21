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

    if (oversampled) fb->oversample = 2.f;

    shader = new LRShadow();
    if (shadow) shadow->visible = false; // uninstall default shadow

    //TODO: [2019-05-24 09:11] => redefine shader AND indicator to act as normal childs
    //fb->addChild(shader);

    indicator = new LRCVIndicator(15.f, ANGLE);
    // addChild(indicator);
    smooth = false;
}


void LRKnob::setSvg(std::shared_ptr<Svg> svg) {
    SvgKnob::setSvg(svg);

    /** inherit dimensions after loaded svg */
    shader->setBox(sw->box);
    indicator->box.size = sw->box.size;
    indicator->middle = Vec(box.size.x / 2, box.size.y / 2);
    shader->setBox(box);
}


void LRKnob::draw(const Widget::DrawArgs &args) {
    /** shadow */
    shader->draw(args);

    /** component */
    SvgKnob::draw(args);

    if (lightning) {
        nvgBeginPath(args.vg);

        auto gradient = nvgLinearGradient(
                args.vg,
                box.size.x / 2,
                box.size.x / 3,
                box.size.x / 2,
                box.size.y,
                startColor,
                endColor
        );

        /*auto gradient = nvgLinearGradient(args.vg, box.size.x / 2 - radius / 2 * 1.1 - 2, box.size.y / 2 - radius / 2 * 1.1 - 2,
                                          box.size.x / 2 + radius / 2 * 1.1,
                                          box.size.y / 2 + radius / 2 * 1.1,
                                          startColor,
                                          endColor);*/

        nvgCircle(args.vg, box.size.x / 2, box.size.y / 2, radius);

        nvgFillPaint(args.vg, gradient);
        nvgFill(args.vg);
    }

    /** cv indicator */
    if (indicator->active)
        indicator->draw(args);
}


void LRKnob::onGestaltChangeAction(LRGestaltChangeEvent &e) {
    indicator->onGestaltChangeAction(e); // forward event for non child widgets

    auto svg = getSVGVariant(e.current);

    if (svg != nullptr) {
        setSvg(svg);
    }

    fb->dirty = true;
}


void LRKnob::step() {
    ParamWidget::step();

    if (oversampled) fb->fbSize = Vec(1, 1);

    // redraw if cv indicator is turned on
    indicator->dirty = indicator->active;
}


}