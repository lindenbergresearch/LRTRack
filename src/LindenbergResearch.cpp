#include "LindenbergResearch.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    plugin->slug = "Lindenberg Research";
    // plugin->name = "Lindenberg Research";
    plugin->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<SimpleFilterWidget>("LPFilter24dB", "24dB Lowpass Filter","24dB Lowpass Filter",FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("BlankPanel", "Blank Panel 26TE","Blank Panel 26TE", BLANK_TAG));
    p->addModel(createModel<ReShaperWidget>("ReShaper", "ReShaper Wavefolder","ReShaper Wavefolder",WAVESHAPER_TAG));

}

