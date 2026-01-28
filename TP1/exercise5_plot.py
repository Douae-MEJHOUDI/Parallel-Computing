#!/usr/bin/env python3
"""
Exercise 5: Plot HPL Benchmark Results - Separate figures
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read data
df = pd.read_csv('hpl_results.csv')
df = df[df['Status'] == 'PASSED']

# CPU: Intel i7-6600U @ 2.6GHz, AVX2+FMA = 16 FLOPs/cycle
P_CORE = 2.6 * 16  # 41.6 GFLOP/s

df['Efficiency_pct'] = (df['GFLOPS'] / P_CORE) * 100

N_values = sorted(df['N'].unique())
colors = plt.cm.viridis(np.linspace(0, 0.8, len(N_values)))

plt.figure(figsize=(8, 5))
for i, n in enumerate(N_values):
    data = df[df['N'] == n].sort_values('NB')
    plt.plot(data['NB'], data['GFLOPS'], '-o', color=colors[i], label=f'N={n}', markersize=6)
plt.axhline(y=P_CORE, color='r', linestyle='--', alpha=0.7, label=f'Peak={P_CORE:.1f}')
plt.xlabel('Block Size (NB)')
plt.ylabel('Performance (GFLOP/s)')
plt.xscale('log', base=2)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('ex5_gflops_vs_nb.png', dpi=150)
plt.close()

# Figure 2: Efficiency vs NB
plt.figure(figsize=(8, 5))
for i, n in enumerate(N_values):
    data = df[df['N'] == n].sort_values('NB')
    plt.plot(data['NB'], data['Efficiency_pct'], '-s', color=colors[i], label=f'N={n}', markersize=6)
plt.axhline(y=100, color='r', linestyle='--', alpha=0.7, label='100%')
plt.xlabel('Block Size (NB)')
plt.ylabel('Efficiency (%)')
plt.xscale('log', base=2)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('ex5_efficiency_vs_nb.png', dpi=150)
plt.close()

# Figure 3: Time vs NB
plt.figure(figsize=(8, 5))
for i, n in enumerate(N_values):
    data = df[df['N'] == n].sort_values('NB')
    plt.plot(data['NB'], data['Time_s'], '-^', color=colors[i], label=f'N={n}', markersize=6)
plt.xlabel('Block Size (NB)')
plt.ylabel('Time (seconds)')
plt.xscale('log', base=2)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('ex5_time_vs_nb.png', dpi=150)
plt.close()

# Figure 4: GFLOPS vs N
plt.figure(figsize=(8, 5))
for nb in [32, 64, 128]:
    data = df[df['NB'] == nb].sort_values('N')
    plt.plot(data['N'], data['GFLOPS'], '-d', label=f'NB={nb}', markersize=8)
plt.axhline(y=P_CORE, color='r', linestyle='--', alpha=0.7, label='Peak')
plt.xlabel('Matrix Size (N)')
plt.ylabel('Performance (GFLOP/s)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('ex5_gflops_vs_n.png', dpi=150)
plt.close()

print("Figures saved: ex5_gflops_vs_nb.png, ex5_efficiency_vs_nb.png, ex5_time_vs_nb.png, ex5_gflops_vs_n.png")
print(f"\nP_CORE = {P_CORE:.1f} GFLOP/s")
print("\nResults:")
for n in N_values:
    best = df[df['N'] == n].loc[df[df['N'] == n]['GFLOPS'].idxmax()]
    print(f"N={n:5d}: NB={int(best['NB']):3d}, Time={best['Time_s']:.2f}s, {best['GFLOPS']:.2f} GFLOP/s, {best['Efficiency_pct']:.1f}%")
