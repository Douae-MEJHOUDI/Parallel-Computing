import matplotlib.pyplot as plt

threads = [1, 4, 8, 16, 36]

# Without collapse
times_no_collapse = [8.463, 1.739, 1.083, 0.812, 0.551]
t1_no = times_no_collapse[0]
speedup_no = [t1_no / t for t in times_no_collapse]
efficiency_no = [s / p for s, p in zip(speedup_no, threads)]

# With collapse(2)
times_collapse = [5.501, 1.448, 0.698, 0.351, 0.159]
t1_col = times_collapse[0]
speedup_col = [t1_col / t for t in times_collapse]
efficiency_col = [s / p for s, p in zip(speedup_col, threads)]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Speedup plot
ax1.plot(threads, speedup_no, 'bo-', label='parallel for')
ax1.plot(threads, speedup_col, 'gs-', label='parallel for collapse(2)')
ax1.plot(threads, threads, 'r--', label='Ideal')
ax1.set_xlabel('Number of threads')
ax1.set_ylabel('Speedup')
ax1.set_title('Speedup comparison')
ax1.set_xticks(threads)
ax1.legend()
ax1.grid(True)

# Efficiency plot
ax2.plot(threads, efficiency_no, 'bo-', label='parallel for')
ax2.plot(threads, efficiency_col, 'gs-', label='parallel for collapse(2)')
ax2.axhline(y=1.0, color='r', linestyle='--', label='Ideal')
ax2.set_xlabel('Number of threads')
ax2.set_ylabel('Efficiency')
ax2.set_title('Efficiency comparison')
ax2.set_xticks(threads)
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig('/home/khaoula/cours/ci2/S8/Parallel computing/TP3/plot_ex4_comparison.png', dpi=150)
print("Plot saved to plot_ex4_comparison.png")
