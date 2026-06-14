#include "../include/random.hpp"
#include "../include/utils.hpp"
#include "../include/eur_put.hpp"
#include "../include/barrier_down_out_put.hpp"

#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>


// MC with control variates using identity P_DO + P_DI = P_eur
// P_eur doesn't introduce any var so Var_DI = Var_DO
// we use the correlation between put and put_DO to reduce Var_DO
// cov and var calculated in the same loop to reduce computational time --> no outside var/cov function

vector<double> MC_DI_price_control(Random rand, int N, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    int N_delta = floor(T / delta);  //number of time steps
    
    double put_BS_price = BS_price(r,sigma,S0,K,T);

    // parameters for cov(P_euro,P_DO), Var(P_euro) & Var(P_DO)
    double sum_put = 0.0;
    double sum_DO = 0.0;
    double sum_prod = 0.0;
    double sum_put_squared = 0.0;
    double sum_DO_squared = 0.0;

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
        double payoff_DO_i;
        if (!barrier_crossed)  //if barrier never crossed
        {
            payoff_DO_i = exp(-r * T) * max<double>(0.0, K - S_current) * prob_survival;  //at the end S_T = S_current
        }
        else
        {
            payoff_DO_i = 0.0;
        }
        double payoff_put_i = exp(-r * T) * max<double>(0.0, K - S_current);

        // for cov and var
        sum_put += payoff_put_i;
        sum_DO += payoff_DO_i;
        sum_prod += payoff_put_i * payoff_DO_i;
        sum_put_squared += payoff_put_i * payoff_put_i;
        sum_DO_squared += payoff_DO_i * payoff_DO_i;
    }

    // expectations
    double E_put = sum_put / N; // = price_put_MC
    double E_DO = sum_DO / N;   // = price_DO_MC
    double E_prod = sum_prod / N;
    double E_put_squared = sum_put_squared / N;
    double E_DO_squared = sum_DO_squared / N;

    // covariance and variance
    double covariance = E_prod - (E_put * E_DO);
    double variance_put = E_put_squared - (E_put * E_put);
    double variance_DO = E_DO_squared - (E_DO * E_DO);

    // var red optimal parameter
    double beta = covariance / variance_put;

    // corrected DO price and STD, with control variate
    double price_DO_control = E_DO - beta * (E_put - put_BS_price);
    double variance_DO_control = variance_DO + beta * beta * variance_put - 2 * beta * covariance;
    double STD = sqrt(variance_DO_control);

    // price and err for DI
    double price = put_BS_price - price_DO_control;
    double err = 1.645 * STD / sqrt(N); //for 90% confidence interval
    return {price, STD, err};
}