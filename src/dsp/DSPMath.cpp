#include "DSPMath.hpp"
#include "engine.hpp"


/**
 * @brief Fast sin approximation
 * @param angle Angle
 * @return App. value
 */
float fastSin(float angle) {
    float sqr = angle * angle;
    float result = -2.39e-08f;
    result *= sqr;
    result += 2.7526e-06f;
    result *= sqr;
    result -= 1.98409e-04f;
    result *= sqr;
    result += 8.3333315e-03f;
    result *= sqr;
    result -= 1.666666664e-01f;
    result *= sqr;
    result += 1.0f;
    result *= angle;
    return result;
    // return sinf(angle);
}


/**
 * @brief Quadratic sin approximation low precicion
 * @param x
 * @return
 */
float qsinlp(float x) {
    if (x < -3.14159265f)
        x += 6.28318531f;
    else if (x > 3.14159265f)
        x -= 6.28318531f;

    if (x < 0)
        return x * (1.27323954f + 0.405284735f * x);
    else
        return x * (1.27323954f - 0.405284735f * x);
}


/**
 * @brief Quadratic sin approximation high precicion
 * @param x
 * @return
 */
float qsinhp(float x) {
    float sin;

    x = wrapTWOPI(x);

    if (x < -3.14159265f)
        x += 6.28318531f;
    else if (x > 3.14159265f)
        x -= 6.28318531f;

    if (x < 0) {
        sin = x * (1.27323954f + 0.405284735f * x);

        if (sin < 0)
            sin = sin * (-0.255f * (sin + 1) + 1);
        else
            sin = sin * (0.255f * (sin - 1) + 1);
    } else {
        sin = x * (1.27323954f - 0.405284735f * x);

        if (sin < 0)
            sin = sin * (-0.255f * (sin + 1) + 1);
        else
            sin = sin * (0.255f * (sin - 1) + 1);
    }

    return sin;
}


/**
 * @brief Fast sin approximation with input wrapping of -PI..PI
 * @param angle Angle
 * @return App. value
 */
float fastSinWrap(float angle) {
    return fastSin(angle);
}


/**
 * @brief Clip signal at bottom by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float clipl(float in, float clip) {
    if (in < clip) return clip;
    else return in;
}


/**
 * @brief Clip signal at top by value
 * @param in Sample input
 * @param clip Clipping value
 * @return Clipped sample
 */
float cliph(float in, float clip) {
    if (in > clip) return clip;
    else return in;
}


/**
 * @brief Wrap input number between -PI..PI
 * @param n Input number
 * @return Wrapped value
 */
float wrapTWOPI(float n) {
    float b = 1.f / TWOPI * n;
    return (b - lround(b)) * TWOPI;
}


/**
 * @brief Get PLL increment depending on frequency
 * @param frq Frequency
 * @return  PLL increment
 */
float getPhaseIncrement(float frq) {
    return TWOPI * frq / engineGetSampleRate();
}


/**
 * @brief Actual BLIT core computation
 * @param N Harmonics
 * @param phase Current phase value
 * @return
 */
float BLITcore(float N, float phase) {
    float a = wrapTWOPI((clipl(N - 1, 0.f) + 0.5f) * phase);
    float x = sinf(a) * 1.f / sinf(0.5f * phase);
    return (x - 1.f) * 2;
}


/**
 * @brief BLIT generator based on current phase
 * @param N Harmonics
 * @param phase Current phase of PLL
 * @return
 */
float BLIT(float N, float phase) {
    if (phase == 0.f) return 1.f;
    else return BLITcore(N, phase);
}


/**
 * @brief Add value to integrator
 * @param x Input
 * @param Fn
 * @return
 */
float Integrator::add(float x, float Fn) {
    value = (x - value) * (d * Fn) + value;
    return value;
}


/**
 * @brief Filter function for DC block
 * @param x Input sample
 * @return Filtered sample
 */
float DCBlocker::filter(float x) {
    float y = x - xm1 + R * ym1;
    xm1 = x;
    ym1 = y;

    return y;
}


/**
 * @brief Filter function for simple 6dB lowpass filter
 * @param x Input sample
 * @return
 */
double LP6DBFilter::filter(double x) {
    double y = y0 + (alpha * (x - y0));
    y0 = y;

    return y;
}


/**
 * @brief Update filter parameter
 * @param fc Cutoff frequency
 */
void LP6DBFilter::updateFrequency(sfloat fc, int factor) {
    this->fc = fc;
    RC = 1.f / (this->fc * TWOPI);
    dt = 1.f / engineGetSampleRate() * factor;
    alpha = dt / (RC + dt);
}


/**
 * @brief Init randomizer
 */
Randomizer::Randomizer() {}


/**
 * @brief Return next random float in the range of 0.0 - 1.0
 * @return Random number
 */
float Randomizer::nextFloat(float start, float stop) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(start, stop); // rage 0 - 1
    return (float) dis(e);
}


/**
 * @brief Shaper type 1 (Saturate)
 * @param a Amount from 0 - x
 * @param x Input sample
 * @return
 */
float shape1(float a, float x) {
    float k = 2 * a / (1 - a);
    float b = (1 + k) * (x * 0.5f) / (1 + k * abs(x * 0.5f));

    return b * 4;
}


/**
     * @brief Return linear interpolated position
     * @param point Point in oversampled data
     * @return
     */
sfloat Oversampler::interpolate(int point) {
    return y0 + (point / factor) * (y1 - y0);
}


/**
 * @brief Create up-sampled data out of two basic values
 */
void Oversampler::upsample() {
    for (int i = 0; i < factor; i++) {
        up[i] = filter.filter(interpolate(i + 1));
    }
}


/**
 * @brief Step to next sample point
 * @param y Next sample point
 */
void Oversampler::next(sfloat y) {
    y0 = y1;
    y1 = y;
}


/**
 * @brief Compute data for oversampled points
 * @param transform Pointer to transform function
 */
void Oversampler::compute(sfloat (*transform)(sfloat)) {
    for (int i = 0; i < factor; i++) {
        data[i] = transform(up[i]);
    }
}


/**
 * @brief Downsample data per average
 * @return Downsampled point
 */
sfloat Oversampler::downsample() {
    sfloat tmp = 0;

    for (int i = 0; i < factor; i++) {
        tmp += data[i];
    }

    return tmp / factor;
}


/**
 * @brief Soft saturating with a clip of a. Works only with positive values so use 'b' as helper here.
 * @param x Input sample
 * @param a Saturating threshold
 * @return
 */
double saturate(double x, double a) {
    double b = 1;

    /* turn negative values positive and remind in b as coefficient */
    if (x < 0) {
        b = -1;
        x *= -1;
    }

    // nothing to do
    if (x <= a) return x * b;

    double d = (a + (x - a) / (1 + pow((x - a) / (1 - a), 2)));

    if (d > 1) {
        return (a + 1) / 2 * b;
    } else {
        return d * b;
    }
}


/**
 * @brief
 * @param input
 * @return
 */
double overdrive(double input) {
    const double x = input * 0.686306;
    const double a = 1 + exp(sqrt(fabs(x)) * -0.75);
    return (exp(x) - exp(-x * a)) / (exp(x) + exp(-x));
}