#include "../LRComponents.hpp"

namespace lrt {

void LRGradientWidget::draw(NVGcontext *vg) {
    FramebufferWidget::draw(vg);

    nvgBeginPath(vg);
    nvgRect(vg, -margin, -margin, box.size.x + margin * 2, box.size.y + margin * 2);
    NVGpaint paint = nvgLinearGradient(vg, v1.x, v1.y, v2.x, v2.y, innerColor, outerColor);
    nvgFillPaint(vg, paint);
    nvgFill(vg);
}

}