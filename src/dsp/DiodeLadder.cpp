#include "DiodeLadder.hpp"

using namespace dsp;


DiodeLadderStage::DiodeLadderStage(float sr) : DSPEffect(sr) {

}


void DiodeLadderStage::init() {
    alpha = 1.f;
    beta = -1.f;
    gamma = 1.f;
    delta = 0.f;
    epsilon = 1.f;
    feedback = 0.f;
    gain = 1.f;

    resetZ1();
}


void DiodeLadderStage::invalidate() {
    DSPEffect::invalidate();
}


void DiodeLadderStage::process() {
    float x = (in * gamma + feedback + epsilon * getFeedbackOutput());
    float vn = (gain * x - z1) * alpha;
    float out = vn + z1;

    z1 = vn + out;
}


float DiodeLadderStage::getFeedbackOutput() {
    return (z1 + feedback * delta) * beta;
}


DiodeLadderFilter::DiodeLadderFilter(float sr) : DSPEffect(sr) {
    lpf1 = new DiodeLadderStage(sr);
    lpf2 = new DiodeLadderStage(sr);
    lpf3 = new DiodeLadderStage(sr);
    lpf4 = new DiodeLadderStage(sr);

    gamma = 0.f;
    k = 0.f;

    sg1 = 0.f;
    sg2 = 0.f;
    sg3 = 0.f;
    sg4 = 0.f;

    lpf1->gain = 1.f;
    lpf2->gain = 0.5f;
    lpf3->gain = 0.5f;
    lpf4->gain = 0.5f;

    lpf4->gamma = 1.f;
    lpf4->delta = 0.f;
    lpf4->epsilon = 0.f;
    lpf4->setFeedback(0.f);
}


void DiodeLadderFilter::init() {
    DSPEffect::init();
}


void DiodeLadderFilter::invalidate() {
    float G1, G2, G3, G4;

    float wd = TWOPI * fc;
    float T = 1 / sr;
    float wa = (2 / T) * tanf(wd * T / 2);
    float g = wa * T / 2;


}


void DiodeLadderFilter::process() {
    DSPEffect::process();
}
