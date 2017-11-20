#include "LindenbergResearch.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "Lindenberg Research";
    //plugin->name = "Lindenberg Research";
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<SimpleFilterWidget>("LindenbergResearch", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("LindenbergResearch", "BlankPanel", "Blank Panel 20TE", UTILITY_TAG));
    p->addModel(createModel<ReShaperWidget>("LindenbergResearch", "ReShaper", "ReShaper Wavefolder", FILTER_TAG));
}

