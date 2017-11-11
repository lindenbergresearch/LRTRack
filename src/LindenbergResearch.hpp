#include "rack.hpp"
#include "asset.hpp"

#define RACK_1U RACK_GRID_WIDTH
#define LAYOUT_X_RASTER 15
#define LAYOUT_Y_RASTER 15

using namespace rack;

extern Plugin *plugin;

struct SimpleFilterWidget : ModuleWidget {
    SimpleFilterWidget();
};

struct LRBasicKnob : SVGKnob {
    LRBasicKnob() {
        minAngle = -0.83f * (float) M_PI;
        maxAngle = 0.83f * (float) M_PI;
    }
};

struct LRBasicKnobWhite : LRBasicKnob {
    LRBasicKnobWhite() {
        setSVG(SVG::load(assetPlugin(plugin, "res/LRBasicKnobWhite.svg")));
    }
};

struct Davies1900hBlueKnob : LRBasicKnob {
    Davies1900hBlueKnob() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Davies1900hBlue.svg")));
    }
};

struct LRIOPort : SVGPort {
    LRIOPort() {
        background->svg = SVG::load(assetPlugin(plugin, "res/IOPort.svg"));
        background->wrap();
        box.size = background->box.size;
    }
};