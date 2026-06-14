#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

inline double calculate_STD(const vector<double>& v)  //unbiased standard deviation
{
    int N = v.size();
    double mean = accumulate(v.begin(), v.end(), 0.0) / N;
    double sum_square = 0.0;

    for (double x : v)
    {
        sum_square += (x - mean) * (x - mean);
    }
    double variance = sum_square / (N - 1); //N-1 to have the unbiased estimator
    return sqrt(variance);
}


#endif