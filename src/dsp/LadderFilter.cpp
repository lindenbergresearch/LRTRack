#include "LadderFilter.hpp"

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
    q = 1.0f - freqExp;
    p = freqExp + 0.8f * freqExp * q;
    f = p + p - 1.0f;
    q = resExp * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
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

    b4 = clampf(b4 - b4 * b4 * b4 * b4 * b4 * 4, -1, 1) + rnd.nextFloat(-10e-8f, +10e-8f);
    //  b4 = lpf.filter(tanh(b4)) + rnd.nextFloat(-10e-8f, +10e-8f);
    //  b4 = (b4 - quadraticBipolar(b4)*0.1) + rnd.nextFloat(-10e-8f, +10e-8f);
    b0 = in;

    lpOut = b4;
}


/**
 * @brief Return cutoff frequency in the range of 0..1
 * @return
 */
float LadderFilter::getFrequency() const {
    return frequency;
}


/**
 * @brief Update cutoff frequency in the range of 0..1
 * @param frequency
 */
void LadderFilter::setFrequency(float frequency) {
    if (LadderFilter::frequency != frequency) {
        LadderFilter::frequency = frequency;
        // translate frequency to logarithmic scale
        freqHz = 20.f * powf(1000.f, frequency);
        freqExp = clampf(freqHz * (1.f / (engineGetSampleRate() * 8 / 2.f)), 0.f, 1.f);

        updateResExp();
        invalidate();
    }
}


/**
 * @brief Update non-linear resonance factor
 */
void LadderFilter::updateResExp() {
    resExp = clampf(resonance, 0, 1.5f);
    resExp = resExp * (1 - (drive / 3));
    // add some curve to resonance to avoid aliasing at high frequency
    // resExp *= (-0.50 * frequency + 1.1);
}


/**
 * @brief Get resonance
 * @return
 */
float LadderFilter::getResonance() const {
    return resExp;
}


/**
 * @brief Set resonance
 * @param resonance
 */
void LadderFilter::setResonance(float resonance) {
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
float LadderFilter::getDrive() const {
    return drive;
}


/**
 * @brief Set overdrive
 * @param drive
 */
void LadderFilter::setDrive(float drive) {
    if (LadderFilter::drive != drive) {
        LadderFilter::drive = drive;

        updateResExp();
        invalidate();
    }
}


/**
 * @brief Set input channel with sample
 * @param in
 */
void LadderFilter::setIn(float in) {
    LadderFilter::in = in;
}


/**
 * @brief Get lowpass output
 * @return
 */
float LadderFilter::getLpOut() {
    float out;// = clip(lpOut * (drive * 20 + 1), 0.7, -0.7) * (1/(drive+1)*12);
    float d = quadraticBipolar(drive) * 50 + 1;

    out = lpOut * (abs(lpOut) + d) / (lpOut * lpOut + (d - 1) * abs(lpOut) + 1);

    return out * 1 / (drive * 3 + 1);
}


float LadderFilter::getFreqHz() const {
    return freqHz;
}

