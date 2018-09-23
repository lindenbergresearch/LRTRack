#include <window.hpp>
#include "../LRComponents.hpp"

namespace lrt {

/**
 * @brief Extention for panel background
 * @param vg
 */
void LRPanel::draw(NVGcontext *vg) {
    FramebufferWidget::draw(vg);
}


/**
 * @brief Constructor
 */
LRPanel::LRPanel() {
    panel = new SVGWidget();
    addChild(panel);
}


/**
 * @brief Initialize a Panel and setup gestalt SVG
 */
void LRPanel::init() {
    auto svg = getSVGVariant(dark);

    if (svg != nullptr) {
        panel->setSVG(svg);
        debug("loaded");
    }

    box.size = panel->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
    debug("box: %f %f x %f", panel->box.size.x, box.size.x, box.size.y);

    auto *pb = new PanelBorder();
    pb->box.size = box.size;
    addChild(pb);
}


void LRPanel::step() {
    if (isNear(gPixelRatio, 1.0)) {
        // Small details draw poorly at low DPI, so oversample when drawing to the framebuffer
        oversample = 2.0;
    }

    if (invalidGestalt()) {
        debug("gestalt about to change: %i", *this->gestalt);

        auto svg = getSVGVariant(*gestalt);

        if (svg != nullptr) {
            panel->setSVG(svg);
            box.size = panel->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);
        }

        dirty = true;
        syncGestalt();
    }

    FramebufferWidget::step();
}


}