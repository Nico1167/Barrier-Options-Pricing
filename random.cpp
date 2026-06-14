#include "../include/random.hpp"

#include <cmath>
#include <random>

using namespace std;

Random::Random(unsigned int seed):generator(seed){};

double Random::uniform(double min, double max) 
{
    uniform_real_distribution<double> dist(min, max);
    return dist(generator);
}

double Random::normal()
{
    double U1 = uniform();
    while (U1 < 1e-10)  //otherwise possible log(0)
    {
        U1 = uniform();
    }
    double U2 = uniform();
    return sqrt(-2 * log(U1)) * cos(2.0 * M_PI * U2);
}

double Random::abramowitz_stegun(double x)
{
    double b0 = 0.2316419;
    double b1 = 0.319381530;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;

    double t = 1 / (1 + b0 * x);
    return 1 - (1 / sqrt(2 * M_PI)) * exp(-0.5 * x * x) * (b1*t + b2*t*t + b3*t*t*t + b4*t*t*t*t + b5*t*t*t*t*t);
}
