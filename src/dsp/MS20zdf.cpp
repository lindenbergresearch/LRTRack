
#include "MS20zdf.hpp"


/**
 * @brief Get current filter cutoff frequency
 * @return
 */
float rack::MS20zdf::getFrequency() const {
    return frequency;
}


/**
 * @brief Set filter cutoff frequency and trigger recalculation of prewarped vars
 * @param frequency
 */
void rack::MS20zdf::setFrequency(float frequency) {
    if (MS20zdf::frequency != frequency) {
        MS20zdf::frequency = frequency;

        invalidate();
    }
}


/**
 * @brief Get filter peak (resonance)
 * @return
 */
float rack::MS20zdf::getPeak() const {
    return peak;
}


/**
 * @brief Set filter peak (resonance) and trigger recalculation of prewarped vars
 * @param peak
 */
void rack::MS20zdf::setPeak(float peak) {
    if (MS20zdf::peak != peak) {
        MS20zdf::peak != peak;

        invalidate();
    }
}


/**
 * @brief Calculate prewarped vars on parameter change
 */
void MS20zdf::invalidate() {
    DSPEffect::invalidate();
}


/**
 * @brief Proccess one sample of filter
 */
void MS20zdf::process() {
    DSPEffect::process();
}
