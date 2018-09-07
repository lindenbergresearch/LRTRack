#pragma once

#include "DSPEffect.hpp"
#include "DSPMath.hpp"

namespace dsp {

struct DiodeLadderStage : DSPEffect {

    float fc; // cutoff frq
    float alpha, beta, gamma, delta, epsilon;
    float gain;
    float feedback;
    float z1;

    float in, out;

    explicit DiodeLadderStage(float sr);
    void init() override;
    void invalidate() override;
    void process() override;


    void resetZ1() {
        z1 = 0;
    }


    float getZ1() {
        return z1;
    }


    void setFeedback(float fb) {
        feedback = fb;
    }


    float getFeedbackOutput();
};


struct DiodeLadderFilter : DSPEffect {

    float fc, k, saturation;

    DiodeLadderStage *lpf1, *lpf2, *lpf3, *lpf4;
    float gamma;

    float sg1, sg2, sg3, sg4;

    explicit DiodeLadderFilter(float sr);
    void init() override;
    void invalidate() override;
    void process() override;


    void reset() {
        lpf1->resetZ1();
        lpf2->resetZ1();
        lpf3->resetZ1();
        lpf4->resetZ1();

        lpf1->setFeedback(0.f);
        lpf2->setFeedback(0.f);
        lpf3->setFeedback(0.f);
        lpf4->setFeedback(0.f);
    }
};

}
