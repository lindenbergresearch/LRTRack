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

#include "LRGestalt.hpp"
#include "rack.hpp"

using namespace rack;

namespace lrt {

/**
 * @brief Virtual Event baseclass
 */
struct LREvent {
    /** marker for consumption */
    bool consumed = false;


    /**
     * @brief
     */
    virtual void consume() { consumed = true; }
};


/**
 * @brief Virtual Action baseclass
 */
struct LREventAction {

    /**
     * @brief To be overridden event catcher
     * @param e
     */
    virtual void onEventAction(LREvent *e) {};
};


}

