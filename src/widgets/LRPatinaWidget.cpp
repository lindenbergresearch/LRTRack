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
**    Copyright 2017/2018 by Patrick Lindenberg / LRT                  **
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
    svg = new SVGWidget();
    svg->setSVG(SVG::load(assetPlugin(plugin, filename)));

    box.size = size;
}


/**
 * @brief Randomize patina svg offset and trigger redraw
 */
void LRPatinaWidget::randomize() {
    float maxx = svg->box.size.x - box.size.x;
    float maxy = svg->box.size.y - box.size.y;

    svg->box.pos = Vec(-randomUniform() * maxx, -randomUniform() * maxy);

    dirty = true;
}