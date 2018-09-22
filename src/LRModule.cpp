#include "LRComponents.hpp"
#include "LRModel.hpp"

using namespace rack;
using namespace lrt;


/**
 * Default constructor for a module
 */
LRModule::LRModule(int numParams, int numInputs, int numOutputs, int numLights) :
        Module(numParams, numInputs, numOutputs, numLights) {

}


/**
 * @brief Save default settings for a Module
 * @return
 */
json_t *LRModule::toJson() {
    json_t *root = json_object();

    LRGestalt gestaltid = *gestalt;

    // Gestalt ID
    json_object_set_new(root, "gestaltID", json_integer(gestaltid));

    return root;
}


/**
 * @brief Load default settings from a module
 * @param root
 */
void LRModule::fromJson(json_t *root) {
    json_t *gestaltID = json_object_get(root, "gestaltID");

    if (gestaltID) {
        switch (json_integer_value(gestaltID)) {
            case 0:
                *gestalt = dark;
                break;
            case 1:
                *gestalt = light;
                break;
            case 2:
                *gestalt = aged;
                break;
            default:
                *gestalt = dark;
        }
    }

}
