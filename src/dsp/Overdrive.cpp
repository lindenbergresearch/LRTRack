#include "Overdrive.hpp"

using namespace dsp;


Overdrive::Overdrive(float sr) : WaveShaper(sr) {
    init();
    noise = new Noise;
    hqclip = new HQClip(sr, 4);
}


void Overdrive::init() {
    WaveShaper::rs = new Resampler<1>(1);
}


void Overdrive::process() {
    WaveShaper::process();
}


void Overdrive::invalidate() {}


double Overdrive::compute(double x) {
    double out;
    double in = clampd(x, -SHAPER_MAX_VOLTS, SHAPER_MAX_VOLTS);

    in *= clampd(gain, 0., 20.); // add gain
    in += clampd(bias * 2, -SHAPER_MAX_BIAS, SHAPER_MAX_BIAS); // add bias

    in *= OVERDRIVE_GAIN;

    in = hqclip->next(in);

    in *= 1 / OVERDRIVE_GAIN * 0.3;
    if (blockDC) in = dc->filter(in);

    out = in + noise->nextFloat(OVERDRIVE_NOISE);

    return out;
}
