import matplotlib.pyplot as plt

threads = [1, 4, 16, 36]
cpu_times = [5.488e-02, 1.405e-02, 8.258e-03, 9.502e-03]

t1 = cpu_times[0]
speedup = [t1 / t for t in cpu_times]
efficiency = [s / p for s, p in zip(speedup, threads)]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Speedup plot
ax1.plot(threads, speedup, 'bo-', label='Measured speedup')
ax1.plot(threads, threads, 'r--', label='Ideal speedup')
ax1.set_xlabel('Number of threads')
ax1.set_ylabel('Speedup')
ax1.set_title('Speedup - Jacobi Method (n=500)')
ax1.set_xticks(threads)
ax1.legend()
ax1.grid(True)

# Efficiency plot
ax2.plot(threads, efficiency, 'go-', label='Measured efficiency')
ax2.axhline(y=1.0, color='r', linestyle='--', label='Ideal efficiency')
ax2.set_xlabel('Number of threads')
ax2.set_ylabel('Efficiency')
ax2.set_title('Efficiency - Jacobi Method (n=500)')
ax2.set_xticks(threads)
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig('/home/khaoula/cours/ci2/S8/Parallel computing/TP3/plot_ex5.png', dpi=150)
print("Plot saved to plot_ex5.png")
