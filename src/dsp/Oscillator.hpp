#pragma once

using namespace rack;

/**
 * @brief Band-limited Oscillator class
 */
struct BLITOscillator : Oscillator {
    void proccess() override;
    void invalidate() override;
};


/**
 * @brief Oscillator base class
 */
struct Oscillator {
    float freq = 440.f; // oscillator frequency
    float pw = 0.5f;    // pulse-width value
    float phase = 0.f;  // current phase
    float incr = 0.f;   // current phase increment for PLL

    /* currents of waveforms */
    float ramp = 0.f;
    float saw = 0.f;
    float pulse = 0.f;
    float sawtri = 0.f;
    float tri = 0.f;

    Oscillator();
    Oscillator(float freq, float pw);
    virtual ~Oscillator();

    /**
     * @brief Proccess next sample for output
     */
    virtual void proccess();


    /**
     * @brief ReCompute states on change
     */
    virtual void invalidate();


    void reset();


    /**
     * @brief Change PulseWidth of oscillator
     * @param pw
     */
    void setPW(float pw) {
        this->pw = pw;
        invalidate();
    }


    /**
     * @brief Change frequency of oscillator
     * @param frq
     */
    void setFrequency(float freq) {
        this->freq = freq;
        invalidate();
    }


    /**
     * @brief Get current value of ramp waveform
     * @return
     */
    float getRamp() {
        return ramp;
    }


    /**
     * @brief Get current value of saw waveform
     * @return
     */
    float getSaw() {
        return saw;
    }


    /**
     * @brief Get current value of pulse waveform
     * @return
     */
    float getPulse() {
        return pulse;
    }


    /**
     * @brief Get current value of triangle-saw waveform
     * @return
     */
    float getSawTri() {
        return sawtri;
    }


    /**
     * @brief Get current value of triangle waveform
     * @return
     */
    float getTri() {
        return tri;
    }

};