#!/usr/bin/env python3
"""
Exercise 1: Plot stride benchmark results
Usage:
    gcc -O0 -o stride_O0 exercise1_stride.c
    gcc -O2 -o stride_O2 exercise1_stride.c
    ./stride_O0 > results_O0.csv
    ./stride_O2 > results_O2.csv
    python3 exercise1_plot.py
"""

import matplotlib.pyplot as plt
import pandas as pd
import os

def read_results(filename):
    if os.path.exists(filename):
        return pd.read_csv(filename)
    return None

# Load results
df_O0 = read_results('res_O0.csv')
df_O2 = read_results('res_O2.csv')

print(df_O0.columns)
print(df_O2.columns)


if df_O0 is None and df_O2 is None:
    print("No result files found")
    exit(1)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

ax1.set_title('CPU Time vs Stride Size', fontsize=12)
if df_O0 is not None:
    ax1.plot(df_O0['stride'], df_O0['time_msec'], 'b-o', label='-O0 (no optimization)', markersize=5)
if df_O2 is not None:
    ax1.plot(df_O2['stride'], df_O2['time_msec'], 'r-s', label='-O2 (optimized)', markersize=5)
ax1.set_xlabel('Stride')
ax1.set_ylabel('CPU Time (ms)')
ax1.legend()
ax1.grid(True, alpha=0.3)

ax2.set_title('Memory Bandwidth vs Stride Size', fontsize=12)
if df_O0 is not None:
    ax2.plot(df_O0['stride'], df_O0['rate_MB_s'], 'b-o', label='-O0 (no optimization)', markersize=5)
if df_O2 is not None:
    ax2.plot(df_O2['stride'], df_O2['rate_MB_s'], 'r-s', label='-O2 (optimized)', markersize=5)
ax2.set_xlabel('Stride')
ax2.set_ylabel('Effective Bandwidth (MB/s)')
ax2.legend()
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('exercise1_results.png', dpi=150)
plt.show()

print("Plot saved to exercise1_results.png")
