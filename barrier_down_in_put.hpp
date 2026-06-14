#ifndef BARRIER_DOWN_IN_PUT_HPP
#define BARRIER_DOWN_IN_PUT_HPP

#include "random.hpp"
#include <vector>

vector<double> MC_DI_price_control(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta);

#endif