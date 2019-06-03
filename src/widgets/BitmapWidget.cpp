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


BitmapWidget::BitmapWidget(const string &filename) : filename(filename) {
    box.size = Vec(512, 512);
    box.pos = Vec(0, 0);

    scale = 0.2f;
    angle = 1.f;


}


void BitmapWidget::draw(const Widget::DrawArgs &args) {
    FramebufferWidget::draw(args);

    // create if
    if (handle == 0) {
        handle = nvgCreateImage(args.vg, filename.c_str(), 0);
        DEBUG("loaded: %s => %d", filename.c_str(), handle);

        int x, y;
        nvgImageSize(args.vg, handle, &x, &y);

        box.size = Vec(x * 0.1, y * 0.1);
    }

    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 3.0);
    nvgFillColor(args.vg, nvgRGBAf(0.3, 0.4, 0.5, 0.01));
    //nvgFillColor(vg, rgba(0,0,0,128, color));
    nvgFill(args.vg);

    auto img = nvgImagePattern(args.vg, 0, 0, box.size.x, box.size.y, 0, handle, 1.0);

    nvgFillPaint(args.vg, img);
    nvgFill(args.vg);
}


void BitmapWidget::step() {
    FramebufferWidget::step();
}


