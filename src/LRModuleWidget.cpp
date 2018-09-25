#include "LRComponents.hpp"
#include "LRModel.hpp"

using namespace rack;
using namespace lrt;


Menu *LRModuleWidget::createContextMenu() {
    Menu *menu = ModuleWidget::createContextMenu();

    unsigned long count = panel->pool.size();

    auto *spacerLabel = new MenuLabel();
    menu->addChild(spacerLabel);

    auto *sectionLabel = new MenuLabel();
    sectionLabel->text = "Module Gestalt";
    menu->addChild(sectionLabel);

    auto *darkGestaltItem = new GestaltItem(DARK, this);
    darkGestaltItem->text = "Dark";
    menu->addChild(darkGestaltItem);

    if (count > 1) {
        auto *lightGestaltItem = new GestaltItem(LIGHT, this);
        lightGestaltItem->text = "Light";
        menu->addChild(lightGestaltItem);
    } else {
        auto *lightGestaltLabel = new MenuLabel();
        lightGestaltLabel->text = "Light";
        menu->addChild(lightGestaltLabel);
    }

    if (count > 2) {
        auto *agedGestaltItem = new GestaltItem(AGED, this);
        agedGestaltItem->text = "Aged";
        menu->addChild(agedGestaltItem);
    } else {
        auto *agedGestaltLabel = new MenuLabel();
        agedGestaltLabel->text = "Aged";
        menu->addChild(agedGestaltLabel);
    }

    auto *spacerLabelBottom = new MenuLabel();
    menu->addChild(spacerLabelBottom);

    auto *sectionLabelBottom = new MenuLabel();
    sectionLabelBottom->text = "Module Style";
    menu->addChild(sectionLabelBottom);

    auto *gradientItem = new GradientItem(this->panel);
    gradientItem->text = "Metallic Look";
    menu->addChild(gradientItem);

    auto *patinaItem = new PatinaItem(this->panel);
    patinaItem->text = "Used Look";
    menu->addChild(patinaItem);

    return menu;
}


/**
 * @brief Load UI relevant settings
 * @return
 */
json_t *LRModuleWidget::toJson() {
    auto *rootJ = ModuleWidget::toJson();

    LRGestalt gestaltid = gestalt;

    json_object_set_new(rootJ, JSON_GESTALT_KEY, json_integer(gestaltid));
    json_object_set_new(rootJ, JSON_GRADIENT_KEY, json_boolean(gradient));
    json_object_set_new(rootJ, JSON_PATINA_KEY, json_boolean(patina));

    debug("[%p] write module json", this);

    return rootJ;
}


/**
 * @brief Save UI relevant settings
 * @param rootJ
 */
void LRModuleWidget::fromJson(json_t *rootJ) {
    ModuleWidget::fromJson(rootJ);

    debug("[%p] read module json: ", this);
    json_t *gestaltID = json_object_get(rootJ, JSON_GESTALT_KEY);
    json_t *gradientJ = json_object_get(rootJ, JSON_GRADIENT_KEY);
    json_t *patinaJ = json_object_get(rootJ, JSON_PATINA_KEY);


    if (gradientJ)
        gradient = json_is_true(gradientJ);

    if (patinaJ)
        patina = json_is_true(patinaJ);


    if (gestaltID) {
        switch (json_integer_value(gestaltID)) {
            case 1:
                gestalt = DARK;
                break;
            case 2:
                gestalt = LIGHT;
                break;
            case 3:
                gestalt = AGED;
                break;
            default:
                gestalt = DARK;
        }
    }

}
