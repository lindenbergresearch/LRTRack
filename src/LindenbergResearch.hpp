#pragma once

#include <cmath>
#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"

using namespace rack;
using namespace lrt;

// Forward-declare the Plugin, defined in Template.cpp
extern Plugin *plugin;

extern Model *modelSimpleFilter;
extern Model *modelMS20Filter;
extern Model *modelAlmaFilter;
extern Model *modelReShaper;
extern Model *modelVCO;
extern Model *modelBlankPanel;
extern Model *modelBlankPanelEmpty;
extern Model *modelBlankPanelSmall;
extern Model *modelBlankPanelWood;
extern Model *modelWestcoast;
extern Model *modelQuickMix;
extern Model *modelDiodeVCF;
extern Model *modelType35;
extern Model *modelTestDriver;
extern Model *modelEchoBox;
//extern Model *modelSpeck;
