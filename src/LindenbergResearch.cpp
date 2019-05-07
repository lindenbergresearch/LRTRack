#include "LindenbergResearch.hpp"

Plugin *plugin;


void init(Plugin *p) {
    plugin = p;
    p->slug = TOSTRING(SLUG);
    p->version = TOSTRING(VERSION);

    p->addModel(modelSimpleFilter);
    p->addModel(modelMS20Filter);
    p->addModel(modelAlmaFilter);
    p->addModel(modelReShaper);
    p->addModel(modelWestcoast);
    p->addModel(modelQuickMix);
    p->addModel(modelDiodeVCF);
    p->addModel(modelType35);
    p->addModel(modelTestDriver);
    p->addModel(modelVCO);

    //  p->addModel(modelSpeck);

    p->addModel(modelBlankPanel);
    p->addModel(modelBlankPanelEmpty);
    p->addModel(modelBlankPanelSmall);
    p->addModel(modelBlankPanelWood);
}
