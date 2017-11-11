#include "rack.hpp"
#include "asset.hpp"

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

struct Davies1900hBlue : LRBasicKnob {
    Davies1900hBlue() {
        setSVG(SVG::load(assetPlugin(plugin, "res/Davies1900hBlue.svg")));
    }
};