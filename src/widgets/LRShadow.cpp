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


void LRShadow::drawShadow(const DrawArgs &args) {
    // add shader
    nvgBeginPath(args.vg);
    nvgRect(args.vg, -20, -20, box.size.x + 40, box.size.y + 40);

    NVGcolor icol = nvgRGBAf(0.0f, 0.0f, 0.0f, strength);
    NVGcolor ocol = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.f);;

    NVGpaint paint = nvgRadialGradient(args.vg, box.size.x / 2 + shadowPos.x, box.size.y / 2 + shadowPos.y,
                                       box.size.x * 0.3f, box.size.x * size, icol, ocol);
    nvgFillPaint(args.vg, paint);
    nvgFill(args.vg);
}


/**
 * @brief Hook into widget draw routine to simulate shadow
 * @param args.vg
 */
void LRShadow::draw(const DrawArgs &args) {
    drawShadow(args);
}


/**
 * @brief Setter for box dimensions
 * @param box
 */
void LRShadow::setBox(const Rect &box) {
    LRShadow::box = box;
}


/**
 * @brief Setter for outer radius size
 * @param size
 */
void LRShadow::setSize(float size) {
    LRShadow::size = size;
}


/**
 * @brief Setter for draw strength of shadow
 * @param strength
 */
void LRShadow::setStrength(float strength) {
    LRShadow::strength = strength;
}


}

