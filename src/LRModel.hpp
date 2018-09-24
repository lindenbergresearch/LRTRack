#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"
#include "LRComponents.hpp"


namespace lrt {

/**
 * @brief Standard LR Module definition
 */
struct LRModule : public Module {
    LRGestalt *gestalt = nullptr;


    json_t *toJson() override;
    void fromJson(json_t *root) override;

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

    /* Gestalt ID which represents the current theme */
    LRGestalt gestalt = dark;


    /**
     * @brief Default constructor derived from rack
     * @param module LRModule instance
     */
    explicit LRModuleWidget(LRModule *module) : ModuleWidget(module) {
        module->gestalt = &gestalt;
    }


    /**
     * @brief Represents an Item for selecting the gestalt
     */
    struct GestaltItem : MenuItem {
        LRGestalt gestaltM;
        LRModuleWidget *widget;


        GestaltItem(LRGestalt gestaltM, LRModuleWidget *widget) : gestaltM(gestaltM), widget(widget) {}


        void onAction(EventAction &e) override {
            if (widget != nullptr) {
                // set new global gestalt to current ID of selected menu item
                widget->gestalt = gestaltM;
            }
        }


        void step() override {
            rightText = (widget->gestalt == gestaltM) ? "✔" : "";
        }
    };


    struct GradientItem : MenuItem {
        LRModuleWidget *widget;


        GradientItem(LRModuleWidget *widget) : widget(widget) {}


        void onAction(EventAction &e) override {
            if (widget != nullptr) {
                // swap bool
                widget->panel->setGradient(!widget->panel->isGradient());
                widget->panel->setGradientVariant(widget->panel->isGradient());
                widget->panel->dirty = true;
            }
        }


        void step() override {
            rightText = widget->panel->isGradient() ? "✔" : "";
        }
    };


    /**
     * @brief Create standard menu for all modules
     * @return
     */
    Menu *createContextMenu() override;
};


}