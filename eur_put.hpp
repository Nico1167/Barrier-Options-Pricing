#ifndef EUR_PUT_HPP
#define EUR_PUT_HPP

#include "random.hpp"
#include <vector>

double BS_price(double r, double sigma, double S0, double K, double T);

vector<double> MC_price(Random rand, int N, double r, double sigma, double S0, double K, double T);

#endif