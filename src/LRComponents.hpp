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
#pragma once

#include <map>
#include <cmath>
#include "rack.hpp"
#include "asset.hpp"
#include "LRGestalt.hpp"
#include "String.hpp"
#include "LindenbergResearch.hpp"
#include "dsp/DSPMath.hpp"

#define LCD_FONT_DIG7 "res/digital-7.ttf"
#define LCD_FONTSIZE 11
#define LCD_LETTER_SPACING 0
#define LCD_MARGIN_VERTICAL 1.73
#define LCD_MARGIN_HORIZONTAL 1.07

#define LCD_DEFAULT_COLOR_DARK nvgRGBAf(0.23, 0.6, 0.82, 1.0)
#define LCD_DEFAULT_COLOR_LIGHT nvgRGBAf(0.13, 0.7, 1.0, 1.0)
#define LCD_DEFAULT_COLOR_AGED nvgRGBAf(0.63, 0.1, 0.0, 1.0)

#define LED_DEFAULT_COLOR_DARK nvgRGBAf(0.23, 0.5, 1.0, 1.0)
#define LED_DEFAULT_COLOR_LIGHT nvgRGBAf(1.0, 0.32, 0.12, 1.0)
#define LED_DEFAULT_COLOR_AGED nvgRGBAf(1.0, 1.0, 0.12, 1.0)


using namespace rack;
using std::vector;
using std::shared_ptr;
using std::string;
using std::map;

extern Plugin *pluginInstance;

namespace lrt {

/* Type definitions for common used data structures */
typedef std::shared_ptr<rack::Font> TrueType;
typedef std::string string;


/**
 * @brief Emulation of a LCD monochrome display
 */
struct LRLCDWidget : ParamWidget, LRGestaltVariant, LRGestaltChangeAction {

    enum LCDType {
        NUMERIC,
        TEXT,
        LIST
    };

    TransformWidget *tw;
    SvgWidget *sw;

    TrueType ttfLCDDIG7;


    LCDType type;
    NVGcolor fg;

    bool active = true;
    float value = 0.0;
    unsigned char length = 0;
    string format, text;
    vector<string> items;

    float fontsize;

    string s1;
    string s2;

    /**
     * @brief Constructor
     */
    LRLCDWidget(unsigned char length, string format, LCDType type, float fontsize);

    void draw(const DrawArgs &args) override;
    void onButton(const event::Button &e) override;
    void doResize(Vec v);
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
};


/**
 * @brief Indicator for control voltages on knobs
 */
struct LRCVIndicator : FramebufferWidget, LRGestaltChangeAction {
    static constexpr float OVERFLOW_THRESHOLD = 0.01f;

    /** enabled or not */
    bool active = false;

    /** radius from middle */
    float distance;

    /** normalized control voltage. must between [0..1] */
    float cv = 0.f;

    /** indicator distances */
    float d1 = 4.f;
    float d2 = 0.1f;

    /** draw angle */
    float angle;
    float angle2;

    /** middle of parent */
    Vec middle;

    NVGcolor normal, overflow;

    /**
     * @brief Init indicator
     * @param distance Radius viewed from the middle
     * @param angle Angle of active knob area
     */
    LRCVIndicator(float distance, float angle);


    /**
     * @brief Manipulate the indicator symbol
     * @param d1 Height of the triangle
     * @param d2 Half of the base width
     */
    void setDistances(float d1, float d2);

    /**
     * @brief Draw indicator
     * @param args
     */
    void draw(const DrawArgs &args) override;
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
};


/**
 * @brief Standard LR Shadow
 */
struct LRShadow : TransparentWidget {
private:
    Rect box;
    float size = 0.65;
    float strength = 1.f;

    /** shadow shift */
    Vec shadowPos = Vec(3, 5);
public:


    LRShadow() {
        box.size = math::Vec();
    }


    /**
     * @brief Set the new offset of the shadow gradient
     * @param x
     * @param y
     */
    inline void setShadowPosition(float x, float y) {
        shadowPos = Vec(x, y);
    }


