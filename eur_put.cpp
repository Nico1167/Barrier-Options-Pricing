#include "../include/random.hpp"
#include "../include/utils.hpp"

#include <cmath>
#include <numeric>
#include <vector>

double BS_price(double r, double sigma, double S0, double K, double T)
{
    double d1 = (log(S0/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    double price = K * exp(- r * T) * Random::abramowitz_stegun(-d2) - S0 * Random::abramowitz_stegun(-d1);

    return price;
}

vector<double> MC_price(Random rand, int N, double r, double sigma, double S0, double K, double T)
{
    vector<double> payoff(N);
    for (int i = 0; i<N; i++)
    {
        double G = rand.normal(); //generate double with distribution N(0,1)
        double W_T = sqrt(T) * G;   //to have brownian at maturity
        double S_T = S0 * exp((r - 0.5 * sigma * sigma) * T + sigma * W_T); //underlying

        payoff[i] = exp(-r * T) * max<double>(0.0, K - S_T);
    }
    double price = accumulate(payoff.begin(), payoff.end(), 0.0) / N; // 0.0 means we're working with doubles
    double STD = calculate_STD(payoff);
    double err = 1.645 * STD / sqrt(N); //for 90% confidence interval
    return {price, STD, err};
}
