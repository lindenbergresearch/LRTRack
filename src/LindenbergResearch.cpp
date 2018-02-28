#include "LindenbergResearch.hpp"

using namespace rack;


Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    p->slug = TOSTRING(SLUG);
    p->version = TOSTRING(VERSION);

    p->addModel(modelSimpleFilter);
    p->addModel(modelMS20Filter);
    p->addModel(modelAlmaFilter);
    p->addModel(modelReShaper);
    p->addModel(modelVCO);
    p->addModel(modelBlankPanel);
    p->addModel(modelBlankPanelM1);
}


//Plugin *plugin;


/**
 * @brief Init plugin and register widgets
 * @param p Pointer to plugin

void init(Plugin *p) {
    plugin = p;
    p->slug = "Lindenberg Research";
#ifdef VERSION
    p->version = TOSTRING(VERSION);
#endif
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<AlmaFilterWidget>("Lindenberg Research", "VCF", "Alma Ladder Filter", FILTER_TAG));
    p->addModel(createModel<MS20FilterWidget>("Lindenberg Research", "MS20 VCF", "Valerie MS20 Filter", FILTER_TAG));
    p->addModel(createModel<ReShaperWidget>("Lindenberg Research", "ReShaper", "ReShaper Wavefolder", WAVESHAPER_TAG));
    p->addModel(createModel<SimpleFilterWidget>("Lindenberg Research", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("Lindenberg Research", "BlankPanel 01", "Blank Panel 20TE", BLANK_TAG));
    p->addModel(createModel<BlankPanelWidgetM1>("Lindenberg Research", "BlankPanel 02", "Blank Panel 12TE", BLANK_TAG));
    // not ready jet
    p->addModel(createModel<VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG));
}

*/

