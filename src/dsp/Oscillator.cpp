#include "helper.hpp"
#include "Oscillator.hpp"

using namespace rack;


/**
 * @brief Set oscillator state back
 */
void BLITOscillator::reset() {
    freq = 440.f;
    pw = 1.f;
    phase = 0.f;
    incr = 0.f;
    saturate = 1.f;

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
    if (BLITOscillator::freq != freq){
        BLITOscillator::freq = freq;

        /* force recalculation of variables */
        invalidate();}
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
    if (pw < 0.1f) {
        BLITOscillator::pw = 0.1f;
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

    /* pulse width */
    float w = pw * (float) M_PI;

    /* get impulse train */
    float blit1 = BLIT(N, phase);
    float blit2 = BLIT(N, wrapTWOPI(w + phase));

    /* feed integrator */
    int1.add(blit1, incr);
    int2.add(blit2, incr);

    /* integrator delta */
    float delta = int1.value - int2.value;

    /* 3rd integrator */
    float beta = int3.add(delta, incr) * 6.f;

    /* compute RAMP waveform */
    ramp = int1.value - 0.1f;
    /* compute pulse waveform */
    pulse = delta * 1.6f;
    /* compute SAW waveform */
    saw = ramp * -1.f;
    /* compute triangle */
    tri = (float) M_PI / w * beta;
    /* compute sawtri */
    sawtri = saw + beta;

    /* adjust output levels */
    ramp *= 10;
    saw *= 10;
    pulse *=  5;
    sawtri = dcb.filter(sawtri) * 5;
    tri *= 6;
}


/**
 * @brief ReCompute basic parameter
 */
void BLITOscillator::invalidate() {
    incr = getPhaseIncrement(freq);
    N = (int) floorf(BLIT_HARMONICS / freq);
}


/**
 * @brief Get saturation
 * @return
 */
float BLITOscillator::getSaturate() const {
    return saturate;
}


/**
 * @brief Set saturation
 * @param saturate
 */
void BLITOscillator::setSaturate(float saturate) {
    BLITOscillator::saturate = saturate;

    /* force recalculation of variables */
    invalidate();
}


/**
 * @brief Translate from controlvoltage to frequency
 * @param cv Controlvoltage
 * @param oct Octave
 */
void BLITOscillator::setPitch(float cv, float oct) {
    setFrequency(261.626f * powf(2.f, cv / 12.f) * oct);
}