    void setBox(const Rect &box);
    void setSize(float size);
    void setStrength(float strength);


    void drawShadow(const DrawArgs &args);
    void draw(const DrawArgs &args) override;
};


/**
 * @brief The base of all knobs used in LR panels, includes a indicator
 */
struct LRKnob : SvgKnob, LRGestaltVariant, public LRGestaltChangeAction {
private:
    static constexpr float ANGLE = 0.83f;

    /** setup indicator with default values */
    LRCVIndicator *indicator;


    /** use gradient */
    bool lightning = false;
    /** gradient radius */
    float radius;
    /** colors */
    NVGcolor startColor, endColor;


protected:
    /** shader */
    LRShadow *shader;
    /** use oversampling? */
    bool oversampled = false;
public:

    LRKnob();


    /**
     * @brief Set the value of the indicator
     * @param value
     */
    void setIndicatorValue(float value) {
        indicator->cv = value;
        //  fb->dirty = true;
    }


    inline void invalidate() {
        fb->dirty = true;
        if (oversampled) fb->fbSize = Vec(1, 1); //invalidate
    }


    /**
     * @brief Switch indicator on/off
     * @param active
     */
    void setIndicatorActive(bool active) {
        indicator->active = active;
        // fb->dirty = true;
    }


    /**
     * @brief Get indicator state
     * @return
     */
    bool isIndicatorActive() {
        return indicator->active;
    }


    /**
     * @brief Setup distance of indicator from middle
     * @param distance
     */
    void setIndicatorDistance(float distance) {
        indicator->distance = distance;
        fb->dirty = true;
        if (oversampled) fb->fbSize = Vec(1, 1); //invalidate
    }


    /**
     * @brief Setup distance of indicator from middle
     * @param distance
     */
    void setIndicatorShape(float d1, float d2) {
        indicator->setDistances(d1, d2);
        fb->dirty = true;
        if (oversampled) fb->fbSize = Vec(1, 1); //invalidate
    }


    /**
     * @brief Setup gradient parameter is a gradient is used by a knob
     * @param radius The inner radius
     * @param startColor Start color (Top)
     * @param endColor  End color (Bottom)
     */
    void setGradientParameter(float radius, NVGcolor startColor, NVGcolor endColor) {
        lightning = true;
        LRKnob::radius = radius;
        LRKnob::startColor = startColor;
        LRKnob::endColor = endColor;
    }


    /**
     * @brief Hook into setSvg() method to setup box dimensions correct for indicator
     * @param svg
     */
    //  void setSvg(shared_ptr<SVG> svg);

    void setSvg(std::shared_ptr<Svg> svg);


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


    /**
     * @brief Draw knob
     * @param vg
     */
    void draw(const DrawArgs &args) override;
    void step() override;
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
};


/**
 * @brief Quantize position to odd numbers to simulate a toggle switch
 */
struct LRToggleKnob : LRKnob {
    LRToggleKnob(float length = 0.5f) {
        //TODO: parametrize start and end angle
        minAngle = -0.666666f * (float) PI;
        maxAngle = length * (float) PI;

        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/ToggleKnob.svg")));

        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/ToggleKnob.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateToggleKnobLight.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateToggleKnobLight.svg")));

        snap = true;
        speed = 2.f; //workaround
        // smooth = false;
    }


    /*void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }*/
    /* void onChange(const event::Change &e) override {
         auto value = paramQuantity->getValue();
         paramQuantity->setValue(round(value));

         SvgKnob::onChange(e);//TODO: [2019-05-29 09:18] => Implement own version of ParamWidget
     }*/


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        LRKnob::onGestaltChangeAction(e);

        switch (e.current) {
            case LRGestaltType::DARK:
                shader->setShadowPosition(3, 4);
                shader->setStrength(1.2f);
                shader->setSize(0.7f);
                setGradientParameter(20.f, nvgRGBAf(1, 1, 1, 0.01), nvgRGBAf(0, 0, 0, 0.32));
                break;
            case LRGestaltType::LIGHT:
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.6f);
                setGradientParameter(20.f, nvgRGBAf(0.9, 0.9, 0.9, 0.10), nvgRGBAf(0, 0, 0, 0.42));
                break;
            case LRGestaltType::AGED:
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.6f);
                setGradientParameter(20.f, nvgRGBAf(0.9, 0.9, 0.9, 0.10), nvgRGBAf(0, 0, 0, 0.42));
                break;
            default:
                break;
        }
    }
};


