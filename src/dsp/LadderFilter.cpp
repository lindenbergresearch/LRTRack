#include "LadderFilter.hpp"
#include "DSPMath.hpp"

using namespace rack;


/**
 * @brief Constructor
 */
LadderFilter::LadderFilter() {}


/**
 * @brief Check parameter
 */
void LadderFilter::invalidate() {
    // Set coefficients given frequency & resonance [0.0...1.0]
    q = 1.0 - freqExp;
    p = freqExp + 0.8 * freqExp * q;
    f = p + p - 1.0;
    q = resExp * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));
}


/**
 * @brief Calculate new sample
 * @return
 */
void LadderFilter::process() {
    in -= q * b4;

    t1 = b1;
    b1 = (in + b0) * p - b1 * f;

    t2 = b2;
    b2 = (b1 + t1) * p - b2 * f;

    t1 = b3;
    b3 = (b2 + t2) * p - b3 * f;

    b4 = (b3 + t1) * p - b4 * f;

    b4 = b4 - b4 * b4 * b4 * 1.66666;
    b0 = in;

    lpOut = b4;// clip(b4, 0.9, -0.9);
}


/**
 * @brief Return cutoff frequency in the range of 0..1
 * @return
 */
sfloat LadderFilter::getFrequency() const {
    return frequency;
}


/**
 * @brief Update cutoff frequency in the range of 0..1
 * @param frequency
 */
void LadderFilter::setFrequency(sfloat frequency) {
    if (LadderFilter::frequency != frequency) {
        LadderFilter::frequency = frequency;
        // translate frequency to logarithmic scale
        freqHz = 19. * pow(1000., frequency);
        freqExp = clampd(freqHz * (1. / (engineGetSampleRate() * 8 / 2)), 0., 1.);

        updateResExp();
        invalidate();
    }
}


/**
 * @brief Update non-linear resonance factor
 */
void LadderFilter::updateResExp() {
    resExp = clampd(resonance, 0, 1.5);
    // add some curve to resonance to avoid aliasing at high frequency
    // resExp *= (-0.33 * frequency + 1.0);
}


/**
 * @brief Get resonance
 * @return
 */
sfloat LadderFilter::getResonance() const {
    return resExp;
}


/**
 * @brief Set resonance
 * @param resonance
 */
void LadderFilter::setResonance(sfloat resonance) {
    if (LadderFilter::resonance != resonance) {
        LadderFilter::resonance = resonance;

        updateResExp();
        invalidate();
    }
}


/**
 * @brief Get overdrive
 * @return
 */
sfloat LadderFilter::getDrive() const {
    return drive;
}


/**
 * @brief Set overdrive
 * @param drive
 */
void LadderFilter::setDrive(sfloat drive) {
    if (LadderFilter::drive != drive) {
        LadderFilter::drive = drive;
    }
}


/**
 * @brief Set input channel with sample
 * @param in
 */
void LadderFilter::setIn(sfloat in) {
    LadderFilter::in = in;
}


/**
 * @brief Get lowpass output
 * @return
 */
sfloat LadderFilter::getLpOut() {
    sfloat out = clip(lpOut * (drive * 5 + 1), 1, -1) * 12;

    out = lpf2.filter(out);

    return out;
}


sfloat LadderFilter::getFreqHz() const {
    return freqHz;
}

