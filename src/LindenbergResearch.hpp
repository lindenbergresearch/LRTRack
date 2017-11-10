#include "rack.hpp"
#include "asset.hpp"

using namespace rack;

extern Plugin *plugin;

struct SimpleFilterWidget : ModuleWidget {
	SimpleFilterWidget();
};

struct LRBasicKnob : SVGKnob {
	LRBasicKnob() {
		minAngle = -0.83f*M_PI;
		maxAngle = 0.83f*M_PI;
	}
};

struct LRBasicKnobWhite : LRBasicKnob {
	LRBasicKnobWhite() {
		setSVG(SVG::load(assetPlugin(plugin, "res/LRBasicKnobWhite.svg")));
	}
};