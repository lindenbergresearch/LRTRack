#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"

using namespace rack;

#define LCD_FONT_DIG7 "res/digital-7.ttf"
#define LCD_COLOR_FG nvgRGBA(0x20, 0xA1, 0xE4, 0xFF)
#define LCD_FONTSIZE 8
#define LCD_LETTER_SPACING 0

/* panel dimensions */
#define BLANKPANEL_WIDTH 18.f
#define BLANKPANEL_MARK_I_WIDTH 12.f
#define FILTER_WIDTH 12.f
#define OSCILLATOR_WIDTH 12.f
#define RESHAPER_WIDTH 8.f

extern Plugin *plugin;


/**
 * @brief ALMA filter
 */
struct SimpleFilterWidget : ModuleWidget {
    SimpleFilterWidget();
};


/**
 * @brief Blank Panel with Logo
 */
struct BlankPanelWidget : ModuleWidget {
    BlankPanelWidget();
};


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelWidgetM1 : ModuleWidget {
    BlankPanelWidgetM1();
};


/**
 * @brief Reshaper Panel
 */
struct ReShaperWidget : ModuleWidget {
    ReShaperWidget();
};


/**
 * @brief Woldemar VCO
 */
struct VCOWidget : ModuleWidget {
    VCOWidget();
};


/**
 * @brief
 */
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


    void step() override {
        Module::step();

        // increment counter
        cnt++;
    }
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
    LCDWidget(NVGcolor fg, unsigned char length) {
        /** load LCD ttf font */
        gLCDFont_DIG7 = Font::load(assetPlugin(plugin, LCD_FONT_DIG7));

        auto r = (unsigned char) (fg.r * 255);
        auto g = (unsigned char) (fg.g * 255);
        auto b = (unsigned char) (fg.b * 255);

        LCDWidget::length = length;

        LCDWidget::fg = fg;
        LCDWidget::bg = nvgRGBA(r - 0x30, g - 0x30, b - 0x30, 0x24);
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
 * @brief Quantize position to odd numbers to simulate a toogle switch
 */
struct LRToggleKnob : SVGKnob {
    LRToggleKnob(float length = 0.45) {
        minAngle = -length * (float) M_PI;
        maxAngle = length * (float) M_PI;

        setSVG(SVG::load(assetPlugin(plugin, "res/BigKnob.svg")));
    }


    void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }
};


/**
 * @brief Basic middle-sized knob
 */
struct LRBigKnob : LRBasicKnob {
    LRBigKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/BigKnob.svg")));
    }
};


/**
 * @brief Basic middle-sized knob
 */
struct LRMiddleKnob : LRBasicKnob {
    LRMiddleKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/MiddleKnob.svg")));
    }
};


/**
 * @brief Blue version of the Davies1900h
 */
struct LRSmallKnob : LRBasicKnob {
    LRSmallKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/SmallKnob.svg")));
    }
};


/**
 * @brief Alternative IO Port
 */
struct IOPort : SVGPort {
    IOPort() {
        background->svg = SVG::load(assetPlugin(plugin, "res/IOPort.svg"));
        background->wrap();
        box.size = background->box.size;
    }
};


/**
 * @brief Alternative screw head A
 */
struct ScrewDarkA : SVGScrew {
    ScrewDarkA() {
        sw->svg = SVG::load(assetPlugin(plugin, "res/ScrewDark.svg"));
        sw->wrap();
        box.size = sw->box.size;
    }
};


struct LRRedLight : SmallLight<ModuleLightWidget> {

    LRRedLight();

    void draw(NVGcontext *vg) override;

};
