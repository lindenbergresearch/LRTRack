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
**    Copyright 2017/2018 by Patrick Lindenberg / LRT                  **
**                                                                     **
**    For Redistribution and use in source and binary forms,           **
**    with or without modification please see LICENSE.                 **
**                                                                     **
\*                                                                     */

#pragma once

#include <widgets.hpp>
#include <map>

using namespace rack;
using std::vector;
using std::shared_ptr;
using std::string;
using std::map;


namespace lrt {

/**
 * @brief Gestalt IDs
 */
enum LRGestalt : int {
    NIL,    // virtuell element to mark unset
    DARK,   // DARK theme (as standard)
    LIGHT,  // LIGHT theme
    AGED    // LIGHT theme with AGED look
};


/**
 * @brief
 */
struct LREventGestaltChange : EventChange {
    LRGestalt current, last;
};

/**
 * Represents all data needed by skinned versions of UI
 */
struct LRGestaltModifier {

    /* pointer to current skin id */
    LRGestalt *gestalt = nullptr;

    /* holds the last used ID for recognizing changes */
    LRGestalt prevID = NIL; //init with unset to trigger first invalidation

    /* SVG pool - Holds all needed SVG images */
    map<LRGestalt, shared_ptr<SVG>> pool;


    //LRGestaltModifier(LRGestalt *gestalt) : gestalt(gestalt) {}


    /*
     * Check if gestalt has been changed
     */
    bool invalidGestalt() {
        if (gestalt != nullptr) {
            auto changed = *gestalt != prevID;

            // trigger event handler
            if (changed) onGestaltChange();

            return changed;
        }

        return false;
    }


    /**
     * @brief Synchronize local gestalt ID
     */
    void syncGestalt() {
        prevID = *gestalt;
    }


    virtual void onGestaltChange() {}


    /**
     * @brief Add new SVG to variant pool
     * @param gestalt Matching ID for variant
     * @param svg SVG Image
     */
    void addSVGVariant(LRGestalt gestalt, shared_ptr<SVG> svg) {
        pool[gestalt] = svg;

        /* first element inserted => set default */
        if (pool.size() == 1) {
            pool[LRGestalt::NIL] = svg;
        }
    }


    shared_ptr<SVG> getSVGVariant() {
        return getSVGVariant(*gestalt);
    }


    /**
     * @brief Get SVG Image from pool matching the gestalt
     * @param gestalt
     * @return SVG Image if found, default if not found
     */
    shared_ptr<SVG> getSVGVariant(LRGestalt gestalt) {

        /* return default value if key not found */
        if (pool.count(gestalt) != 1) {
            return pool[LRGestalt::NIL];
        }

        return pool[gestalt];
    }

};

}
