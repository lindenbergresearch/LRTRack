#include "dsp/helper.hpp"
#include "dsp/Oscillator.hpp"

namespace rack {


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
     * @brief Default constructor
     */
    Oscillator::Oscillator() {
        reset();
    }


    /**
     * @brief Default destructor
     */
    Oscillator::~Oscillator() {}


    /**
     * @brief Get current frequency
     * @return
     */
    float Oscillator::getFreq() const {
        return freq;
    }


    /**
     * @brief Set frequency
     * @param freq
     */
    void Oscillator::setFreq(float freq) {
        Oscillator::freq = freq;
        invalidate();
    }


    /**
     * @brief Get current pulse-width
     * @return
     */
    float Oscillator::getPw() const {
        return pw;
    }


    /**
     * @brief Set current pulse-width
     * @param pw
     */
    void Oscillator::setPw(float pw) {
        Oscillator::pw = pw;
        invalidate();
    }


    /**
     * @brief Ramp waveform current
     * @return
     */
    float Oscillator::getRamp() const {
        return ramp;
    }


    /**
     * @brief Saw waveform current
     * @return
     */
    float Oscillator::getSaw() const {
        return saw;
    }


    /**
     * @brief Pulse waveform current
     * @return
     */
    float Oscillator::getPulse() const {
        return pulse;
    }


    /**
     * @brief SawTri waveform current
     * @return
     */
    float Oscillator::getSawtri() const {
        return sawtri;
    }


    /**
     * @brief Triangle waveform current
     * @return
     */
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
        incr = getPhaseIncrement(BLITOscillator::freq);
        N = (int) floorf(harmonics / freq);
    }


    /**
     * @brief Get current BLIT harmonics
     * @return
     */
    float BLITOscillator::getHarmonics() const {
        return harmonics;
    }


    /**
     * @brief Set BLIT harmonics
     * @param harmonics
     */
    void BLITOscillator::setHarmonics(float harmonics) {
        BLITOscillator::harmonics = harmonics;
        invalidate();
    }

}