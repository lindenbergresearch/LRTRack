#pragma once


#include "WaveShaper.hpp"
#include "HQTrig.hpp"

#define SERGE_R1 33e3
#define SERGE_IS 10e-9
#define SERGE_VT 25.864

#define SERGE_THRESHOLD 10e-10

namespace dsp {

    struct SergeWFStage {
    private:

        double fn1, xn1;
        double a, b, d;

    public:

        SergeWFStage();

        double compute(double x);

    };


    class SergeWavefolder {

    };


}