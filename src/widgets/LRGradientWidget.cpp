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

/**
 * @brief Draw routines for gradient effect
 * @param args.vg
 */
void LRGradientWidget::draw(const Widget::DrawArgs &args) {
    nvgBeginPath(args.vg);
    nvgRect(args.vg, -margin, -margin, box.size.x + margin * 2, box.size.y + margin * 2);

    NVGpaint paint = nvgLinearGradient(args.vg, v1.x, v1.y, v2.x, v2.y, innerColor, outerColor);
    nvgFillPaint(args.vg, paint);
    nvgFill(args.vg);

}


const NVGcolor &LRGradientWidget::getInnerColor() const {
    return innerColor;
}


void LRGradientWidget::setInnerColor(const NVGcolor &innerColor) {
    LRGradientWidget::innerColor = innerColor;
}


const NVGcolor &LRGradientWidget::getOuterColor() const {
    return outerColor;
}


void LRGradientWidget::setOuterColor(const NVGcolor &outerColor) {
    LRGradientWidget::outerColor = outerColor;
}

}