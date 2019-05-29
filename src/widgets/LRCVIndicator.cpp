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
 * @brief Init indicator
 * @param distance Radius viewed from the middle
 * @param angle Angle of active knob area
 */
LRCVIndicator::LRCVIndicator(float distance, float angle) {
    LRCVIndicator::distance = distance;
    LRCVIndicator::angle = angle;

    /** for optimization */
    angle2 = 2 * angle;
}


/**
 * @brief Draw routine for cv indicator
 * @param args.vg
 */
void LRCVIndicator::draw(const Widget::DrawArgs &args) {
    NVGcolor current = normal;

    if (active) {
        /** underrun */
        if (cv < 0.f - OVERFLOW_THRESHOLD) {
            cv = 0.f - OVERFLOW_THRESHOLD;
            current = overflow;
        }

        /** overrun */
        if (cv > 1.f + OVERFLOW_THRESHOLD) {
            cv = 1.f + OVERFLOW_THRESHOLD;
            current = overflow;
        }


        float a = -angle + cv * angle2;
        float d = distance - d1;
        Vec p1, p2, p3;

        /** compute correct point of indicator on circle */
        p1.x = middle.x - sin(-a * (float) M_PI) * distance;
        p1.y = middle.y - cos(-a * (float) M_PI) * distance;

        p2.x = middle.x - sin(-(a + d2) * (float) M_PI) * d;
        p2.y = middle.y - cos(-(a + d2) * (float) M_PI) * d;

        p3.x = middle.x - sin(-(a - d2) * (float) M_PI) * d;
        p3.y = middle.y - cos(-(a - d2) * (float) M_PI) * d;

        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, p1.x, p1.y);
        nvgLineTo(args.vg, p2.x, p2.y);
        nvgLineTo(args.vg, p3.x, p3.y);
        nvgLineTo(args.vg, p1.x, p1.y);
        nvgClosePath(args.vg);

        nvgFillColor(args.vg, current);
        nvgFill(args.vg);
    }
}


void LRCVIndicator::setDistances(float d1, float d2) {
    LRCVIndicator::d1 = d1;
    LRCVIndicator::d2 = d2;
}


void LRCVIndicator::onGestaltChangeAction(LRGestaltChangeEvent &e) {
    switch (e.current) {
        case NIL:
        case DARK:
            normal = nvgRGBAf(0, 0, 0, 0.9);
            overflow = nvgRGBAf(0.9, 0, 0, 0.9);
            break;
        case LIGHT:
        case AGED:
            normal = nvgRGBAf(0.9, 0.9, 0.9, 0.9);
            overflow = nvgRGBAf(0.9, 0.1, 0, 0.9);
            break;
        default:
            normal = nvgRGBAf(0, 0, 0, 0.9);
            overflow = nvgRGBAf(0.9, 0, 0, 0.9);
            break;
    }
}


}