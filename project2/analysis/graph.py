import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline

def calculate_r2(y_true, y_pred):
    ss_res = np.sum((y_true - y_pred) ** 2)
    ss_tot = np.sum((y_true - np.mean(y_true)) ** 2)
    r2 = 1 - (ss_res / ss_tot)
    return r2

def plot_theoretical_values(ax, x, T, Teo_F_N_M_L):
    function = eval(Teo_F_N_M_L)
    offset = min(T)
    scaling_factor_function = (max(T) - offset) / max(function)
    function_normalized = function * scaling_factor_function + offset
    r2 = calculate_r2(T, function_normalized)
    ax.plot(x, function_normalized, label=f'O({Teo_F_N_M_L}) (R²={r2:.3f})', linestyle='--')
    return function_normalized
    
# Read data from CSV file
filename = 'spreadsheets/ve_growth.csv'  # Replace with your CSV file name
data = pd.read_csv(filename)

# Assuming the CSV has columns 'stations', 'connections', 'lines' and 'mean'
N = data['stations'].values
M = data['connections'].values
L = data['lines'].values
T = data['mean'].values

# Start the graph
fig, ax = plt.subplots()

F_N_M_L = "N * M * L"  # Replace with your function

# Graph Properties
ax.set_xlabel('F(N, M, L) = ' + F_N_M_L)
ax.set_ylabel('T (Time mean)')
ax.set_title('Time Complexity Analysis (N & M Grows, L = 200)')
ax.grid(True, which='both', linestyle='--', linewidth=0.5)

# x-axis values
F_N_M_L = eval(F_N_M_L)

ax.set_xlim([min(F_N_M_L), max(F_N_M_L)])
ax.set_ylim([min(T), max(T)])

# Plot the measured times
ax.plot(F_N_M_L, T, label='Measured Times')

# Plot and calculate R² for theoretical values
plot_theoretical_values(ax, F_N_M_L, T, "N")
plot_theoretical_values(ax, F_N_M_L, T, "N**2")
plot_theoretical_values(ax, F_N_M_L, T, "N**3")

ax.legend()

# Save the plot
plt.savefig('time_complexity_analysis.png')