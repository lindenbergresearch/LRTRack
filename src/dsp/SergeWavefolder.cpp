#include "SergeWavefolder.hpp"

using namespace dsp;


double SergeWFStage::compute(double x) {
    double out;
    double l, u, ln, fn, xn;

    l = sign(x);
    u = SERGE_R1 * SERGE_IS / SERGE_VT * pow(M_E, l * x / SERGE_VT);
    ln = lambert_W_Fritsch(u);
    fn = x * x / 2 - SERGE_VT * SERGE_VT * ln * (ln + 2);

    // Check for ill-conditioning
    if (abs(x - xn1) < SERGE_THRESHOLD) {
        // Compute Averaged Wavefolder Output
        xn = 0.5 * (x + xn1);
        u = SERGE_R1 * SERGE_IS / SERGE_VT * pow(M_E, l * xn / SERGE_VT);
        ln = lambert_W_Fritsch(u);
        out = xn - 2 * l * SERGE_VT * ln;
    } else {
        // Apply AA Form
        out = (fn - fn1) / (x - xn1);
    }

    fn1 = fn;
    xn1 = x;

    return out;
}


SergeWFStage::SergeWFStage() {
    fn1 = 0;
    xn1 = 0;
}

