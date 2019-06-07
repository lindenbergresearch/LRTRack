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

using namespace lrt;


LRScrew::LRScrew() {
    tw = new TransformWidget();
    addChild(tw);

    sw = new SvgWidget();
    tw->addChild(sw);

    sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkC.svg"));
    sw->wrap();
    box.size = sw->box.size;

    angle = 0.f;
    oversample = 2.0;

    addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkC.svg")));
    addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkLightC.svg")));
    addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkLightC.svg")));
}


void LRScrew::onGestaltChangeAction(LRGestaltChangeEvent &e) {
    sw->svg = getSVGVariant(e.current);
    sw->wrap();

    dirty = true;
    fbSize = Vec(1, 1); // invalidate
}


void LRScrew::rotate(float amount) {
    tw->identity();

    angle = fmodf(angle + amount, 2 * (float) M_PI);

    Vec center = sw->box.getCenter();
    tw->translate(center);
    tw->rotate(angle);
    tw->translate(center.neg());

    invalidate();
}


void LRScrew::onButton(const event::Button &e) {
    Widget::onButton(e);
    if (e.button == 0 && e.action == 1)
        rotate(random::uniform() * 0.3 + 0.13);
}
