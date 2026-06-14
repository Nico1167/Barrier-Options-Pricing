#ifndef EXPORT_TO_PLOT_HPP
#define EXPORT_TO_PLOT_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "random.hpp"
#include "eur_put.hpp"
#include "barrier_down_out_put.hpp"
#include "barrier_down_in_put.hpp"


using namespace std;

// ---------------------------------------
// European put MC convergence
// ---------------------------------------

inline void put_price_against_N_with_CI(const string& file_name, double BS_price, int N_min, int N_max, int pas, double r, double sigma, double S0, double K, double T)
{
    ofstream file(file_name);  //open file 

    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Sample size" << "," << "BS price" << "," << "Price"<< "," << "Lower bound" << "," << "Upper bound" << "\n"; //header
    for (int i = N_min; i<=N_max; i++)
    {
        if (i%pas == 0)
        {
            Random rand_local; //uses the same beginning values and add new at the end at each iteration --> to have a smoother CV
            double price = MC_price(rand_local, i, r, sigma, S0, K, T)[0];
            double err = MC_price(rand_local, i, r, sigma, S0, K, T)[2];
            double CI_low = price - err;
            double CI_up = price + err;
            file << i << "," << BS_price << "," << price << "," << CI_low << "," << CI_up <<"\n"; 
        }
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


// --------------------------------------------------
// Put Down & In MC convergence
// --------------------------------------------------

//MC classic

inline void put_DO_price_against_N_with_CI(const string& file_name, int N_min, int N_max, int pas, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    ofstream file(file_name);  //open file 

    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Sample size" << "," << "Price"<< "," << "Lower bound" << "," << "Upper bound" << "\n"; //header
    for (int i = N_min; i<=N_max; i++)
    {
        if (i%pas == 0)
        {
            Random rand_local; //uses the same beginning values and add new at the end at each iteration --> to have a smoother CV
            double price = MC_DO_price(rand_local, i, r, sigma, S0, K, T, B, delta)[0];
            double err = MC_DO_price(rand_local, i, r, sigma, S0, K, T, B, delta)[2];
            double CI_low = price - err;
            double CI_up = price + err;
            file << i << "," << price << "," << CI_low << "," << CI_up <<"\n"; 
        }
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


//MC with antithetic var reduction

inline void put_DO_comparison_antithetic(const string& file_name, int N_min, int N_max, int pas, double r, double sigma, double S0, double K, double T, double B, double delta)
{
    ofstream file(file_name);  //open file 

    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Sample size" << "," << "Price MC"<< "," << "Lower bound" << "," << "Upper bound" << "," << "Price MC antithetic"<< "," << "Lower bound antithetic" << "," << "Upper bound antithetic" << "\n"; //header
    for (int i = N_min; i<=N_max; i++)
    {
        if (i%pas == 0)
        {
            Random rand_local; //uses the same beginning values and add new at the end at each iteration --> to have a smoother CV
            double price = MC_DO_price(rand_local, i, r, sigma, S0, K, T, B, delta)[0];
            double err = MC_DO_price(rand_local, i, r, sigma, S0, K, T, B, delta)[2];
            double CI_low = price - err;
            double CI_up = price + err;

            //with var red
            double price_anti = MC_DO_price_antithetic(rand_local, i, r, sigma, S0, K, T, B, delta)[0];
            double err_anti = MC_DO_price_antithetic(rand_local, i, r, sigma, S0, K, T, B, delta)[2];
            double CI_low_anti = price_anti - err_anti;
            double CI_up_anti = price_anti + err_anti;
            file << i << "," << price << "," << CI_low << "," << CI_up << "," << price_anti << "," << CI_low_anti << "," << CI_up_anti <<"\n"; 
        }
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


// -------------------------------------------------
// Parameters sensibilities
// -------------------------------------------------

// Against barrier value 0.5 < B < 1

inline void DO_sensibility_B(const string& file_name, int N, double B_min, double B_max, int n_B, double r, double sigma, double S0, double K, double T, double delta)
{
    double pas = (B_max - B_min) / n_B;
    Random rand; //always the same simulation but with different B only

    ofstream file(file_name);  //open file 
    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Barrier value" << "," << "Price"<<"\n"; //header
    for (int i = 0; i<n_B; i++)
    {
        double B_i = B_min + i * pas;
        double price = MC_DO_price(rand, N, r, sigma, S0, K, T, B_i, delta)[0];
        file << B_i << "," << price <<"\n"; 
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


// Against volatility value 0 < sigma < 0.8, for two values of S0

inline void DO_sensibility_sigma(const string& file_name, int N, double sigma_min, double sigma_max, int n_sigma, double r, vector<double> S0, double K, double T, double B, double delta)
{
    double pas = (sigma_max - sigma_min) / n_sigma;
    Random rand; //always the same simulation but with different B only

    ofstream file(file_name);  //open file 
    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Volatility" << "," << "Price 0"<< "," << "Price 1"<< "\n"; //header
    for (int i = 0; i<n_sigma; i++)
    {
        double sigma_i = sigma_min + i * pas;
        double price_0 = MC_DO_price(rand, N, r, sigma_i, S0[0], K, T, B, delta)[0];
        double price_1 = MC_DO_price(rand, N, r, sigma_i, S0[1], K, T, B, delta)[0];
        file << sigma_i << "," << price_0 << "," << price_1 << "\n"; 
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


// ----------------------------------------------------------------------------------------
// Comparison of discrete-time approx and Brownian bridge, against delta
// ----------------------------------------------------------------------------------------

inline void put_DO_comparison_delta(const string& file_name, int N, double r, double sigma, double S0, double K, double T, double B, vector<double> delta_val)
{
    ofstream file(file_name);  //open file 

    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Time step" << "," << "Price MC delta"<< "," << "Price MC bridge" << "\n"; //header
    for (double delta : delta_val)
    {
        Random rand_local; //uses the same beginning values and add new at the end at each iteration --> to have a smoother CV
        double price_delta = MC_DO_price(rand_local, N, r, sigma, S0, K, T, B, delta)[0];
        double price = MC_DO_price_upgrade(rand_local, N, r, sigma, S0, K, T, B, delta)[0];
            
        file << delta << "," << price_delta << "," << price <<"\n"; 
    }
    file.close();
    cout << "File successfully exported !" << endl;
}


// -------------------------------------------------------------------------------
// Verifying accuracy of controle variate var red method, against B
// ------------------------------------------------------------------------------

inline void DO_STD_against_B(const string& file_name, int N, double B_min, double B_max, int n_B, double r, double sigma, double S0, double K, double T, double delta)
{
    double pas = (B_max - B_min) / n_B;
    Random rand; //always the same simulation but with different B only

    ofstream file(file_name);  //open file 
    // error check on file opening
    if (!file.is_open())
    {
        cerr << "Error : impossible to open file" << endl;
        return;
    }
    file << "Barrier value" << "," << "STD DO"<<"\n"; //header
    for (int i = 0; i<n_B; i++)
    {
        double B_i = B_min + i * pas;
        double STD = MC_DI_price_control(rand, N, r, sigma, S0, K, T, B_i, delta)[1];
        file << B_i << "," << STD <<"\n"; 
    }
    file.close();
    cout << "File successfully exported !" << endl;
}
#endif