/**
 * @brief LR Big Knob
 */
struct LRBigKnob : LRKnob {
    LRBigKnob() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/BigKnob.svg")));

        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/BigKnob.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateBigLight.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateBigLight.svg")));
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        LRKnob::onGestaltChangeAction(e);

        switch (e.current) {
            case LRGestaltType::DARK:
                setIndicatorDistance(15);
                setIndicatorShape(4.8, 0.12);
                shader->setShadowPosition(4, 5);
                shader->setStrength(0.8f);
                shader->setSize(.65f);
                setGradientParameter(28.f, nvgRGBAf(1, 1, 1, 0.02), nvgRGBAf(0, 0, 0, 0.28));
                break;
            case LRGestaltType::LIGHT:
                setIndicatorDistance(17);
                setIndicatorShape(4.1, 0.08);
                shader->setShadowPosition(4, 5);
                shader->setStrength(0.58f);
                shader->setSize(0.6f);
                setGradientParameter(28.f, nvgRGBAf(1.0, 1.0, 1.0, 0.09), nvgRGBAf(0.0, 0, 0, 0.42));
                break;
            case LRGestaltType::AGED:
                setIndicatorDistance(17);
                setIndicatorShape(4.1, 0.08);
                shader->setShadowPosition(4, 5);
                shader->setStrength(0.58f);
                shader->setSize(0.6f);
                setGradientParameter(28.f, nvgRGBAf(1.0, 1.0, 1.0, 0.09), nvgRGBAf(0.0, 0, 0, 0.42));
                break;
            default:
                break;
        }
    }
};


/**
 * @brief LR Middle Knob
 */
struct LRMiddleKnob : LRKnob {
    LRMiddleKnob() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/MiddleKnob.svg")));

        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/MiddleKnob.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateMiddleLight.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateMiddleLight.svg")));
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        LRKnob::onGestaltChangeAction(e);

        switch (e.current) {
            case LRGestaltType::DARK:
                setIndicatorDistance(13);
                setIndicatorShape(5, 0.13);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.8f);
                shader->setSize(.65f);
                setGradientParameter(20.f, nvgRGBAf(1, 1, 1, 0.13), nvgRGBAf(0, 0, 0, 0.28));
                break;
            case LRGestaltType::LIGHT:
                setIndicatorDistance(11);
                setIndicatorShape(4.3, 0.11);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.6f);
                setGradientParameter(20.f, nvgRGBAf(1.0, 1.0, 1.0, 0.09), nvgRGBAf(0.0, 0, 0, 0.42));
                break;
            case LRGestaltType::AGED:
                setIndicatorDistance(11);
                setIndicatorShape(4.3, 0.11);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.6f);
                setGradientParameter(20.f, nvgRGBAf(1.0, 1.0, 1.0, 0.09), nvgRGBAf(0.0, 0, 0, 0.42));
                break;
            default:
                break;
        }
    }
};


/**
 * @brief LR Small Knob
 */
struct LRSmallKnob : LRKnob {
    LRSmallKnob() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/SmallKnob.svg")));

        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/SmallKnob.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallLight.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallLight.svg")));

        oversampled = true;

        speed = 0.9f;
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        LRKnob::onGestaltChangeAction(e);

