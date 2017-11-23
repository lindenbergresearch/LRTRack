#include "dsp/helper.hpp"
#include "dsp/Oscillator.hpp"

using namespace rack;


/**
 * @brief Set oscillator state back
 */
void Oscillator::reset() {
    freq = 440.f;
    pw = 0.5f;
    phase = 0.f;
    incr = 0.f;

    invalidate();
}


/**
 * @brief Construct a new oscillator with given parameter
 * @param freq Initial frequency
 * @param pw Initial pulse-width
 */
Oscillator::Oscillator(float freq, float pw) : freq(freq), pw(pw) {}


/**
 * @brief Default destructor
 */
Oscillator::~Oscillator() {}


/**
 * @brief Default constructor
 */
Oscillator::Oscillator() {
    reset();
}


float Oscillator::getFreq() const {
    return freq;
}


void Oscillator::setFreq(float freq) {
    Oscillator::freq = freq;
}


float Oscillator::getPw() const {
    return pw;
}


void Oscillator::setPw(float pw) {
    Oscillator::pw = pw;
}


float Oscillator::getRamp() const {
    return ramp;
}


float Oscillator::getSaw() const {
    return saw;
}


float Oscillator::getPulse() const {
    return pulse;
}


float Oscillator::getSawtri() const {
    return sawtri;
}


float Oscillator::getTri() const {
    return tri;
}


/**
 * @brief Process band-limited oscillator
 */
void BLITOscillator::proccess() {
    /* phase locked loop */
    phase = wrapTWOPI(incr + phase);


}


/**
 * @brief ReCompute basic parameter
 */
void BLITOscillator::invalidate() {
    incr = getPhaseIncrement(this->freq);
    N = floorf(harmonics / freq);
}


float BLITOscillator::getHarmonics() const {
    return harmonics;
}


void BLITOscillator::setHarmonics(float harmonics) {
    BLITOscillator::harmonics = harmonics;
}
