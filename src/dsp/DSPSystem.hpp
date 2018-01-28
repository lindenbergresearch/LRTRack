#pragma once

#define DEFAULT_SR 44100.0f

/**
 * @brief Basic DSP types
 */

namespace dsp {

    /**
     * @brief Represents an IOPort of a DSP system
     */
    struct DSPPort {
        float value;
    };


    /**
     * @brief Represents a parameter of a DSP system
     */
    struct DSPParam {
        float value;
    };


    /**
     * @brief Base class for all signal processors
     */
    template<int NUM_IN, int NUM_OUT, int NUM_PARAM>
    struct DSPSystem {

        /**
         * @brief Enumerate all Inputs
         */
        enum Inputs {
        };

        /**
         * @brief Enumerate all Outputs
         */
        enum Outputs {
        };

        /**
         * @brief Enumerate all Parameters
         */
        enum Params {
        };

    protected:
        DSPPort input[NUM_IN] = {};
        DSPPort output[NUM_OUT] = {};
        DSPParam param[NUM_PARAM] = {};

        float sr;

    public:

        /**
         * @brief Default constructor
         */
        DSPSystem() {
            sr = DEFAULT_SR;
        }


        /**
         * @brief Init system with sample rate
         * @param sr
         */
        explicit DSPSystem(float sr) : sr(sr) {}


        /**
         * @brief Update sample rate on change
         * @param sr
         */
        void updateSampleRate(float sr) {
            DSPSystem::sr = sr;
            invalidate();
        }


        /**
         * @brief Update a parameter of the system
         * @param id Parameter ID
         * @param value Value
         * @param trigger Trigger call of invalidate() - use false to supress
         */
        void setParam(int id, float value, bool trigger = true) {
            if (param[id].value != value) {
                param[id].value = value;

                /* setup of new parameter triggers invalidation per default */
                if (trigger) {
                    invalidate();
                }
            }

        }


        /**
         * @brief Get the current parameter value by ID
         * @param id Parameter ID
         * @return current value
         */
        float getParam(int id) {
            return param[id].value;
        }


        /**
         * @brief Get the current value of the output by ID
         * @param id
         * @return
         */
        float getOutput(int id) {
            return output[id].value;
        }


        /**
         * @brief Set input port to new value
         * @param id Port ID
         * @param value
         */
        void setInput(int id, float value, bool proccess = false) {
            input[id].value = value;

            if (proccess) {
                process();
            }
        }


        /**
         * @brief Get current output value
         * @param id Output ID
         * @return
         */
        float getOut(int id) {
            return output[id].value;
        }


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