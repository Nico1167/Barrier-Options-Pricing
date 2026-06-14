# Pricing of Barrier Options & Variance Reduction Methods

## Overview
This repository contains a C++ pricing engine for exotic options (Barrier Options) within the Black-Scholes framework. The project focuses on implementing and analyzing various Monte Carlo simulation techniques and variance reduction methods to efficiently price these derivatives.

Beyond simply pricing the assets, this project serves as a quantitative study on the impact of discrete vs. continuous barrier monitoring and the mathematical limits of classical variance reduction methods (Antithetic Variates, Control Variates, and Brownian Bridge).

## Financial Products Implemented
The pricing engine covers the following instruments under the risk-neutral measure:
* **European Put Option:** Used as a baseline and theoretical benchmark.
* **Down & Out Put:** A barrier option that becomes worthless (knocked-out) if the underlying asset price falls below a predefined barrier $B$ prior to maturity $T$.
* **Down & In Put:** A barrier option that only becomes active (knocked-in) if the underlying asset price falls below $B$ prior to maturity $T$.

## Quantitative Analysis & Variance Reduction

### 1. The Discretization Bias & The Brownian Bridge
Standard Monte Carlo simulations evaluate the barrier only at discrete time steps (e.g., weekly). This introduces a significant upward bias in the Down & Out price, as the simulation misses intra-step barrier breaches.

To price a strictly continuous barrier without requiring an infinite number of time steps, a **Brownian Bridge** method is implemented.
For each discrete interval $[T^i, T^{i+1}]$, the exact probability of the underlying asset hitting the barrier *between* the observation dates is calculated using the properties of the Brownian Bridge:

$$
p_i = \exp\left( -2 \frac{\ln(S(T^i)/B) \cdot \ln(S(T^{i+1})/B)}{\sigma^2 \Delta} \right)
$$

* **Result:** This conditional Monte Carlo approach completely removes the discretization bias and significantly reduces the variance, yielding the exact continuous price even with large time steps $\Delta$.

*(Insert image of Price vs. Delta showing the classic MC bias vs the flat Brownian Bridge line here)*
`![Price vs Delta](path/to/your/delta_graph.png)`

### 2. The Failure of Antithetic Variates on Barrier Options
Antithetic variates are implemented to reduce variance by generating negatively correlated path pairs ($+Z$ and $-Z$).
* **Finding:** While highly effective for vanilla European options, this method proves largely ineffective for Down & Out options. When volatility is high, one path often knocks out (payoff = 0), and the mirrored path finishes out-of-the-money (payoff = 0). The resulting positive correlation (0 and 0) destroys the variance reduction mechanism.

### 3. Control Variates & In-Out Parity
To achieve extreme precision on the Down & Out Put, a **Control Variate** method is implemented using the closed-form Black-Scholes European Put price. This relies on the strict In-Out Parity:

$$
P^{euro} = P^{DO} + P^{DI}
$$

The controlled estimator is computed inline:

$$
\bar{P}^{DO}_{CV} = \bar{P}^{DO}_{MC} - \beta^* \left( \bar{P}^{euro}_{MC} - P^{euro}_{BS} \right)
$$

Where $\beta^*$ is the optimal correlation coefficient calculated empirically during the simulation.

* **Finding:** The efficiency of this method is heavily dependent on the barrier level $B$.
    * When $B$ is low, the DO Put behaves similarly to the European Put ($\rho \to 1$), resulting in massive variance reduction.
    * When $B$ is closer to the strike $K$, the correlation breaks down due to frequent knock-outs, and the standard deviation (STD) forms a distinct bell-curve peak, highlighting the limitations of the control variate.

*(Insert image of the STD vs Barrier Level showing the bell curve here)*
`![STD vs Barrier](path/to/your/std_barrier_graph.png)`

## Results & Visualizations

### Vega Sensitivity (Price vs. Volatility)
Unlike vanilla options which are strictly Long Vega, the Down & Out Put exhibits a distinct asymmetric bell-curve relationship with volatility. As $\sigma$ grows excessively large, the probability of hitting the knock-out barrier approaches 100%, causing the option's value to collapse toward zero.
`![Price vs Volatility](path/to/your/volatility_graph.png)`

## Code Architecture
The project is built in C++ for computational efficiency, with Python used for data visualization.
* `main.cpp`: Entry point executing the simulations and outputting results.
* `EuropeanPut.cpp` / `DownAndOut.cpp` / `DownAndIn.cpp`: Product-specific pricing engines implementing the Monte Carlo loops and payoff logic.
* `random.cpp`: Uniform and Normal random number generators.
* `export_csv.hpp`: Utility to export simulation results (prices, variances, confidence intervals) for data analysis.
* `plot.py`: Python script utilizing `matplotlib` to generate the quantitative visualizations.

## Compilation & Execution
```bash
# Example
g++ -O3 main.cpp EuropeanPut.cpp DownAndOut.cpp random.cpp -o pricer
./pricer
python plot.py
