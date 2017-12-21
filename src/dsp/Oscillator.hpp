#pragma once

#include "DSPMath.hpp"

#define BLIT_HARMONICS 18000.f
#define NOTE_C4 261.626f
#define OSC_SHAPING 0.778

using namespace rack;


/**
 * @brief Oscillator base class
 */
struct BLITOscillator {
    float freq;      // oscillator frequency
    float pw;        // pulse-width value
    float phase;     // current phase
    float incr;      // current phase increment for PLL
    float detune;    // analogue detune
    float drift;     // oscillator drift
    float warmup;    // oscillator warmup detune
    Randomizer rand; // randomizer
    bool boost;

    float saturate;
    int n;

    /* currents of waveforms */
    float ramp;
    float saw;
    float pulse;
    float sawtri;
    float tri;

    /* saved frequency states */
    float _cv, _oct, _base, _coeff, _tune, _biqufm;

    /* leaky integrators */
    Integrator int1;
    Integrator int2;
    Integrator int3;
    DCBlocker dcb1;
    DCBlocker dcb2;

    BLITOscillator();
    ~BLITOscillator();

    /**
     * @brief Proccess next sample for output
     */
    void proccess();


    /**
     * @brief ReCompute states on change
     */
    void invalidate();


    /**
     * @brief Reset oscillator
     */
    void reset();


    void updatePitch(float cv, float fm, float tune, float oct);

    /* common getter and setter */
    float getFrequency() const;
    void setFrequency(float freq);
    float getPulseWidth() const;
    void setPulseWidth(float pw);

    float getRampWave() const;
    float getSawWave() const;
    float getPulseWave() const;
    float getSawTriWave() const;
    float getTriangleWave() const;
    float getSaturate() const;
    void setSaturate(float saturate);
};