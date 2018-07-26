#pragma once

namespace rack {

    /**
     * @brief Base class for all signal processors
     */
    struct DSPEffect {

    private:
        float sr = 44100.0;

    public:

        float getSamplerate() const {
            return sr;
        }


        void setSamplerate(float sr) {
            DSPEffect::sr = sr;
        }


        virtual void init() {};


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