import pandas as pd
import matplotlib.pyplot as plt

def plot_speedup_efficiency(df, title, filename):
    sizes = sorted(df["N"].unique())
    procs = sorted(df["n_procs"].unique())

    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    fig.suptitle(title)

    ax1, ax2 = axes

    for N in sizes:
        subset = df[df["N"] == N].sort_values("n_procs")
        t1 = subset[subset["n_procs"] == 1]["time"].values[0]
        speedup    = t1 / subset["time"].values
        efficiency = speedup / subset["n_procs"].values

        ax1.plot(subset["n_procs"].values, speedup,    marker="o", label=f"N={N}")
        ax2.plot(subset["n_procs"].values, efficiency, marker="o", label=f"N={N}")

    ax1.plot(procs, procs, "k--", label="Ideal")
    ax1.set_xlabel("Number of processes")
    ax1.set_ylabel("Speedup")
    ax1.set_title("Speedup")
    ax1.legend()
    ax1.grid(True)

    ax2.axhline(y=1.0, color="k", linestyle="--", label="Ideal")
    ax2.set_xlabel("Number of processes")
    ax2.set_ylabel("Efficiency")
    ax2.set_title("Efficiency")
    ax2.legend()
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig(filename, dpi=150)
    print(f"Saved {filename}")

# --- Exercise 4 ---
df4 = pd.read_csv("results_ex4.csv")
plot_speedup_efficiency(df4, "Ex4 - Matrix-Vector Product", "speedup_ex4.png")

# --- Exercise 5 ---
df5 = pd.read_csv("results_ex5.csv")
plot_speedup_efficiency(df5, "Ex5 - Pi Calculation", "speedup_ex5.png")

plt.show()
