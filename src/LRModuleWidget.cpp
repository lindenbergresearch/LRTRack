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

    return menu;
}


/**
 * @brief Save default settings for a Module
 * @return
 */
json_t *LRModuleWidget::toJson() {
    json_t *root = json_object();

    LRGestalt gestaltid = gestalt;

    // Gestalt ID
    json_object_set_new(root, "gestaltID", json_integer(gestaltid));

    debug("[%p] write module json", this);

    return root;
}


/**
 * @brief Load default settings from a module
 * @param root
 */
void LRModuleWidget::fromJson(json_t *root) {
    json_t *gestaltID = json_object_get(root, "gestaltID");

    debug("[%p] read module json", this);

    if (gestaltID) {
        switch (json_integer_value(gestaltID)) {
            case 0:
                gestalt = DARK;
                break;
            case 1:
                gestalt = LIGHT;
                break;
            case 2:
                gestalt = AGED;
                break;
            default:
                gestalt = DARK;
        }
    }

}
