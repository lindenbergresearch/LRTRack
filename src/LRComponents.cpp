#include "LRComponents.hpp"


/**
 * @brief Constructor of LCD Widget
 */
LCDWidget::LCDWidget(NVGcolor fg, unsigned char length) {
    /** load LCD ttf font */
    gLCDFont_DIG7 = Font::load(assetPlugin(plugin, LCD_FONT_DIG7));

    auto r = (unsigned char) (fg.r * 0xFF);
    auto g = (unsigned char) (fg.g * 0xFF);
    auto b = (unsigned char) (fg.b * 0xFF);

    LCDWidget::length = length;

    LCDWidget::fg = fg;
    LCDWidget::bg = nvgRGBA(r - 0x40, g - 0x40, b - 0x40, 0x40);
}


/**
 * @brief Creates a new instance of a LRKnob child
 * @tparam TParamWidget Subclass of LRKnob
 * @param pos Position
 * @param module Module pointer
 * @param paramId Parameter ID
 * @param minValue Min
 * @param maxValue Max
 * @param defaultValue Default
 * @return Pointer to new subclass of LRKnob
 */
template<class TParamWidget>
static TParamWidget *LRKnob::create(Vec pos, Module *module, int paramId, float minValue, float maxValue, float defaultValue) {
    auto *param = new TParamWidget();
    param->box.pos = pos;
    param->module = module;
    param->paramId = paramId;
    param->setLimits(minValue, maxValue);
    param->setDefaultValue(defaultValue);
    return param;
}