#include "helper.hpp"
#include "Oscillator.hpp"

namespace rack {
    /**
     * @brief Set oscillator state back
     */
    void BLITOscillator::reset() {
        freq = 440.f;
        pw = 0.5f;
        phase = 0.f;
        incr = 0.f;

        invalidate();
    }


    /**
     * @brief Default constructor
     */
    BLITOscillator::BLITOscillator() {
        reset();
    }


    /**
     * @brief Default destructor
     */
    BLITOscillator::~BLITOscillator() {}


    /**
     * @brief Get current frequency
     * @return
     */
    float BLITOscillator::getFreq() const {
        return freq;
    }


    /**
     * @brief Set frequency
     * @param freq
     */
    void BLITOscillator::setFreq(float freq) {
        BLITOscillator::freq = freq;
        invalidate();
    }


    /**
     * @brief Get current pulse-width
     * @return
     */
    float BLITOscillator::getPw() const {
        return pw;
    }


    /**
     * @brief Set current pulse-width
     * @param pw
     */
    void BLITOscillator::setPw(float pw) {
        if (pw < 0.f) {
            BLITOscillator::pw = 0.f;
            return;
        }

        if (pw > 1.f) {
            BLITOscillator::pw = 1.f;
            return;
        }

        BLITOscillator::pw = pw;
        invalidate();
    }


    /**
     * @brief Ramp waveform current
     * @return
     */
    float BLITOscillator::getRamp() const {
        return ramp;
    }


    /**
     * @brief Saw waveform current
     * @return
     */
    float BLITOscillator::getSaw() const {
        return saw;
    }


    /**
     * @brief Pulse waveform current
     * @return
     */
    float BLITOscillator::getPulse() const {
        return pulse;
    }


    /**
     * @brief SawTri waveform current
     * @return
     */
    float BLITOscillator::getSawtri() const {
        return sawtri;
    }


    /**
     * @brief Triangle waveform current
     * @return
     */
    float BLITOscillator::getTri() const {
        return tri;
    }


    /**
     * @brief Process band-limited oscillator
     */
    void BLITOscillator::proccess() {
        /* phase locked loop */
        phase = wrapTWOPI(incr + phase);

        // TODO: compute waveforms
    }


    /**
     * @brief ReCompute basic parameter
     */
    void BLITOscillator::invalidate() {
        incr = getPhaseIncrement(freq);
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