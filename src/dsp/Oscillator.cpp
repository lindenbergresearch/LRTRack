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
}