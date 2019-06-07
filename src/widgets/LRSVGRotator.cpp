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

SVGRotator::SVGRotator() : FramebufferWidget() {
    tw = new TransformWidget();
    addChild(tw);

    sw = new SvgWidget();
    tw->addChild(sw);

    oversample = 2.0;
}


/**
 * @brief Set SVG image to rotator
 * @param svg
 */
void SVGRotator::setSvg(std::shared_ptr<Svg> svg) {
    sw->setSvg(svg);
    tw->box.size = sw->box.size;
    box.size = sw->box.size;
}


/**
 * @brief Rotate one step
 */
void SVGRotator::step() {
    tw->identity();

    angle = fmodf(angle + inc, 2 * (float) M_PI);

    Vec center = sw->box.getCenter();
    tw->translate(center);
    tw->rotate(angle);
    tw->scale(Vec(scale, scale));
    tw->translate(center.neg());

    dirty = true;
    fbSize = Vec(1, 1); // invalidate

    FramebufferWidget::step();
}


void SVGRotator::draw(const Widget::DrawArgs &args) {
    nvgGlobalAlpha(args.vg, transperency);
    FramebufferWidget::draw(args);
}
}