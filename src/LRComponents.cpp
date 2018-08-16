#include "LRComponents.hpp"

using namespace rack;
using namespace lrt;


/**
 * @brief Extention for panel background
 * @param vg
 */
void LRPanel::draw(NVGcontext *vg) {
    FramebufferWidget::draw(vg);

    nvgBeginPath(vg);
    nvgRect(vg, -MARGIN, -MARGIN, box.size.x + MARGIN * 2, box.size.y + MARGIN * 2);

    NVGpaint paint = nvgLinearGradient(vg, offset.x, offset.y, box.size.x, box.size.y, inner, outer);
    nvgFillPaint(vg, paint);
    nvgFill(vg);
}


void LRPanel::setInner(const NVGcolor &inner) {
    LRPanel::inner = inner;
}


void LRPanel::setOuter(const NVGcolor &outer) {
    LRPanel::outer = outer;
}


LRPanel::LRPanel() {}


const NVGcolor &LRPanel::getInner() const {
    return inner;
}


const NVGcolor &LRPanel::getOuter() const {
    return outer;
}


SVGRotator::SVGRotator() : FramebufferWidget() {
    tw = new TransformWidget();
    addChild(tw);

    sw = new SVGWidget();
    tw->addChild(sw);
}


/**
 * @brief Set SVG image to rotator
 * @param svg
 */
void SVGRotator::setSVG(std::shared_ptr<SVG> svg) {
    sw->setSVG(svg);
    tw->box.size = sw->box.size;
    box.size = sw->box.size;
}


/**
 * @brief Rotate one step
 */
void SVGRotator::step() {
    tw->identity();

    angle = fmodf(angle + inc, 2 * M_PI);;

    Vec center = sw->box.getCenter();
    tw->translate(center);
    tw->rotate(angle);
    tw->translate(center.neg());

    dirty = true;

    FramebufferWidget::step();
}


/**
 * @brief
 * @param module
 */
LRModuleWidget::LRModuleWidget(Module *module) : ModuleWidget(module) {
}