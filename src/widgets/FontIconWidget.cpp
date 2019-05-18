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

#define ICON_FONT_GLYPHYX "res/GlyphyxOneNF.ttf"

using namespace rack;
using namespace lrt;


FontIconWidget::FontIconWidget(float fontSize, NVGcolor color) {
    iconFont = APP->window->loadFont(asset::plugin(pluginInstance, ICON_FONT_GLYPHYX));
    FontIconWidget::fontSize = fontSize;
    FontIconWidget::color = color;
}


void FontIconWidget::draw(const Widget::DrawArgs &args) {
    FramebufferWidget::draw(args);

    nvgFontSize(args.vg, fontSize);
    nvgFontFaceId(args.vg, iconFont->handle);
    nvgTextLetterSpacing(args.vg, 0);


    nvgFillColor(args.vg, color);
    nvgTextBox(args.vg, parent->box.size.x / 2 - fontSize / 2, parent->box.size.y / 2 - fontSize / 2, 0, "H", nullptr);
    //  nvgTextBox(args.vg, 0, 0, 0, "Ldfgjsndfklgjhbsdfkljhgsdkfjgh", nullptr);
}
