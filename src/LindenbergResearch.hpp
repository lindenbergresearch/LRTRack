#include "rack.hpp"
#include "asset.hpp"

#define PI 3.14159265359
#define TWOPI 6.28318530718

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
