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

#include <map>
#include "LREvent.hpp"

using namespace rack;
using std::vector;
using std::shared_ptr;
using std::string;
using std::map;

namespace lrt {

/**
 * @brief Gestalt IDs
 */
enum LRGestaltType : int {
    NIL,    // virtuell element to mark unset
    DARK,   // DARK theme (as standard)
    LIGHT,  // LIGHT theme
    AGED    // LIGHT theme with AGED look
};


/**
 * @brief
 */
struct LRGestaltChangeEvent : LREvent {

    /** gestalt type transition and setup */
    LRGestaltType previous, current;
    bool patina, gradient;


    LRGestaltChangeEvent(LRGestaltType previous, LRGestaltType current, bool patina, bool gradient) : previous(previous), current(current),
                                                                                                      patina(patina), gradient(gradient) {}
};


/**
 * @brief Event Action interface for gestalt changes
 */
struct LRGestaltChangeAction : LREventAction {


    /**
     * @brief To be implemented
     * @param e
     */
    virtual void onGestaltChangeAction(LRGestaltChangeEvent &e) = 0;

};


/**
 * Represents all data needed by skinned versions of UI
 */
struct LRGestaltVariant {

    /* SVG pool - Holds all needed SVG images */
    map<LRGestaltType, shared_ptr<Svg>> pool;


    /**
     * @brief Add new SVG to variant pool
     * @param gestalt Matching ID for variant
     * @param svg SVG Image
     */
    void addSVGVariant(LRGestaltType gestalt, shared_ptr<Svg> svg) {
        pool[gestalt] = svg;

        /* first element inserted => set default */
        if (pool.size() == 1) {
            pool[LRGestaltType::NIL] = svg;
        }
    }


    /**
     * @brief Get SVG Image from pool matching the gestalt
     * @param gestalt
     * @return SVG Image if found, default if not found
     */
    shared_ptr<Svg> getSVGVariant(LRGestaltType gestalt) {

        /* return default value if key not found */
        if (pool.count(gestalt) != 1) {
            return pool[LRGestaltType::NIL];
        }

        return pool[gestalt];
    }

};

}
