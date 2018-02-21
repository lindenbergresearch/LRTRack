#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"

/* panel dimensions */
#define BLANKPANEL_WIDTH 18.f
#define BLANKPANEL_MARK_I_WIDTH 12.f
#define FILTER_WIDTH 12.f
#define MS20_WIDTH 12.f
#define OSCILLATOR_WIDTH 12.f
#define RESHAPER_WIDTH 8.f
#define SIMPLEFILTER_WIDTH 10.f

using namespace rack;

extern Plugin *plugin;


/**
 * @brief Recover of old filer
 */
struct SimpleFilterWidget : LRModuleWidget {
    SimpleFilterWidget();
};


/**
 * @brief Valerie MS20 filter
 */
struct MS20FilterWidget : LRModuleWidget {
    MS20FilterWidget();
};


/**
 * @brief ALMA filter
 */
struct AlmaFilterWidget : LRModuleWidget {
    AlmaFilterWidget();
};


/**
 * @brief Blank Panel with Logo
 */
struct BlankPanelWidget : LRModuleWidget {
    BlankPanelWidget();
};


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelWidgetM1 : LRModuleWidget {
    BlankPanelWidgetM1();
};


/**
 * @brief Reshaper Panel
 */
struct ReShaperWidget : LRModuleWidget {
    ReShaperWidget();
};


/**
 * @brief Woldemar VCO
 */
struct VCOWidget : LRModuleWidget {
    VCOWidget();
};


/**
 * @brief Quantize position to odd numbers to simulate a toogle switch
 */
struct LRToggleKnob : SVGKnob {
    LRToggleKnob(float length = 0.45) {
        minAngle = -length * (float) M_PI;
        maxAngle = length * (float) M_PI;

        setSVG(SVG::load(assetPlugin(plugin, "res/MiddleKnob.svg")));
    }


    void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }
};


/**
 * @brief Basic middle-sized knob
 */
struct LRBigKnob : LRKnob {
    LRBigKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/BigKnob.svg")));
    }
};


/**
 * @brief Basic middle-sized knob
 */
struct LRMiddleKnob : LRKnob {
    LRMiddleKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/MiddleKnob.svg")));
    }
};


/**
 * @brief Blue version of the Davies1900h
 */
struct LRSmallKnob : LRKnob {
    LRSmallKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/SmallKnob.svg")));
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
        sw->svg = SVG::load(assetPlugin(plugin, "res/ScrewDark.svg"));
        sw->wrap();
        box.size = sw->box.size;
    }
};


/**
 * @brief Custom switch based on original Rack files
 */
struct LRSwitch : SVGSwitch, ToggleSwitch {
    LRSwitch() {
        addFrame(SVG::load(assetPlugin(plugin, "res/Switch0.svg")));
        addFrame(SVG::load(assetPlugin(plugin, "res/Switch1.svg")));
    }
};


struct LRRedLight : SmallLight<ModuleLightWidget> {

    LRRedLight();

    void draw(NVGcontext *vg) override;

};
