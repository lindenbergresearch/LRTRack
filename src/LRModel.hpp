/*                                                                     *\
**       __   ___  ______                                              **
**      / /  / _ \/_  __/                                              **
**     / /__/ , _/ / /    Lindenberg                                   **
**    /____/_/|_| /_/  Research Tec.                                   **
**                                                                     **
**                                                                     **
**	  https://github.com/lindenbergresearch/LRTRack	                   **
**    heapdump@icloud.com                                              **
**		                                                               **
**    Sound Modules for VCV Rack                                       **
**    Copyright 2017-2019 by Patrick Lindenberg / LRT                  **
**                                                                     **
**    For Redistribution and use in source and binary forms,           **
**    with or without modification please see LICENSE.                 **
**                                                                     **
\*                                                                     */
#pragma once

#include "LRComponents.hpp"
#include "rack.hpp"
#include "asset.hpp"

namespace lrt {

using std::vector;

static const char *const JSON_GESTALT_KEY = "gestaltID";
static const char *const JSON_GRADIENT_KEY = "gradient";
static const char *const JSON_PATINA_KEY = "patina";

static const char *const JSON_PATINA_A_X = "patina_a_x";
static const char *const JSON_PATINA_A_Y = "patina_a_y";

static const char *const JSON_PATINA_B_X = "patina_b_x";
static const char *const JSON_PATINA_B_Y = "patina_b_y";

static const string STR_CHECKMARK_UNICODE = "✔";

// forward declaration
struct LRModuleWidget;


/**
 * @brief Standard LR Module definition
 */
struct LRModule : public Module {

    // reflect back to push events to UI
    LRModuleWidget *reflect;

    /**
     * @brief Default constructor derived from rack
     * @param numParams
     * @param numInputs
     * @param numOutputs
     * @param numLights
     */
    explicit LRModule(int numParams, int numInputs, int numOutputs, int numLights);
};


/**
 * @brief Standard LR ModuleWidget definition
 */
struct LRModuleWidget : ModuleWidget {

    /* standard module panel */
    LRPanel *panel = nullptr;


    /* Gestalt ID and UI settings */
    LRGestalt gestalt = LIGHT;      // DARK == default
    LRGestalt prevGestalt = NIL;    // set to NIL to trigger change event on first start

    bool gradient = true;           // gradient used at panel
    bool patina = false;            // patina used at panel
    bool noVariants = false;        // if set all gestalt options and menus are disabled
    bool isPreview = false;         // wiget is created in module less state for preview etc.


    /**
     * @brief Default constructor derived from rack
     * @param module LRModule instance
     */
    explicit LRModuleWidget(LRModule *module) {
        setModule(module);

        if (!module) isPreview = true;

        if (module) {
            module->reflect = this;
        }

        panel = new LRPanel();
    }


    /**
     * @brief Represents an Item for selecting the gestalt
     */
    struct GestaltItem : MenuItem {
        LRGestalt gestaltM;
        LRModuleWidget *widget;


        GestaltItem(LRGestalt gestaltM, LRModuleWidget *widget) : gestaltM(gestaltM), widget(widget) {}


        void onAction(const event::Action &e) override {
            if (widget != nullptr) {
                // set new global gestalt to current ID of selected menu item
                widget->gestalt = gestaltM;
            }
        }


        void step() override {
            rightText = (widget->gestalt == gestaltM) ? STR_CHECKMARK_UNICODE : "";
        }
    };


    /**
     * @brief Represents a gradient select item
     */
    struct GradientItem : MenuItem {
        LRPanel *panel;


        explicit GradientItem(LRPanel *panel) : panel(panel) {}


        void onAction(const event::Action &e) override {
            if (panel != nullptr) {
                panel->setGradientVariant(true);
                panel->dirty = true;
            }
        }


        void step() override {
            rightText = *panel->gradient ? STR_CHECKMARK_UNICODE : "";
        }
    };


    /**
     * @brief Represents a gradient select item
     */
    struct PatinaItem : MenuItem {
        LRPanel *panel;


        explicit PatinaItem(LRPanel *panel) : panel(panel) {}


        void onAction(const event::Action &e) override {
            if (panel != nullptr) {
                panel->setPatina(!*panel->patina); // invert flag on trigger
                panel->dirty = true;
            }
        }


        void step() override {
            rightText = *panel->patina ? STR_CHECKMARK_UNICODE : "";
        }
    };


    void step() override;
    void appendContextMenu(ui::Menu *menu) override;
    json_t *toJson() override;
    void fromJson(json_t *rootJ) override;

    //void randomize() override;
    //TODO: check randomize on widgets!
};


}