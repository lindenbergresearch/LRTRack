#include "dsp/helper.hpp"
#include "dsp/Oscillator.hpp"

using namespace rack;


void Oscillator::reset() {
    freq = 440.f;
    pw = 0.5f;
    phase = 0.f;
    incr = 0.f;

    invalidate();
}


Oscillator::Oscillator(float freq, float pw) : freq(freq), pw(pw) {}


Oscillator::~Oscillator() {}
