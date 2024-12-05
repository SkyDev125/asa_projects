import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline

def plot_theoretical_values(ax, x, T, Teo_F_N_M):
    function = eval(Teo_F_N_M)
    offset = min(T)
    scaling_factor_function = (max(T) - offset) / max(function)
    function_normalized = function * scaling_factor_function + offset
    ax.plot(x, function_normalized, label='O(' + Teo_F_N_M + ")", linestyle='--')
    
# Read data from CSV file
filename = 'spreadsheets/cube_squared_growth.csv'  # Replace with your CSV file name
data = pd.read_csv(filename)

# Assuming the CSV has columns 'firstnum', 'secondnum', and 'mean'
N = data['firstnum'].values
M = data['secondnum'].values
T = data['mean'].values

# Start the graph
fig, ax = plt.subplots()

F_N_M = "M * N"

# Graph Properties
ax.set_xlabel('F(N, M) = ' + F_N_M)
ax.set_ylabel('T (Time mean)')
ax.set_title('Time Complexity Analysis')
ax.grid(True, which='both', linestyle='--', linewidth=0.5)

# x-axis values
F_N_M = eval(F_N_M)

ax.set_xlim([min(F_N_M), max(F_N_M)])
ax.set_ylim([min(T), max(T)])

# Plot the measured times
ax.plot(F_N_M, T, label='Measured Times')

plot_theoretical_values(ax, F_N_M, T, "M**3 * N**2")
plot_theoretical_values(ax, F_N_M, T, "M * N")

ax.legend()

# Save the plot
plt.savefig('time_complexity_analysis.png')