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
#include <window.hpp>
#include "../LRComponents.hpp"
#include "../LRGestalt.hpp"

namespace lrt {

void LRPanel::init() {
    // improve quality
    oversample = 2.0;

    /* set panel svg */
    panelWidget = new SvgWidget();
    auto svg = getSVGVariant(NIL);  // INIT

    if (svg != nullptr) {
        panelWidget->setSvg(svg);
    }

    box.size = panelWidget->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
    addChild(panelWidget);


    /* setup patina widget */
    patinaWidgetWhite = new LRPatinaWidget("res/panels/WhitePatina.svg", box.size);
    patinaWidgetWhite->randomize();
    patinaWidgetWhite->strength = .9f;
    patinaWidgetWhite->visible = false;
    addChild(patinaWidgetWhite);

    patinaWidgetClassic = new LRPatinaWidget("res/panels/AlternatePatina.svg", box.size);
    patinaWidgetClassic->randomize();
    patinaWidgetClassic->strength = .3;
    patinaWidgetClassic->visible = false;
    addChild(patinaWidgetClassic);


    auto v2 = Vec(box.size.x, box.size.y + 190);

    /* setup gradient variants */
    auto gradientDark = new LRGradientWidget(v2, nvgRGBAf(.6f, .6f, .6f, 0.19f), nvgRGBAf(0.0f, 0.0f, 0.0f, 0.41f), Vec(50, 20));
    gradientDark->visible = false;
    addChild(gradientDark);
    gradients[LRGestaltType::DARK] = gradientDark;

    auto gradientLight = new LRGradientWidget(v2, nvgRGBAf(0.9, 0.9, 0.9f, 0.12f), nvgRGBAf(0.f, 0.f, 0.f, 0.58f), Vec(0, -0));
    gradientLight->visible = false;
    addChild(gradientLight);
    gradients[LRGestaltType::LIGHT] = gradientLight;

    auto gradientAged = new LRGradientWidget(v2, nvgRGBAf(0.4, 0.3, .32f, 0.24f), nvgRGBAf(0.05f, 0.04f, 0.f, 0.93), Vec(0, 0));
    gradientAged->visible = false;
    addChild(gradientAged);
    gradients[LRGestaltType::AGED] = gradientAged;


    screws.resize(4);
    screws[0] = new LRScrew;
    screws[1] = new LRScrew;
    screws[2] = new LRScrew;
    screws[3] = new LRScrew;

    if (box.size.x > 120) {
        screws[0]->box.pos = Vec(15, 1);
        screws[1]->box.pos = Vec(box.size.x - 30, 1);
        screws[2]->box.pos = Vec(15, 366);
        screws[3]->box.pos = Vec(box.size.x - 30, 366);

        /* add screws */
        addChild(screws[0]);
        addChild(screws[1]);
        addChild(screws[2]);
        addChild(screws[3]);
    }

    /* setup panel border */
    pb = new LRPanelBorder();
    pb->box.size = box.size;
    addChild(pb);

    dirty = true;
}


/**
 * @brief Set the gradient for the current variant on or off
 * @param invert Automaticaly invert state
 */
void LRPanel::setGradientVariant(LRGestaltType gestalt, bool gradient) {
    gradients[LRGestaltType::DARK]->visible = false;
    gradients[LRGestaltType::LIGHT]->visible = false;
    gradients[LRGestaltType::AGED]->visible = false;

    gradients[gestalt]->visible = gradient;

    dirty = true;
}


/**
 * @brief Setup patina on / off
 * @param enabled
 */
void LRPanel::setPatina(LRGestaltType gestalt, bool enabled) {
    patinaWidgetClassic->visible = enabled && gestalt == LRGestaltType::DARK;
    patinaWidgetWhite->visible = enabled && gestalt != LRGestaltType::DARK;

    dirty = true;
}


void LRPanel::onGestaltChangeAction(LRGestaltChangeEvent &e) {
    auto svg = getSVGVariant(e.current);

    if (svg != nullptr) {
        panelWidget->setSvg(svg);
        panelWidget->wrap();
        box.size = panelWidget->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
    }

    setGradientVariant(e.current, e.gradient);
    setPatina(e.current, e.patina);

    //nvgluDeleteFramebuffer(fb);
    //fb = nullptr;

    // force framebuffer to redraw to take care of oversampling!
    fbSize = Vec(1, 1);

    dirty = true;
}

}