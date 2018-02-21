#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"

#define LCD_FONT_DIG7 "res/digital-7.ttf"
#define LCD_COLOR_FG nvgRGBA(0x10, 0xE1, 0xE4, 0xFF)
#define LCD_FONTSIZE 8
#define LCD_LETTER_SPACING 0

using namespace rack;


/**
 * @brief Standard LRT module
 */
struct LRModule : Module {
    long cnt = 0;


    /**
     * @brief Overtake default constructor for module to be compatible
     * @param numParams
     * @param numInputs
     * @param numOutputs
     * @param numLights
     */
    LRModule(int numParams, int numInputs, int numOutputs, int numLights = 0) :
            Module(numParams, numInputs, numOutputs, numLights) {}


    void step() override {
        Module::step();

        // increment counter
        cnt++;
    }
};


/**
 * @brief Standard LRT ModuleWidget definition
 */
struct LRModuleWidget : ModuleWidget {
};


/**
 * @brief Emulation of a LCD monochrome display
 */
struct LCDWidget : Label {
    std::shared_ptr<Font> gLCDFont_DIG7;
    NVGcolor fg;
    NVGcolor bg;
    unsigned char length = 0;


    /**
     * @brief Constructor
     */
    LCDWidget(NVGcolor fg, unsigned char length);


    /**
     * @brief Draw LCD display
     * @param vg
     */
    void draw(NVGcontext *vg) override;
};


/**
 * @brief Basic knob definition
 */
struct LRKnob : SVGKnob {

private:
    /** control voltage, used for drawing the indicator */
    float cv = 0;

    /** flag if cv indicator should be drawn */
    bool visibleCV = false;

public:
    /**
     * @brief Default constructor
     */
    LRKnob() {
        minAngle = -0.83f * (float) M_PI;
        maxAngle = 0.83f * (float) M_PI;
    }


    float getCV() const {
        return cv;
    }


    void setCV(float cv) {
        LRKnob::cv = cv;
    }


    bool isVisibleCV() const {
        return visibleCV;
    }


    void setVisibleCV(bool visibleCV) {
        LRKnob::visibleCV = visibleCV;
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
    static TParamWidget *create(Vec pos, Module *module, int paramId, float minValue, float maxValue, float defaultValue) {
        auto *param = new TParamWidget();
        param->box.pos = pos;
        param->module = module;
        param->paramId = paramId;
        param->setLimits(minValue, maxValue);
        param->setDefaultValue(defaultValue);
        return param;
    }


    void draw(NVGcontext *vg) override {
        FramebufferWidget::draw(vg);


    }
};


