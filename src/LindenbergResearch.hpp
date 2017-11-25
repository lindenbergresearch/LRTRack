#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"

/* true type font for LCD display emulation */
#define LCD_SEGMENT_FONT "res/digital7.ttf"
#define LCD_COLOR_FG nvgRGBA(0x00, 0x2F, 0xAF, 0xFF)


using namespace rack;

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


/**
 * @brief Emulation of an LCD monochrome display
 */
struct LCDWidget : Label {
    std::shared_ptr<Font> gLCDFont;
    NVGcolor fg;
    NVGcolor bg;


    /**
     * @brief Constructor
     */
    LCDWidget(NVGcolor fg) {
        /** load LCD ttf font */
        gLCDFont = Font::load(assetPlugin(plugin, LCD_SEGMENT_FONT));

        unsigned char r = (unsigned char) (fg.r * 255);
        unsigned char g = (unsigned char) (fg.g * 255);
        unsigned char b = (unsigned char) (fg.b * 255);


        LCDWidget::fg = fg;
        LCDWidget::bg = nvgRGBA(r - 0x20, g - 0x20, b - 0x20, 0x20);
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
