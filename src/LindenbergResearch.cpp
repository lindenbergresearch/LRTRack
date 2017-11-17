#include "LindenbergResearch.hpp"

// The plugin-wide instance of the Plugin class
Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    plugin->slug = "LRT Rack";
    plugin->name = "LRT Rack";
    plugin->homepageUrl = "https://github.com/lindenbergresearch/LRTRack";

    createModel<SimpleFilterWidget>(plugin, "LPFilter24dB", "24dB Lowpass Filter");
}

