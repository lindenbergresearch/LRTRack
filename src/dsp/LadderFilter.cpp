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
    os.next(in);
    os.doUpsample();

    for (int i = 0; i < os.factor; i++) {
        float x = os.up[i];

        // non linear feedback with nice saturation
        x -= fastatan(bx * q);

        t1 = b1;
        b1 = ((x + b0) * p - b1 * f);

        t2 = b2;
        b2 = ((b1 + t1) * p - b2 * f);

        t1 = b3;
        b3 = ((b2 + t2) * p - b3 * f);

        t2 = b4;
        b4 = ((b3 + t1) * p - b4 * f);

        b5 = ((b4 + t2) * p - b5 * f);

        // fade over filter poles from 3dB/oct (1P) => 48dB/oct (5P)
        bx = fade5(b1, b2, b3, b4, b5, slope);

        // saturate and add very low noise to have self oscillation with no input and high res
        b0 = fastatan(x + noise.nextFloat(NOISE_GAIN));

        // overdrive with fastatan, which folds back the waves at high input and creates a noisy bright sound
        os.data[LOWPASS][i] = fastatan(bx * (1 + drive * 50));
    }

    lpOut = os.getDownsampled(LOWPASS);
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
        freqExp = clampf(freqHz * (1.f / (engineGetSampleRate() * OVERSAMPLE / 2.f)), 0.f, 1.f);

        updateResExp();
        invalidate();
    }
}


/**
 * @brief Update resonance factor
 */
void LadderFilter::updateResExp() {
    resExp = clampf(resonance, 0, 1.5f);
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
        LadderFilter::drive = clampf(drive, 0, 1);

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
    return lpOut;
}


/**
 * @brief Get frequency of cutoff in Hz
 * @return
 */
float LadderFilter::getFreqHz() const {
    return freqHz;
}


/**
 * @brief Get filter slope
 * @return
 */
float LadderFilter::getSlope() const {
    return slope;
}


/**
 * @brief Set filter slope
 * @param slope
 */
void LadderFilter::setSlope(float slope) {
    if (LadderFilter::slope != slope) {
        LadderFilter::slope = clampf(slope, 0, 4);
    }
}
