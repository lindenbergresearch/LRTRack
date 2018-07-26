
#include "WaveShaper.hpp"

using namespace dsp;


void WaveShaper::invalidate() {}


void WaveShaper::process() {
    float x = input[IN].value;
    float g = param[GAIN].value;
    float b = param[BIAS].value;
    float kpos = param[KPOS].value;
    float kneg = param[KNEG].value;

    rs.doUpsample(IN, x);

    for (int i = 0; i < rs.getFactor(); i++) {
        x = rs.getUpsampled(IN)[i] + b;

        /* float y = fastSin(x / g) * g;

         y = y / sqrt(s + y * y);


         y = fastatan(y);

         y = y * g;*/

        // for (int j = 0; j < 5; j++) {
        if (x >= 0)
            x = 1 / atan(kpos) * atan(kpos * x);
        else
            x = 1 / atan(kneg) * atan(kneg * x);

        //     if (j % 2 == 0) x *= -1;
        //   }

        rs.data[IN][i] = x * g;
    }


    output[OUT].value = rs.getDownsampled(IN);
}


void LockhartWavefolder::init() {
    DSPEffect::init();
}


void LockhartWavefolder::invalidate() {
    DSPEffect::invalidate();
}


void LockhartWavefolder::process() {
    DSPEffect::process();
}
