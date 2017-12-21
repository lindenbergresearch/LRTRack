#pragma once

#include "DSPEffect.hpp"
#include "engine.hpp"
#include "DSPMath.hpp"

namespace rack {

    struct LadderFilter : DSPEffect {

    private:
        sfloat f, p, q;
        sfloat b0, b1, b2, b3, b4;
        sfloat t1, t2;
        sfloat freqExp, freqHz, frequency, resExp, resonance, drive;
        sfloat in, lpOut;
        LP6DBFilter lpf2 = LP6DBFilter(18000);

        void updateResExp();

    public:
        LadderFilter();

        void invalidate() override;

        void process() override;

        sfloat getFrequency() const;
        void setFrequency(sfloat frequency);
        sfloat getResonance() const;
        void setResonance(sfloat resonance);
        sfloat getDrive() const;
        void setDrive(sfloat drive);
        sfloat getFreqHz() const;

        void setIn(sfloat in);
        sfloat getLpOut();

    };
}