#pragma once

/**
 * @brief Basic DSP types
 */
namespace dsp {

    /**
     * @brief Represents an IOPort of an DSP system
     */
    struct DSPPort {
        float value;
    };

    /**
     * @brief Base class for all signal processors
     */
    struct DSPSystem {

        /**
         * @brief Method for mark parameters as invalidate to trigger recalculation
         */
        virtual void invalidate() {};


        /**
         * @brief Process one step and return the computed sample
         * @return
         */
        virtual void process() {};
    };

}