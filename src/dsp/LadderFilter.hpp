#pragma once


#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"


namespace rack {

    struct LadderFilter : DSPEffect {
    private:
        float f, p, q;
        float b0, b1, b2, b3, b4;
        float t1, t2;
        float freqExp, freqHz, frequency, resExp, resonance, drive;
        float in, lpOut;

        Randomizer rnd;

        void updateResExp();

    public:
        LadderFilter();

        void invalidate() override;

        void process() override;

        float getFrequency() const;
        void setFrequency(float frequency);
        float getResonance() const;
        void setResonance(float resonance);
        float getDrive() const;
        void setDrive(float drive);
        float getFreqHz() const;

        void setIn(float in);
        float getLpOut();

    };
}