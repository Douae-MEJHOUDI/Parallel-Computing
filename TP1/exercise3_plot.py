#!/usr/bin/env python3
"""
Exercise 3: Plot block matrix multiplication results
Usage:
    gcc -O2 -o mxm_block exercise3_mxm_bloc.c
    ./mxm_block > results_block.csv
    python3 exercise3_plot.py
"""

import matplotlib.pyplot as plt
import pandas as pd
import os

# Read data from CSV file
filename = 'results_block.csv'

if not os.path.exists(filename):
    print(f"No result file '{filename}' found")

    exit(1)

df = pd.read_csv(filename, skiprows=4, names=['block_size', 'time_ms', 'bandwidth_MB_s', 'gflops'])

print("Data loaded:")
print(df)

best_idx = df['gflops'].idxmax()
best_block = df.loc[best_idx, 'block_size']
best_gflops = df.loc[best_idx, 'gflops']

print(f"\nOptimal block size: {best_block} with {best_gflops:.2f} GFLOPS")

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

ax1.set_title('CPU Time vs Block Size', fontsize=12)
ax1.plot(df['block_size'], df['time_ms'], 'b-o', markersize=6)
ax1.axvline(x=best_block, color='r', linestyle='--', alpha=0.7, label=f'Optimal: {best_block}')
ax1.set_xlabel('Block Size')
ax1.set_ylabel('CPU Time (ms)')
ax1.legend()
ax1.grid(True, alpha=0.3)

ax2.set_title('Performance vs Block Size', fontsize=12)
ax2.plot(df['block_size'], df['gflops'], 'g-s', markersize=6)
ax2.axvline(x=best_block, color='r', linestyle='--', alpha=0.7, label=f'Optimal: {best_block}')
ax2.scatter([best_block], [best_gflops], color='red', s=100, zorder=5, label=f'Best: {best_gflops:.2f} GFLOPS')
ax2.set_xlabel('Block Size')
ax2.set_ylabel('Performance (GFLOPS)')
ax2.legend()
ax2.grid(True, alpha=0.3)

plt.suptitle('Block Matrix Multiplication Performance Analysis', fontsize=14)
plt.tight_layout()
plt.savefig('exercise3_results.png', dpi=150)
plt.show()

print("\nPlot saved to exercise3_results.png")
