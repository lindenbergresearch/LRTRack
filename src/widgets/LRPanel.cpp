#include <window.hpp>
#include "../LRComponents.hpp"
#include "../LRGestalt.hpp"

namespace lrt {

/**
 * @brief Extention for panel background
 * @param vg
 */
void LRPanel::draw(NVGcontext *vg) {
    FramebufferWidget::draw(vg);
}


/**
 * @brief Setup UI interface
 * @param gestalt Pointer to gestalt id
 * @param gradient Pointer to grandient flag
 * @param patina Pointer to patina flag
 */
void LRPanel::setupGestalt(LRGestalt *gestalt, bool *gradient, bool *patina) {
    this->gestalt = gestalt;
    this->gradient = gradient;
    this->patina = patina;
}


/**
 * @brief Constructor
 */
LRPanel::LRPanel() {
    panelWidget = new SVGWidget();
    addChild(panelWidget);
}


/**
 * @brief Initialize a Panel and setup gestalt and UI
 */
void LRPanel::init() {
    auto svg = getSVGVariant(NIL); // INIT

    if (svg != nullptr) {
        panelWidget->setSVG(svg);
    }


    box.size = panelWidget->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);

    /* setup patina widget */
    patinaWidgetWhite = new LRPatinaWidget("res/panels/WhitePatina.svg", box.size);
    patinaWidgetWhite->randomize();
    patinaWidgetWhite->visible = *patina;
    addChild(patinaWidgetWhite);

    patinaWidgetClassic = new LRPatinaWidget("res/panels/AlternatePatina.svg", box.size);
    patinaWidgetClassic->randomize();
    patinaWidgetClassic->strength = .4f;
    patinaWidgetClassic->visible = *patina;
    addChild(patinaWidgetClassic);


    /* setup gradient variants */
    auto gradientDark = new LRGradientWidget(box.size, nvgRGBAf(.4f, .4f, .5f, 0.3f), nvgRGBAf(0.0f, 0.0f, 0.0f, 0.2f), Vec(-10, 10));
    gradientDark->visible = false;
    addChild(gradientDark);
    gradients[LRGestalt::DARK] = gradientDark;

    auto gradientLight = new LRGradientWidget(box.size, nvgRGBAf(0.3, 0.3, 0.f, 0.09f), nvgRGBAf(0.f, 0.f, 0.f, 0.7f), Vec(-10, -10));
    gradientLight->visible = false;
    addChild(gradientLight);
    gradients[LRGestalt::LIGHT] = gradientLight;

    auto gradientAged = new LRGradientWidget(box.size, nvgRGBAf(0.5, 0.5, 0.f, 0.1f), nvgRGBAf(0.f, 0.f, 0.f, 0.73f), Vec(-10, -10));
    gradientAged->visible = false;
    addChild(gradientAged);
    gradients[LRGestalt::AGED] = gradientAged;

    auto *pb = new PanelBorder();
    pb->box.size = box.size;
    addChild(pb);

    dirty = true;
}


/**
 * @brief Set the gradient for the current variant on or off
 * @param invert Automaticaly invert state
 */
void LRPanel::setGradientVariant(bool invert) {
    *gradient = invert == !*gradient;

    gradients[LRGestalt::DARK]->visible = false;
    gradients[LRGestalt::LIGHT]->visible = false;
    gradients[LRGestalt::AGED]->visible = false;

    gradients[*gestalt]->visible = *gradient;

    dirty = true;
}


/**
 * @brief Setup patina on / off
 * @param enabled
 */
void LRPanel::setPatina(bool enabled) {
    *patina = enabled;

    patinaWidgetClassic->visible = *patina && *gestalt == LRGestalt::DARK;

    // TODO: extra patina for aged mode?
    patinaWidgetWhite->visible = *patina && *gestalt != LRGestalt::DARK;


    dirty = true;
}


void LRPanel::onChange(EventChange &e) {
    Widget::onChange(e);

    auto *ec = static_cast<LREventGestaltChange *>(&e);

    if (ec != nullptr) {
        auto svg = getSVGVariant(ec->current);

        if (svg != nullptr) {
            panelWidget->setSVG(svg);
            box.size = panelWidget->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
        }

        setGradientVariant(false);
        setPatina(*patina);

        dirty = true;
    }

    e.consumed = true;
}

}