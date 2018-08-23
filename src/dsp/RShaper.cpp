#include "RShaper.hpp"

using namespace dsp;


ReShaper::ReShaper(float sr) : WaveShaper(sr) {
    init();
    noise = new Noise;
}


void ReShaper::init() {
    WaveShaper::rs = new Resampler<1>(1);
}


void ReShaper::process() {
    WaveShaper::process();
}


void ReShaper::invalidate() {}


double ReShaper::compute(double x) {
    double out;
    double in = clampd(x, -SHAPER_MAX_VOLTS, SHAPER_MAX_VOLTS);

    in *= clampd(gain, 0., 20.); // add gainBtn
    in += clampd(bias * 2, -SHAPER_MAX_BIAS, SHAPER_MAX_BIAS); // add biasBtn

    in *= RSHAPER_GAIN;

    in = tanh1->next(in);

    in *= 1 / RSHAPER_GAIN * 0.3;
    if (blockDC) in = dc->filter(in);

    out = in + noise->nextFloat(RSHAPER_NOISE);

    return out;
}
