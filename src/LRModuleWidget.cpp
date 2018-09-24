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

    auto *darkGestaltItem = new GestaltItem(dark, this);
    darkGestaltItem->text = "Dark";
    menu->addChild(darkGestaltItem);

    if (count > 1) {
        auto *lightGestaltItem = new GestaltItem(light, this);
        lightGestaltItem->text = "Light";
        menu->addChild(lightGestaltItem);
    } else {
        auto *lightGestaltLabel = new MenuLabel();
        lightGestaltLabel->text = "Light";
        menu->addChild(lightGestaltLabel);
    }

    if (count > 2) {
        auto *agedGestaltItem = new GestaltItem(aged, this);
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

    auto *gradientItem = new GradientItem(this);
    gradientItem->text = "Metallic Look";
    menu->addChild(gradientItem);

    return menu;
}