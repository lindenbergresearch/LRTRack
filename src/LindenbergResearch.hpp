#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"

using namespace rack;

/* panel dimensions */
#define BLANKPANEL_WIDTH 18.f
#define BLANKPANEL_MARK_I_WIDTH 12.f
#define FILTER_WIDTH 12.f
#define MS20_WIDTH 12.f
#define OSCILLATOR_WIDTH 12.f
#define RESHAPER_WIDTH 8.f
#define SIMPLEFILTER_WIDTH 10.f


extern Plugin *plugin;


/**
 * @brief Recover of old filer
 */
struct SimpleFilterWidget : LRModuleWidget {
    SimpleFilterWidget();
};


/**
 * @brief Valerie MS20 filter
 */
struct MS20FilterWidget : LRModuleWidget {
    MS20FilterWidget();
};


/**
 * @brief ALMA filter
 */
struct AlmaFilterWidget : LRModuleWidget {
    AlmaFilterWidget();
};


/**
 * @brief Blank Panel with Logo
 */
struct BlankPanelWidget : LRModuleWidget {
    BlankPanelWidget();
};


/**
 * @brief Blank Panel Mark I
 */
struct BlankPanelWidgetM1 : LRModuleWidget {
    BlankPanelWidgetM1();
};


/**
 * @brief Reshaper Panel
 */
struct ReShaperWidget : LRModuleWidget {
    ReShaperWidget();
};


/**
 * @brief Woldemar VCO
 */
struct VCOWidget : LRModuleWidget {
    VCOWidget();
};



