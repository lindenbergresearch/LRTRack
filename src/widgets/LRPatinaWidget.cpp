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
using std::string;


/**
 * Standard constructor with a given filename
 */
LRPatinaWidget::LRPatinaWidget(const string &filename, const Vec &size) {
    svg = new SvgWidget();
    svg->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));

    addChild(svg);
    box.size = size;
}


/**
 * @brief Randomize patina svg offset and trigger redraw
 */
void LRPatinaWidget::randomize() {
    float maxx = svg->box.size.x - box.size.x;
    float maxy = svg->box.size.y - box.size.y;

    svg->box.pos = Vec(-random::uniform() * maxx, -random::uniform() * maxy);
}


/**
 * @brief Override draw to set global (widget) transparency (strength)
 * @param args.vg
 */
void LRPatinaWidget::draw(const Widget::DrawArgs &args) {
    nvgGlobalAlpha(args.vg, strength);
    TransparentWidget::draw(args.vg);
}
