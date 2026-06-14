#ifndef BARRIER_DOWN_OUT_PUT_HPP
#define BARRIER_DOWN_OUT_PUT_HPP

#include "random.hpp"
#include <vector>

vector<double> MC_DO_price(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta);

vector<double> MC_DO_price_antithetic(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta);

vector<double> MC_DO_price_upgrade(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta);

#endif