        switch (e.current) {
            case LRGestaltType::DARK:
                setIndicatorDistance(13);
                setIndicatorShape(5, 0.13);
                shader->setShadowPosition(3, 3);
                shader->setStrength(1.f);
                shader->setSize(.65f);
                setGradientParameter(12.f, nvgRGBAf(1, 1, 1, 0.13), nvgRGBAf(0, 0, 0, 0.3));
                break;
            case LRGestaltType::LIGHT:
                shader->setShadowPosition(3, 3);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.7f);
                setGradientParameter(14.f, nvgRGBAf(1.0, 1.0, 1.0, 0.03), nvgRGBAf(0.0, 0, 0, 0.32));
                break;
            case LRGestaltType::AGED:
                shader->setShadowPosition(3, 3);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.7f);
                setGradientParameter(14.f, nvgRGBAf(1.0, 1.0, 1.0, 0.03), nvgRGBAf(0.0, 0, 0, 0.32));
                break;
            default:
                break;
        }
    }
};


/**
 * @brief LR Small Knob
 */
struct LRSmallToggleKnob : LRKnob {
    LRSmallToggleKnob(float length = 0.83) {
        //TODO: parametrize start and end angle
        minAngle = -length * (float) PI;
        maxAngle = length * (float) PI;

        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallToggle.svg")));

        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallToggle.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallToggleLight.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateSmallToggleLight.svg")));

        speed = 3.0;
    }


    /*void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }*/
    void onChange(const event::Change &e) override {
        auto value = paramQuantity->getValue();
        paramQuantity->setValue(round(value));

        SvgKnob::onChange(e);
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        LRKnob::onGestaltChangeAction(e);

        switch (e.current) {
            case LRGestaltType::DARK:
                setIndicatorDistance(13);
                setIndicatorShape(5, 0.13);
                shader->setShadowPosition(3, 3);
                shader->setStrength(1.f);
                shader->setSize(.65f);
                setGradientParameter(14.f, nvgRGBAf(1, 1, 1, 0.17), nvgRGBAf(0, 0, 0, 0.32));
                break;
            case LRGestaltType::LIGHT:
                shader->setShadowPosition(3, 3);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.3f);
                shader->setSize(0.7f);
                setGradientParameter(14.f, nvgRGBAf(0.9, 0.9, 0.9, 0.19), nvgRGBAf(0, 0, 0, 0.29));
                break;
            case LRGestaltType::AGED:
                shader->setShadowPosition(3, 3);
                shader->setShadowPosition(2, 3);
                shader->setStrength(0.5f);
                shader->setSize(0.7f);
                setGradientParameter(14.f, nvgRGBAf(0.9, 0.9, 0.9, 0.19), nvgRGBAf(0, 0, 0, 0.29));
                break;
            default:
                break;
        }
    }
};


/**
 * @brief LR Big Knob
 */
struct LRAlternateMiddleLight : LRKnob {
    LRAlternateMiddleLight() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/AlternateMiddleLight.svg")));
        setIndicatorDistance(11);
        setIndicatorShape(4.3, 0.11);


        shader->setShadowPosition(2, 3);

        shader->setStrength(0.5f);
        shader->setSize(0.6f);

    }
};


/**
 * @brief Alternative IO Port
 */
struct LRIOPortBLight : SvgPort {
private:
    LRShadow *shader;

public:
    LRIOPortBLight() {
        auto svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortBLight.svg"));

        setSvg(svg);

        shader = new LRShadow();
        //  addChild(shadow);
        if (shadow) shadow->visible = false;

        /** inherit dimensions */
        shader->setBox(box);
        shader->setSize(0.55);
        shader->setStrength(0.3);
        shader->setShadowPosition(1, 2);
    }


    void draw(const Widget::DrawArgs &args) override {
        shader->draw(args);
        SvgPort::draw(args);
    }
};


/**
 * @brief Alternative IO Port
 */
struct LRIOPortD : SvgPort, LRGestaltVariant, LRGestaltChangeAction { //TODO: rename after migration
private:
    LRShadow *shader;

public:
    LRIOPortD() {
        shader = new LRShadow();
        if (shadow) shadow->visible = false;
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        switch (e.current) {
            case LRGestaltType::DARK:
                setSvg(getSVGVariant(DARK));

                shader->setBox(box);
                shader->setSize(0.57);
                shader->setStrength(0.3);
                shader->setShadowPosition(2, 3);
                break;
            case LRGestaltType::LIGHT:
                setSvg(getSVGVariant(LIGHT));

                shader->setBox(box);
                shader->setSize(0.52);
                shader->setStrength(0.5);
                shader->setShadowPosition(1, 2);
                break;
            case LRGestaltType::AGED:
                setSvg(getSVGVariant(AGED));

                shader->setBox(box);
                shader->setSize(0.52);
                shader->setStrength(0.5);
                shader->setShadowPosition(1, 2);
                break;
            default:
                break;
        }

        fb->dirty = true;
    }


