import pandas as pd
import matplotlib
matplotlib.use('WebAgg')
import matplotlib.pyplot as plt
from pathlib import Path
import numpy as np

color1 = "#980FB4"
color2 = "#C597F4"
color3 = "#0D7219"
color4 = "#B5E77C"
color5 = "#E00505"

# ----------------------------------
# European put MC convergence
# ----------------------------------

def plot_put_price_against_N_with_CI(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color3 = "#0D7219"
    color4 = "#B5E77C"
    color5 = "#E00505"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Sample size"], data["Price"], label="MC Put price", color=color3, linewidth=2)
    plt.plot(data["Sample size"], data["BS price"], label="BS Put price", color=color5, linewidth=2)

    plt.fill_between(data["Sample size"], data["Lower bound"], data["Upper bound"], color = color4, alpha=0.5, label='90% Confidence Interval')

    plt.xlabel("Monte Carlo sample size (N)")
    plt.ylabel("Put price (P)")
    plt.legend()

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_put_price_against_N_with_CI("data_put_price_against_N_with_CI","put_price_against_N_with_CI.png")

# ------------------------------------------
# Barrier Down & Out Put MC convergence
# ------------------------------------------

def plot_DO_price_against_N_with_CI(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color3 = "#0D7219"
    color4 = "#B5E77C"
    color5 = "#E00505"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Sample size"], data["Price"], label="MC Down & Out Put price", color=color3, linewidth=2)
    
    plt.fill_between(data["Sample size"], data["Lower bound"], data["Upper bound"], color = color4, alpha=0.5, label='90% Confidence Interval')

    plt.xlabel("Monte Carlo sample size (N)")
    plt.ylabel("Down & Out Put price (P)")
    plt.legend()

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_DO_price_against_N_with_CI("data_DO_price_against_N_with_CI","DO_price_against_N_with_CI.png")

#comparison between MC classic and with antithetic var red

def plot_DO_price_comparison_antithetic(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color1 = "#980FB4"
    color2 = "#C597F4"
    color3 = "#0D7219"
    color4 = "#B5E77C"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Sample size"], data["Price MC"], label="MC Down & Out Put price", color=color3, linewidth=2)
    plt.plot(data["Sample size"], data["Price MC antithetic"], label="MC antithetic Down & Out Put price", color=color1, linewidth=2)

    plt.fill_between(data["Sample size"], data["Lower bound"], data["Upper bound"], color = color4, alpha=0.5, label='90% Confidence Interval')
    plt.fill_between(data["Sample size"], data["Lower bound antithetic"], data["Upper bound antithetic"], color = color2, alpha=0.5, label='90% Confidence Interval antithetic')

    plt.xlabel("Monte Carlo sample size (N)")
    plt.ylabel("Down & Out Put price (P)")
    plt.legend()

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_DO_price_comparison_antithetic("data_DO_comparison_antithetic","DO_price_comparison_antithetic")


# ------------------------------------------------
# Parameters sensibilities
# ------------------------------------------------

def plot_DO_sensibility_B(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color5 = "#E00505"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Barrier value"], data["Price"], color=color5, linewidth=2)
   
    plt.xlabel("Barrier value (B)")
    plt.ylabel("Down & Out Put price (P)")

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_DO_sensibility_B("data_DO_sensibility_B", "DO_sensibility_B.png")


def plot_DO_sensibility_sigma(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color2 = "#C597F4"
    color3 = "#0D7219"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Volatility"], data["Price 0"], label="S0 = 0.8",color=color2, linewidth=2)
    plt.plot(data["Volatility"], data["Price 1"], label="S0 = 1.0",color=color3, linewidth=2)

    plt.xlabel("Volatility (sigma)")
    plt.ylabel("Down & Out Put price (P)")
    plt.legend()

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_DO_sensibility_sigma("data_DO_sensibility_sigma", "DO_sensibility_sigma.png")

# ----------------------------------------------------------------------------------------
# Comparison of discrete-time approx and Brownian bridge, in terms of delta
# ----------------------------------------------------------------------------------------



def plot_DO_comparison_delta(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color1 = "#980FB4"
    color3 = "#0D7219"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Time step"], data["Price MC delta"], label = 'Price MC delta', color=color1, marker = 'x', markersize=8)
    plt.plot(data["Time step"], data["Price MC bridge"], label = 'Price MC bridge', color=color3, marker = 'x', markersize=8)

    plt.xlabel("Time step (delta)")
    plt.ylabel("Down & Out Put price (P)")
    plt.legend()

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

#plot_DO_comparison_delta("data_DO_price_comparison_delta", "DO_price_comparison_delta.png")

# -------------------------------------------------------------------------------
# Verifying accuracy of controle variate var red method, against B
# ------------------------------------------------------------------------------


def plot_DO_STD_against_B(file_name,graph_name):
    # read data
    file_script = Path(__file__).resolve().parent
    path_abs = file_script.parent / "output" / file_name
    data = pd.read_csv(path_abs)

    color5 = "#E00505"
    # create graph
    plt.figure(figsize=(8, 5))
    plt.plot(data["Barrier value"], data["STD DO"], color=color5, linewidth=2)
   
    plt.xlabel("Barrier value (B)")
    plt.ylabel("Down & In Put STD")

    graph_path = file_script.parent / "output" / graph_name
    plt.savefig(graph_path, dpi=300) 
    print(f"Graph saved in : {graph_path}")
    #plt.show()

plot_DO_STD_against_B("data_DO_STD_against_B","data_DO_STD_against_B.png")