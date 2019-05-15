#include "LindenbergResearch.hpp"

Plugin *pluginInstance;


void init(Plugin *p) {
    pluginInstance = p;


    p->addModel(modelType35);


/*    p->addModel(modelSimpleFilter);
    p->addModel(modelMS20Filter);
    p->addModel(modelAlmaFilter);
    p->addModel(modelReShaper);
    p->addModel(modelWestcoast);
    p->addModel(modelQuickMix);
    p->addModel(modelDiodeVCF);
    p->addModel(modelTestDriver);
    p->addModel(modelEchoBox);
    p->addModel(modelVCO);



    //  p->addModel(modelSpeck);

    p->addModel(modelBlankPanel);
    p->addModel(modelBlankPanelEmpty);
    p->addModel(modelBlankPanelSmall);
    p->addModel(modelBlankPanelWood);*/
}
