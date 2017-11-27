#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"

using namespace rack;

#define LCD_FONT_DIG7 "res/digital-7.ttf"
#define LCD_FONT_MAX "res/Maxter Board St.ttf"
#define LCD_FONT_CALC "res/pocket_calcuatlor_tt.ttf"

/* LCD default color */
#define LCD_COLOR_FG nvgRGBA(0xFA, 0x10, 0x04, 0xFF)

extern Plugin *plugin;


struct SimpleFilterWidget : ModuleWidget {
    SimpleFilterWidget();
};


struct BlankPanelWidget : ModuleWidget {
    BlankPanelWidget();
};


struct ReShaperWidget : ModuleWidget {
    ReShaperWidget();
};


struct VCOWidget : ModuleWidget {
    VCOWidget();
};


struct LRTModule : Module {
    long cnt = 0;


    /**
     * @brief Overtake default constructor for module to be compatible
     * @param numParams
     * @param numInputs
     * @param numOutputs
     * @param numLights
     */
    LRTModule(int numParams, int numInputs, int numOutputs, int numLights = 0) :
            Module(numParams, numInputs, numOutputs, numLights) {}


    void step() override;
};


/**
 * @brief Emulation of an LCD monochrome display
 */
struct LCDWidget : Label {
    std::shared_ptr<Font> gLCDFont_DIG7, gLCDFont_MAX, gLCDFont_CALC;
    NVGcolor fg;
    NVGcolor bg;
    unsigned char length = 0;


    /**
     * @brief Constructor
     */
    LCDWidget(NVGcolor fg, unsigned char length) {
        /** load LCD ttf font */
        gLCDFont_DIG7 = Font::load(assetPlugin(plugin, LCD_FONT_DIG7));
        gLCDFont_MAX = Font::load(assetPlugin(plugin, LCD_FONT_MAX));
        gLCDFont_CALC = Font::load(assetPlugin(plugin, LCD_FONT_CALC));

        unsigned char r = (unsigned char) (fg.r * 255);
        unsigned char g = (unsigned char) (fg.g * 255);
        unsigned char b = (unsigned char) (fg.b * 255);

        LCDWidget::length = length;

        LCDWidget::fg = fg;
        LCDWidget::bg = nvgRGBA(r - 0x20, g - 0x20, b - 0x20, 0x14);
    }


    /**
     * @brief Draw LCD display
     * @param vg
     */
    void draw(NVGcontext *vg) override;
};


/**
 * @brief Basic knob definition
 */
struct LRBasicKnob : SVGKnob {
    LRBasicKnob() {
        minAngle = -0.83f * (float) M_PI;
        maxAngle = 0.83f * (float) M_PI;
    }
};


/**
 * @brief Basic middle-sized knob
 */
struct LRBigKnobWhite : LRBasicKnob {
    LRBigKnobWhite() {
        setSVG(SVG::load(assetPlugin(plugin, "res/BigKnob.svg")));
    }
};


/**
 * @brief Blue version of the Davies1900h
 */
struct LRBasicKnobWhite : LRBasicKnob {
    LRBasicKnobWhite() {
        setSVG(SVG::load(assetPlugin(plugin, "res/LRBasicKnobWhite.svg")));
    }
};


/**
 * @brief Alternative IO Port
 */
struct IOPort : SVGPort {
    IOPort() {
        background->svg = SVG::load(assetPlugin(plugin, "res/IOPortB.svg"));
        background->wrap();
        box.size = background->box.size;
    }
};


/**
 * @brief Alternative screw head A
 */
struct ScrewDarkA : SVGScrew {
    ScrewDarkA() {
        sw->svg = SVG::load(assetPlugin(plugin, "res/ScrewDarkA.svg"));
        sw->wrap();
        box.size = sw->box.size;
    }
};
