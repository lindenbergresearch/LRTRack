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
#include "../LRModel.hpp"

namespace lrt {

/**
 * @brief Constructor of LCD Widget
 */
LRLCDWidget::LRLCDWidget(unsigned char length, std::string format, LCDType type, float fontsize) {
    tw = new TransformWidget();
    addChild(tw);

    sw = new SvgWidget();
    tw->addChild(sw);

    /** load LCD ttf font */
    ttfLCDDIG7 = APP->window->loadFont(asset::plugin(pluginInstance, LCD_FONT_DIG7));
    LRLCDWidget::fontsize = fontsize;
    LRLCDWidget::type = type;
    LRLCDWidget::length = length;
    LRLCDWidget::format = format;
    LRLCDWidget::fg = LCD_DEFAULT_COLOR_DARK;

    addSVGVariant(LRGestalt::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/LCDFrameDark.svg")));
    addSVGVariant(LRGestalt::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/LCDFrameLight.svg")));
    addSVGVariant(LRGestalt::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/LCDFrameAged.svg")));

    for (int i = 0; i < LRLCDWidget::length; ++i) {
        s1.append("O");
        s2.append("X");
    }
}


/**
 * @brief Draw method of custom LCD widget
 * @param args.vg
 */
void LRLCDWidget::draw(const Widget::DrawArgs &args) {
    FramebufferWidget::draw(args);

    nvgFontSize(args.vg, fontsize);
    nvgFontFaceId(args.vg, ttfLCDDIG7->handle);
    nvgTextLetterSpacing(args.vg, LCD_LETTER_SPACING);

    float bounds[4];
    nvgTextBoxBounds(args.vg, 0, 0, 120, s2.c_str(), nullptr, bounds);

    auto mx = (bounds[2] - bounds[0]) * LCD_MARGIN_HORIZONTAL;
    auto my = (bounds[3] - bounds[1]) * LCD_MARGIN_VERTICAL;

    // size of frame not proper setup
    if (!sw->box.size.isEqual(Vec(mx, my))) {
        doResize(Vec(mx, my));
    }

    /**
     * @brief Remark: Due to inconsistent baseline shift on changing the
     * font size this is set rather manual and may do not work with other
     * fonts or sizes.
     *
     */
    float xoffs = (mx - bounds[2] + 0.6f - bounds[0]) / 2.f;
    float yoffs = (my - bounds[3] - 1.f - bounds[1]) / 2.f;


    /*if (++foo % 100 == 0)
        debug("bounds: (%f %f %f %f) box: %f x %f font: %f offs: (%f : %f)", bounds[0], bounds[1], bounds[2], bounds[3], box.size.x, box
                .size.y,
              fontsize, xoffs,
              yoffs);*/

    /*nvgFillPaint(args.vg, nvgBoxGradient(args.vg, 0 - 50, 0 - 50, mx + 50, my + 50, 10, 10,
                                    nvgRGBAf(1, 0, 0, 0.25),
                                    nvgRGBAf(0, 0, 0, .0)));
    nvgFill(args.vg);*/

    nvgFillColor(args.vg, nvgRGBAf(fg.r, fg.g, fg.b, 0.23));
    std::string str;

    nvgTextBox(args.vg, xoffs, yoffs, 120, s1.c_str(), nullptr);
    nvgTextBox(args.vg, xoffs, yoffs, 120, s2.c_str(), nullptr);

    /** if set to inactive just draw the background segments */
    if (!active) return;


    // if set to numeric, do some formatting
    if (type == NUMERIC) {
        str = stringf(format.c_str(), value);

        // quick and dirty, urgs
        if (value < 10)
            text = "0" + text;
    }

    // on text mode just format
    if (type == TEXT) {
        str = stringf(format.c_str(), text.c_str());
    }

    // on list mode get current item out of the current value
    if (type == LIST) {
        unsigned long index;
        long current = lround(value);

        if (current < 0) {
            index = 0;
        } else if ((unsigned long) current >= items.size()) {
            index = items.size() - 1;
        } else {
            index = (unsigned long) current;
        }

        value = index;
        text = items[index];
        str = stringf(format.c_str(), items[index].c_str());
    }

    nvgFillColor(args.vg, fg);
    nvgTextBox(args.vg, xoffs, yoffs, 120, str.c_str(), nullptr);
}


void LRLCDWidget::onGestaltChange(LREventGestaltChange &e) {
    auto svg = getSVGVariant(*gestalt);

    if (svg != nullptr) {
        tw->identity();

        sw->setSvg(svg);
        sw->wrap();

        dirty = true;
    }

    LRGestaltChangeAction::onGestaltChange(e);

    switch (*gestalt) {
        case DARK:
            fg = LCD_DEFAULT_COLOR_DARK;
            break;
        case LIGHT:
            fg = LCD_DEFAULT_COLOR_LIGHT;
            break;
        case AGED:
            fg = LCD_DEFAULT_COLOR_AGED;
            break;
        default:
            fg = LCD_DEFAULT_COLOR_DARK;
    }

}


void LRLCDWidget::doResize(Vec v) {
    auto factor = Vec(v.x / sw->box.size.x, v.y / sw->box.size.y);

    // tw->identity();
    tw->scale(factor);

    sw->box.size = v;
    tw->box.size = sw->box.size;
    box.size = sw->box.size;

    dirty = true;
}


void LRLCDWidget::onButton(const event::Button &e) {
    Widget::onButton(e);


    //TODO: check correct mouse button

    if (type == LIST) {
        if (value < items.size() - 1) value++;
        else value = 0;
    }
}

}