#pragma once

#include <cmath>
#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"

/* panel dimensions */
#define BLANKPANEL_WIDTH 18.f
#define BLANKPANEL_MARK_I_WIDTH 12.f
#define FILTER_WIDTH 12.f
#define MS20_WIDTH 12.f
#define OSCILLATOR_WIDTH 12.f
#define RESHAPER_WIDTH 8.f
#define SIMPLEFILTER_WIDTH 10.f

using namespace rack;

extern Model *modelSimpleFilter;
extern Model *modelMS20Filter;
extern Model *modelAlmaFilter;
extern Model *modelReShaper;
extern Model *modelVCO;
extern Model *modelBlankPanel;
extern Model *modelBlankPanelM1;
