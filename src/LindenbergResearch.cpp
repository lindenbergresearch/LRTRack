#include "LindenbergResearch.hpp"

Plugin *plugin;

void init(rack::Plugin *p) {
    plugin = p;
    p->slug = TOSTRING(SLUG);
    p->version = TOSTRING(VERSION);

    p->addModel(modelSimpleFilter);
    p->addModel(modelMS20Filter);
    p->addModel(modelAlmaFilter);
    p->addModel(modelReShaper);
    p->addModel(modelWestcoast);
    p->addModel(modelQuickMix);

    p->addModel(modelVCO);

    p->addModel(modelBlankPanel);
    p->addModel(modelBlankPanelM1);
    p->addModel(modelBlankPanelSmall);
}
