#include "../include/random.hpp"
#include "../include/utils.hpp"
#include "../include/eur_put.hpp"

#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

// MC classic 

vector<double> MC_DO_price(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    int N_delta = floor(T / delta);  //number of time steps

    vector<double> payoff(N);
    for (int i = 0; i<N; i++)
    {
        double S_current = S0;   //tracks underlying at each time step
        bool barrier_crossed = false;  
        for (int t = 1; t<=N_delta; t++)
        {
            double G_t = rand.normal();
            
            S_current = S_current * exp((r - 0.5 * sigma * sigma) * delta + sigma * sqrt(delta) * G_t);

            if (S_current < B)  //check at each time if barrier crossed --> necessarily the min would be < B if the bool became true
            {
                barrier_crossed = true;
            }
        }
        if (!barrier_crossed)  //if barrier never crossed
        {
            payoff[i] = exp(-r * T) * max<double>(0.0, K - S_current);  //at the end S_T = S_current
        }
        else
        {
            payoff[i] = 0.0;
        }
    }
    double price;
    double STD;
    if (N_delta == 0)
    {
        price = MC_price(rand, N, r, sigma, S0, K, T)[0];
        STD = MC_price(rand, N, r, sigma, S0, K, T)[1];
    }
    else
    {
        price = accumulate(payoff.begin(), payoff.end(), 0.0) / N;
        STD = calculate_STD(payoff);
    }
    double err = 1.645 * STD / sqrt(N); //for 90% confidence interval
    return {price, STD, err};
}


// MC var red antithetic --> not the best for put DO

vector<double> MC_DO_price_antithetic(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    int N_delta = floor(T / delta);  //number of time steps

    vector<double> payoff(N/2);
    vector<double> payoff_anti(N/2);
    vector<double> payoff_tot(N/2);
    for (int i = 0; i<N/2; i++)
    {
        double S_current = S0;   //tracks underlying at each time step
        double S_current_anti = S0;
        bool barrier_crossed = false;  //maybe need to have a bool for each S_current and S_current_anti
        bool barrier_crossed_anti = false;
        for (int t = 1; t<=N_delta; t++)
        {
            double G_t = rand.normal();
            
            S_current = S_current * exp((r - 0.5 * sigma * sigma) * delta + sigma * sqrt(delta) * G_t);
            S_current_anti = S_current_anti * exp((r - 0.5 * sigma * sigma) * delta + sigma * sqrt(delta) * (-G_t));

            if (S_current < B)  //check at each time if barrier crossed --> necessarily the min would be < B if the bool became true
            {
                barrier_crossed = true;
            }
            if (S_current_anti < B)
            {
                barrier_crossed_anti = true;
            }
        }
        if (!barrier_crossed)  //if barrier never crossed
        {
            payoff[i] = exp(-r * T) * max<double>(0.0, K - S_current);
        }
        else
        {
            payoff[i] = 0.0;
        }
        if (!barrier_crossed_anti)
        {
            payoff_anti[i] = exp(-r * T) * max<double>(0.0, K - S_current_anti);
        }
        else
        {
            payoff_anti[i] = 0.0;
        }
        payoff_tot[i] = payoff[i] + payoff_anti[i];
    }
    double price = accumulate(payoff_tot.begin(), payoff_tot.end(), 0.0) / N;
    double STD = calculate_STD(payoff);
    double err = 1.645 * STD / sqrt(N); //for 90% confidence interval
    return {price, STD, err};
}


// continous time pricing of put DO --> Brownian bridge
// now if S above the barrier at i * delta and (i+1) * delta, it can get below between this 2 times


vector<double> MC_DO_price_upgrade(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    int N_delta = floor(T / delta);  //number of time steps

    vector<double> payoff(N);
    for (int i = 0; i<N; i++)
    {
        double S_current = S0;   //tracks underlying at each time step
        bool barrier_crossed = false;
        double prob_survival = 1.0; //global probability of not crossing the barrier after all the N_delta intervals
        for (int t = 1; t<=N_delta; t++)
        {
            double G_t = rand.normal();
            
            double S_past = S_current;  //needed for p_t
            S_current = S_current * exp((r - 0.5 * sigma * sigma) * delta + sigma * sqrt(delta) * G_t);
            
            if (S_current < B)  //check at each time if barrier crossed --> necessarily the min would be < B if the bool became true
            {
                barrier_crossed = true;
                break;
            }
            else 
            {
                double p_t = exp( - 0.5 * log(S_past / B) * log(S_current / B) / (sigma * sigma * delta)); //probability of crossing the barrier even if not crossed at (t-1) * delta and t * delta
                prob_survival = prob_survival * (1 - p_t);
            }

        }
        if (!barrier_crossed)  //if barrier never crossed
        {
            payoff[i] = exp(-r * T) * max<double>(0.0, K - S_current) * prob_survival;  //at the end S_T = S_current
        }
        else
        {
            payoff[i] = 0.0;
        }
    }
    double price;
    double STD;
    if (N_delta == 0)
    {
        price = MC_price(rand, N, r, sigma, S0, K, T)[0];
        STD = MC_price(rand, N, r, sigma, S0, K, T)[1];
    }
    else
    {
        price = accumulate(payoff.begin(), payoff.end(), 0.0) / N;
        STD = calculate_STD(payoff);
    }
    double err = 1.645 * STD / sqrt(N); //for 90% confidence interval
    return {price, STD, err};
}