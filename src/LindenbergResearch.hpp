#include "rack.hpp"
#include "asset.hpp"

#define LAYOUT_X_RASTER 7.5f
#define LAYOUT_Y_RASTER 7.5f

using namespace rack;

extern Plugin *plugin;

struct SimpleFilterWidget : ModuleWidget {
    SimpleFilterWidget();
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
struct LRBasicKnobWhite : LRBasicKnob {
    LRBasicKnobWhite() {
        setSVG(SVG::load(assetPlugin(plugin, "res/LRBasicKnobWhite.svg")));
    }
};

/**
 * @brief Blue version of the Davies1900h
 */
struct Davies1900hBlueKnob : LRBasicKnob {
    Davies1900hBlueKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Davies1900hBlue.svg")));
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
        sw->setSVG(SVG::load(assetPlugin(plugin, "res/ScrewDarkA.svg")));
        box.size = sw->box.size;
    }
};
