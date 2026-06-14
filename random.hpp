#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <algorithm>

using namespace std;

class Random 
{
    private:
    mt19937 generator; //seed

    public:
    Random(unsigned int seed = 42); //constructor

    double uniform(double min = 0.0, double max = 1.0); //uniform distribution

    double normal(); //gaussian distribution

    static double abramowitz_stegun(double x); //approximation of repartition function of N(0,1)
};

#endif