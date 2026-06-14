#include "../include/random.hpp"
#include "../include/eur_put.hpp"
#include "../include/barrier_down_out_put.hpp"
#include "../include/barrier_down_in_put.hpp"
#include "../include/export_to_plot.hpp"

#include <cmath>
#include <iostream>

using namespace std;

int main()
{
    //parameters
    double r = 0.015;
    double sigma = 0.15;
    double S0 = 1;
    double K = 1;
    double T = 2;

    //barrier parameters
    double delta = 1.0/52;
    double B = 0.7;

    int N = 10000;
    Random rand;

    int N_max = N;
    int N_min = 1000;
    int pas_N = 100;

    double B_min = 0.5;
    double B_max = 1.0;
    double n_B = 100; //number of B values

    double sigma_min = 0.0;
    double sigma_max = 0.8;
    double n_sigma = 100; //number of sigma values
    vector<double> S0_val = {0.8 , 1};

    vector <double> delta_val = {1.0/250, 1.0/52, 1.0/12, 1.0, 3.0};

    //double put_price_BS = BS_price(r, sigma, S0, K, T);

    //put_price_against_N_with_CI("data_put_price_against_N_with_CI", put_price_BS, N_min, N_max, pas_N, r, sigma, S0, K, T);
    
    //put_DO_price_against_N_with_CI("data_DO_price_against_N_with_CI", N_min, N_max, pas_N, r, sigma, S0, K, T, B, delta);
    
    //put_DO_comparison_antithetic("data_DO_comparison_antithetic", N_min, N_max, pas_N, r, sigma, S0, K, T, B, delta);
    
    //DO_sensibility_B("data_DO_sensibility_B", N, B_min, B_max, n_B, r, sigma, S0, K, T, delta);

    //DO_sensibility_sigma("data_DO_sensibility_sigma", N, sigma_min, sigma_max, n_sigma, r, S0_val, K, T, B, delta);

    //put_DO_comparison_delta("data_DO_price_comparison_delta", N, r, sigma, S0, K, T, B, delta_val);

    //DO_STD_against_B("data_DO_STD_against_B", N, B_min, B_max, n_B, r, sigma, S0, K, T, delta);

    //double put_price_MC = MC_price(rand, N, r, sigma, S0, K, T)[0];
    //double err_put = MC_price(rand, N, r, sigma, S0, K, T)[2];
    
    //double put_DO_price_MC = MC_DO_price(rand, N, r, sigma, S0, K, T, B, delta)[0];
    //double err_DO = MC_DO_price(rand, N, r, sigma, S0, K, T, B, delta)[2];
    
    //double put_DO_price_MC_anti = MC_DO_price_antithetic(rand, N, r, sigma, S0, K, T, B, delta)[0];
    //double err_DO_anti = MC_DO_price_antithetic(rand, N, r, sigma, S0, K, T, B, delta)[2];
    
    double put_DO_price_MC_upgrade = MC_DO_price_upgrade(rand, N, r, sigma, S0, K, T, B, delta)[0];
    double err_DO_upgrade = MC_DO_price_upgrade(rand, N, r, sigma, S0, K, T, B, delta)[2];

    double put_DI_price_MC = MC_DI_price_control(rand, N, r, sigma, S0, K, T, B, delta)[0];
    double err_DI = MC_DI_price_control(rand, N, r, sigma, S0, K, T, B, delta)[2];
    
    //cout << put_price_BS <<endl;
    
    //cout << put_price_MC <<endl;
    //cout << err_put <<endl;
   
    //cout << put_DO_price_MC <<endl;
    //cout << err_DO <<endl;
    
    //cout << put_DO_price_MC_anti <<endl;
    //cout << err_DO_anti <<endl;

    cout << put_DO_price_MC_upgrade <<endl;
    cout << err_DO_upgrade <<endl;

    cout << put_DI_price_MC <<endl;
    cout << err_DI <<endl;

    return 0;
}
