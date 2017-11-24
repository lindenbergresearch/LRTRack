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

        /* force recalculation of variables */
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
    float BLITOscillator::getFrequency() const {
        return freq;
    }


    /**
     * @brief Set frequency
     * @param freq
     */
    void BLITOscillator::setFrequency(float freq) {
        BLITOscillator::freq = freq;

        /* force recalculation of variables */
        invalidate();
    }


    /**
     * @brief Get current pulse-width
     * @return
     */
    float BLITOscillator::getPulseWidth() const {
        return pw;
    }


    /**
     * @brief Set current pulse-width
     * @param pw
     */
    void BLITOscillator::setPulseWidth(float pw) {
        if (pw < 0.f) {
            BLITOscillator::pw = 0.f;
            return;
        }

        if (pw > 1.f) {
            BLITOscillator::pw = 1.f;
            return;
        }

        BLITOscillator::pw = pw;

        /* force recalculation of variables */
        invalidate();
    }


    /**
     * @brief Ramp waveform current
     * @return
     */
    float BLITOscillator::getRampWave() const {
        return ramp;
    }


    /**
     * @brief Saw waveform current
     * @return
     */
    float BLITOscillator::getSawWave() const {
        return saw;
    }


    /**
     * @brief Pulse waveform current
     * @return
     */
    float BLITOscillator::getPulseWave() const {
        return pulse;
    }


    /**
     * @brief SawTri waveform current
     * @return
     */
    float BLITOscillator::getSawTriWave() const {
        return sawtri;
    }


    /**
     * @brief Triangle waveform current
     * @return
     */
    float BLITOscillator::getTriangleWave() const {
        return tri;
    }


    /**
     * @brief Process band-limited oscillator
     */
    void BLITOscillator::proccess() {
        /* phase locked loop */
        phase = wrapTWOPI(incr + phase);

        /* get impulse train */
        float blit = BLIT(N, phase);

        /* compute RAMP waveform */
        ramp = int1.add(blit, incr) - 0.1f;
        /* compute SAW waveform */
        saw = ramp * -1.f;

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

        /* force recalculation of variables */
        invalidate();
    }

}