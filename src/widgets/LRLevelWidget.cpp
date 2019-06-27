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
\*
 */
#include "../LRComponents.hpp"
#include "../LRModel.hpp"

using namespace rack;
using namespace rack::widget;


namespace lrt {

LRRoundRectLevelLED::LRRoundRectLevelLED() {}


void LRRoundRectLevelLED::drawShape(NVGcontext *vg) {
    //LRLevelLED::drawShape(vg);
    nvgBeginPath(vg);
    //nvgRect(vg, getAbsX(), getAbsY(), size.x, size.y);
    nvgRoundedRect(vg, getAbsX(), getAbsY(), size.x, size.y, 1.f);
    nvgFillColor(vg, enabled ? bodyOn : bodyOff);
    nvgFill(vg);

    nvgStrokeWidth(vg, 1.f);
    nvgStrokeColor(vg, frame);
    nvgStroke(vg);
}


void LRRectLevelLED::drawShape(NVGcontext *vg) {
    //LRLevelLED::drawShape(vg);
    nvgBeginPath(vg);
    nvgRect(vg, getAbsX(), getAbsY(), size.x, size.y);
    nvgFillColor(vg, enabled ? bodyOn : bodyOff);
    nvgFill(vg);

    nvgStrokeWidth(vg, 1.f);
    nvgStrokeColor(vg, frame);
    nvgStroke(vg);
}


LRRectLevelLED::LRRectLevelLED() {}


LRRectLevelLED::~LRRectLevelLED() {}


}