    void draw(const Widget::DrawArgs &args) override {
        shader->draw(args);
        SvgPort::draw(args);
    }
};


/**
 * @brief Audio variant of IO port
 */
struct LRIOPortAudio : LRIOPortD {
    LRIOPortAudio() : LRIOPortD() {
        addSVGVariant(DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortB.svg")));
        addSVGVariant(LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortBLight.svg")));
        addSVGVariant(AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortBLight.svg")));
    }
};


/**
 * @brief CV variant of IO port
 */
struct LRIOPortCV : LRIOPortD {
    LRIOPortCV() : LRIOPortD() {
        addSVGVariant(DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortC.svg")));
        addSVGVariant(LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortCLight.svg")));
        addSVGVariant(AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/IOPortCLight.svg")));
    }
};


/**
 * @brief Alternative screw head A
 */
struct ScrewLight : SvgScrew, LRGestaltVariant, LRGestaltChangeAction {
    ScrewLight() {
        sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewLight.svg"));
        sw->wrap();
        box.size = sw->box.size;


        addSVGVariant(LRGestaltType::DARK, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkC.svg")));
        addSVGVariant(LRGestaltType::LIGHT, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkLightC.svg")));
        addSVGVariant(LRGestaltType::AGED, APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkLightC.svg")));
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        sw->svg = getSVGVariant(e.current);
        //dirty = true;
    }

};


/**
 * @brief Alternative screw head A
 */
struct ScrewDarkB : SvgScrew {
    ScrewDarkB() {
        sw->svg = APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/ScrewDarkB.svg"));
        sw->wrap();
        box.size = sw->box.size;
    }
};


/**
 * @brief Custom switch based on original Rack files
 */
struct LRSwitch : SvgSwitch {
    LRSwitch() {
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/Switch0.svg")));
        addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/elements/Switch1.svg")));
    }
};


/**
 * @brief Standard LED
 */
struct LRLight : ModuleLightWidget, LRGestaltChangeAction {
    float glowIntensity = 0.25;

    LRLight();

    void draw(const Widget::DrawArgs &args) override;
    void setColor(NVGcolor color);
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
};


/**
 * @brief Standard linear gradient widget
 */
struct LRGradientWidget : TransparentWidget {
private:
    /* gradient vectors */
    Vec v1, v2;

    /* standard margin */
    float margin = 10.f;

    /* gradient colors */
    NVGcolor innerColor, outerColor;

public:

    LRGradientWidget(const Vec &size) {
        box.pos = Vec(0, 0);
        box.size = size;
    }


    /**
     * @brief Create a new gradient widget
     * @param size box size (derived from parent widget)
     * @param innerColor The inner color (v1)
     * @param outerColor The outher color (v2)
     * @param offset The offset of the left-top corner (optional)
     */
    LRGradientWidget(const Vec &size, const NVGcolor &innerColor, const NVGcolor &outerColor, const Vec &offset = Vec(30, -50)) :
            innerColor(innerColor), outerColor(outerColor) {
        box.size = size;

        box.pos = Vec(0, 0);

        /* initialise with standard dimensions */
        v1 = offset;
        v2 = Vec(v1.x, size.y);
    }


    void setGradientOffset(const Vec &v1, const Vec &v2) {
        LRGradientWidget::v1 = v1;
        LRGradientWidget::v2 = v2;
    }


    const NVGcolor &getInnerColor() const;
    void setInnerColor(const NVGcolor &innerColor);
    const NVGcolor &getOuterColor() const;
    void setOuterColor(const NVGcolor &outerColor);

    void draw(const Widget::DrawArgs &args) override;
};


/**
 * @brief Widget for simulating used look
 */
struct LRPatinaWidget : TransparentWidget {
    SvgWidget *svg;

    float strength = 0.99f;

    LRPatinaWidget(const string &filename, const Vec &size);
    void draw(const Widget::DrawArgs &args) override;
    void randomize();

};


/**
 * @brief Default panel border
 */
struct LRPanelBorder : TransparentWidget, LRGestaltChangeAction {
    static constexpr float VERT_BORDER_WIDTH = 1.3f;
    static constexpr float HORIZ_BORDER_WIDTH = 5.4f;

    NVGcolor borderColorLight;
    NVGcolor borderColorDark;


    inline void draw(const Widget::DrawArgs &args) override {

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, VERT_BORDER_WIDTH, 0 + VERT_BORDER_WIDTH, box.size.y);
        nvgFillColor(args.vg, borderColorLight);
        nvgFill(args.vg);

        NVGpaint paint1 = nvgLinearGradient(args.vg, box.size.x / 2, 0, box.size.x / 2, HORIZ_BORDER_WIDTH, borderColorDark, nvgRGBAf(0, 0, 0, 0));
        NVGpaint paint2 = nvgLinearGradient(args.vg, box.size.x / 2, box.size.y, box.size.x / 2, box.size.y - HORIZ_BORDER_WIDTH, borderColorDark, nvgRGBAf(0, 0, 0, 0));

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, HORIZ_BORDER_WIDTH);
        nvgFillPaint(args.vg, paint1);
        nvgFill(args.vg);

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, box.size.y - HORIZ_BORDER_WIDTH, box.size.x, box.size.y - HORIZ_BORDER_WIDTH);
        nvgFillPaint(args.vg, paint2);
        nvgFill(args.vg);

        /*nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, HORIZ_BORDER_WIDTH);
        nvgFillColor(args.vg, borderColorLight);
        nvgFill(args.vg);

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, box.size.y - HORIZ_BORDER_WIDTH, box.size.x, box.size.y - HORIZ_BORDER_WIDTH);
        nvgFillColor(args.vg, borderColorDark);
        nvgFill(args.vg);*/



        nvgBeginPath(args.vg);
        nvgRect(args.vg, box.size.x - VERT_BORDER_WIDTH, 0, box.size.x - VERT_BORDER_WIDTH, box.size.y);
        nvgFillColor(args.vg, borderColorDark);
        nvgFill(args.vg);
    }


    void onGestaltChangeAction(LRGestaltChangeEvent &e) override {
        switch (e.current) {
            case DARK:
                borderColorLight = nvgRGBAf(0.6, 0.6, 0.6, 0.2);
                borderColorDark = nvgRGBAf(0.0, 0.0, 0.0, 0.4);
                break;
            case LIGHT:
                borderColorLight = nvgRGBAf(0.9, 0.9, 0.9, 0.2);
                borderColorDark = nvgRGBAf(0.0, 0.0, 0.0, 0.1);
                break;
            case AGED:
                borderColorLight = nvgRGBAf(0.9, 0.9, 0.9, 0.2);
                borderColorDark = nvgRGBAf(0.0, 0.0, 0.0, 0.1);
                break;
            case NIL:
                borderColorLight = nvgRGBAf(0.9, 0.9, 0.9, 0.2);
                borderColorDark = nvgRGBAf(0.0, 0.0, 0.0, 0.1);
                break;
            default:
                borderColorLight = nvgRGBAf(0.9, 0.9, 0.9, 0.2);
                borderColorDark = nvgRGBAf(0.0, 0.0, 0.0, 0.1);
                break;
        }
    }

};


struct BitmapWidget : FramebufferWidget {
    string filename;
    int handle;
    float scale, angle;

    BitmapWidget(const string &filename);

    void draw(const DrawArgs &args) override;
    void step() override;

};


struct LRScrew;


/**
 * @brief Standard LR module Panel
 */
struct LRPanel : FramebufferWidget, LRGestaltVariant, LRGestaltChangeAction {
    SvgWidget *panelWidget;
    LRPanelBorder *pb;
    map<LRGestaltType, LRGradientWidget *> gradients;
    vector<LRScrew *> screws;
    LRPatinaWidget *patinaWidgetClassic, *patinaWidgetWhite;


    void setGradientVariant(LRGestaltType gestalt, bool gradient);
    void setPatina(LRGestaltType gestalt, bool enabled);
    void init();
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
};


/**
 * @brief Standard LR Screw
 */
struct LRScrew : FramebufferWidget, LRGestaltVariant, LRGestaltChangeAction {
    TransformWidget *tw;
    SvgWidget *sw;
    float angle;

    LRScrew();
    void onGestaltChangeAction(LRGestaltChangeEvent &e) override;
    void onButton(const event::Button &e) override;
    void rotate(float amount);


    /**
     * @brief Trigger redraw if parent is a LRPanel
     */
    void invalidate() {
        dirty = true;
        fbSize = Vec(1, 1); // invalidate

        if (auto *panel = dynamic_cast<LRPanel *>(parent)) {
            panel->fbSize = Vec(1, 1);
            panel->dirty = true;
        }
    }
};


struct InformationWidget : FramebufferWidget {
    TrueType statsttf;

#ifdef LRT_DEBUG
    InformationWidget();
    void draw(const DrawArgs &args) override;
#endif
};


/**
 * @brief Passive rotating SVG image
 */
struct SVGRotator : FramebufferWidget {
    TransformWidget *tw;
    SvgWidget *sw;

    /** angle to rotate per step */
    float angle = 0;
    float inc;
    float scale;
    float transperency;


    SVGRotator();


    /**
     * @brief Factory method
     * @param pos Position
     * @param svg Pointer to SVG image
     * @param angle Increment angle per step
     * @param scale Scaling of the SVG / default 100%
     * @param transperency Transperancy of the SVG / default 100%
     */
    SVGRotator static *create(Vec pos, shared_ptr<Svg> svg, float inc, float scale = 1.0f, float transperency = 1.f) {
        SVGRotator *rotator = new SVGRotator();

        rotator->box.pos = pos;
        rotator->setSvg(svg);
        rotator->inc = inc;
        rotator->scale = scale;
        rotator->transperency = transperency;

        return rotator;
    }


    void draw(const Widget::DrawArgs &args) override;

    void setSvg(shared_ptr<Svg> svg);
    void step() override;
};


struct FontIconWidget : FramebufferWidget {
    TrueType iconFont;
    float fontSize;
    NVGcolor color;

    explicit FontIconWidget(float fontSize = 12.f, NVGcolor color = nvgRGBAf(1.f, 1.f, 1.f, 1.f));

    void draw(const Widget::DrawArgs &args) override;
};


struct LRLevelLED {
    Vec pos, size;
    bool enabled = false;
    NVGcolor frame, bodyOff, bodyOn;


    LRLevelLED() {}


    virtual ~LRLevelLED() {}


    virtual void drawShape(NVGcontext *vg) {
        // just for debug
        nvgBeginPath(vg);
        nvgRect(vg, getAbsX(), getAbsY(), size.x, size.y);
        nvgFillColor(vg, nvgRGBAf(1, 0.5, 0, 0.3));
        nvgFill(vg);
    };


    float getAbsX() { return pos.x - (size.x / 2); }


    float getAbsY() { return pos.y - (size.y / 2); }


    Vec getAbsolutePos() {
        return Vec(getAbsX(), getAbsY());
    }

};


struct LRRectLevelLED : LRLevelLED {
    LRRectLevelLED();

    virtual ~LRRectLevelLED();
    void drawShape(NVGcontext *vg) override;
};


struct LRRoundRectLevelLED : LRLevelLED {
    LRRoundRectLevelLED();

    void drawShape(NVGcontext *vg) override;
};


template<class T>
struct LRLevelWidget : FramebufferWidget {
    static constexpr int SAMPLE_FRQ = 5;

    Vec ledSize = Vec(10, 5);
    vector<LRLevelLED *> elements;

    float value, oldValue;
    float dist;
    float margin;
    int count, interval;


    LRLevelWidget(Vec pos, Vec size = Vec(10, 5), float dist = 5.f, float margin = 15.f, int count = 10) :
            ledSize(size), dist(dist), margin(margin), count(count) {

        box.pos = pos;
        box.size = Vec(margin * 2 + ledSize.x, margin * 2 + count * ledSize.y + (count - 1) * dist);
        elements.reserve(count);

        value = 0;
        oldValue = 0;

        for (int i = 0; i < count; i++) {
            LRLevelLED *e = new T;
            e->pos = getYLEDPosition(i + 1);
            e->size = ledSize;

            e->frame = nvgRGBAf(0.4, 0.4, 0.4, 0.3);
            e->bodyOff = nvgRGBAf(0, 0.1, 0.2, 0);
            e->bodyOn = nvgRGBAf(0, 0.65, 0.99, 1);

            elements.push_back(e);
        }
    }


    inline Vec getYLEDPosition(int pos) {
        return Vec(margin + ledSize.x / 2, margin + (pos - 1) * dist + ledSize.y * pos - ledSize.y / 2);
    }


    void draw(const DrawArgs &args) override {
        FramebufferWidget::draw(args);

        nvgBeginPath(args.vg);
        //nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
        nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 2.f);
        nvgFillColor(args.vg, nvgRGBAf(0, 0.0, 0.0, 0));
        nvgFill(args.vg);

        for (int i = 0; i < count; i++) {
            elements[count - 1 - i]->enabled = (i + 1) <= value * count;
            elements[count - 1 - i]->drawShape(args.vg);
        }
    }


    void setValue(float val) {
        value += val;
        value /= 2;
    }


    void step() override {
        if (interval++ >= SAMPLE_FRQ) {
            interval = 0;
            dirty = true;
        }
    }
};





//TODO: [2019-05-18 21:34] => implement resize panel
/**
 * Utility widget for resize action on modules
 */
/*
struct ModuleResizeWidget : Widget {

   float minWidth;
   bool right = false;
   float dragX;
   float dragY;
   Rect originalBox;


   ModuleResizeWidget(float _minWidth) {
       box.size = Vec(RACK_GRID_WIDTH * 1, RACK_GRID_HEIGHT);
       minWidth = _minWidth;
   }


   void onMouseDown(EventMouseDown &e) override {
       if (e.button == 0) {
           e.consumed = true;
           e.target = this;
       }
   }


   void onDragStart(EventDragStart &e) override {
       dragX = gRackWidget->lastMousePos.x;
       dragY = gRackWidget->lastMousePos.y;
       ModuleWidget *m = getAncestorOfType<ModuleWidget>();
       originalBox = m->box;
   }


   void onDragMove(EventDragMove &e) override {
       ModuleWidget *m = getAncestorOfType<ModuleWidget>();

       float newDragX = gRackWidget->lastMousePos.x;
       float deltaX = newDragX - dragX;
       float newDragY = gRackWidget->lastMousePos.y;
       float deltaY = newDragY - dragY;

       Rect newBox = originalBox;

       // resize width
       if (right) {
           newBox.size.x += deltaX;
           newBox.size.x = fmaxf(newBox.size.x, minWidth);
           newBox.size.x = roundf(newBox.size.x / RACK_GRID_WIDTH) * RACK_GRID_WIDTH;
       } else {
           newBox.size.x -= deltaX;
           newBox.size.x = fmaxf(newBox.size.x, minWidth);
           newBox.size.x = roundf(newBox.size.x / RACK_GRID_WIDTH) * RACK_GRID_WIDTH;
           newBox.pos.x = originalBox.pos.x + originalBox.size.x - newBox.size.x;
       }

       // resize height
       newBox.size.y += deltaY;
       newBox.size.y = fmaxf(newBox.size.y, RACK_GRID_HEIGHT);
       newBox.size.y = roundf(newBox.size.y / RACK_GRID_HEIGHT) * RACK_GRID_HEIGHT;

       gRackWidget->requestModuleBox(m, newBox);
   }

};*/
}