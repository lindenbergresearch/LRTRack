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
     * @brief System parameter
     */
    struct DSPParam {
        float value;
    };


    /**
     * @brief Base class for all signal processors
     */
    template<int NUM_IN, int NUM_OUT, int NUM_PARAM>
    struct DSPSystem {
    private:
        DSPPort input[NUM_IN];
        DSPPort output[NUM_OUT];
        DSPParam param[NUM_PARAM];

        int sr;

    public:
        /**
         * @brief Init system with samplerate
         * @param sr
         */
        DSPSystem(int sr) : sr(sr) {}


        /**
         * @brief Update samplerate on change
         * @param sr
         */
        void updateSampleRate(int sr) {
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