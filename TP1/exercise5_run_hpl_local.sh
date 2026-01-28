#!/bin/bash
#=============================================================================
# Exercise 5: HPL Benchmark Script (LOCAL VERSION)
# Runs HPL for different matrix sizes (N) and block sizes (NB)
#=============================================================================

# HPL location
HPL_DIR="/tmp/hpl-2.3/bin/Linux_Local"
MPIRUN="/usr/bin/mpirun"

# Matrix sizes to test
N_VALUES=(1000 2000 5000 10000)

# Block sizes to test
NB_VALUES=(1 2 4 8 16 32 64 128 256)

# Output file for results
OUTPUT_FILE="hpl_results.csv"

# Get CPU frequency for theoretical peak calculation
CPU_FREQ=$(lscpu | grep "CPU max MHz" | awk '{print $4}' | cut -d'.' -f1)
if [ -z "$CPU_FREQ" ]; then
    CPU_FREQ=$(lscpu | grep "CPU MHz" | awk '{print $3}' | cut -d'.' -f1)
fi
if [ -z "$CPU_FREQ" ]; then
    CPU_FREQ=2500  # Default
fi

# Check for AVX support and calculate FLOPs per cycle
if grep -q avx512 /proc/cpuinfo; then
    FLOPS_PER_CYCLE=32
    SIMD="AVX-512"
elif grep -q avx2 /proc/cpuinfo; then
    FLOPS_PER_CYCLE=16
    SIMD="AVX2"
elif grep -q avx /proc/cpuinfo; then
    FLOPS_PER_CYCLE=8
    SIMD="AVX"
else
    FLOPS_PER_CYCLE=4
    SIMD="SSE"
fi

# Theoretical peak (GFLOP/s)
P_CORE=$(echo "scale=2; $CPU_FREQ / 1000 * $FLOPS_PER_CYCLE" | bc)

echo "=============================================="
echo "HPL Benchmark - Exercise 5 (LOCAL)"
echo "=============================================="
echo "CPU Frequency: $CPU_FREQ MHz"
echo "SIMD: $SIMD ($FLOPS_PER_CYCLE FLOPs/cycle)"
echo "Theoretical Peak (1 core): $P_CORE GFLOP/s"
echo "Matrix sizes: ${N_VALUES[*]}"
echo "Block sizes: ${NB_VALUES[*]}"
echo "Total runs: $((${#N_VALUES[@]} * ${#NB_VALUES[@]}))"
echo "=============================================="
echo ""

# Check HPL exists
if [ ! -f "$HPL_DIR/xhpl" ]; then
    echo "ERROR: xhpl not found at $HPL_DIR"
    echo "Please build HPL first"
    exit 1
fi

cd "$HPL_DIR"

# Backup original HPL.dat
cp HPL.dat HPL.dat.backup 2>/dev/null

# Create header for results file
echo "N,NB,Time_s,GFLOPS,Efficiency_pct,Status" > "$OUTPUT_FILE"

# Function to update HPL.dat
update_hpl_dat() {
    local n=$1
    local nb=$2

    cat > HPL.dat << EOF
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)
$n           Ns
1            # of NBs
$nb          NBs
0            PMAP process mapping (0=Row-,1=Column-major)
1            # of process grids (P x Q)
1            Ps
1            Qs
16.0         threshold
1            # of panel fact
2            PFACTs (0=left, 1=Crout, 2=Right)
1            # of recursive stopping criterium
4            NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
1            RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
1            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
1            DEPTHs (>=0)
2            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold
0            L1 in (0=transposed,1=no-transposed) form
0            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
EOF
}

# Counter for progress
run_count=0
total_runs=$((${#N_VALUES[@]} * ${#NB_VALUES[@]}))

# Main loop
for n in "${N_VALUES[@]}"; do
    echo "==========================================="
    echo "Testing N = $n"
    echo "==========================================="

    for nb in "${NB_VALUES[@]}"; do
        run_count=$((run_count + 1))
        echo -n "[$run_count/$total_runs] N=$n, NB=$nb ... "

        update_hpl_dat $n $nb

        # Run HPL
        $MPIRUN -np 1 ./xhpl > hpl_temp_output.txt 2>&1

        # Parse results
        result_line=$(grep "^WR" hpl_temp_output.txt | tail -1)

        if [ -n "$result_line" ]; then
            time=$(echo $result_line | awk '{print $6}')
            gflops=$(echo $result_line | awk '{print $7}')

            if grep -q "PASSED" hpl_temp_output.txt; then
                status="PASSED"
            else
                status="FAILED"
            fi

            efficiency=$(echo "scale=2; $gflops / $P_CORE * 100" | bc 2>/dev/null || echo "N/A")
            echo "Time: ${time}s, GFLOP/s: $gflops, Eff: ${efficiency}%"
        else
            time="N/A"
            gflops="N/A"
            efficiency="N/A"
            status="ERROR"
            echo "ERROR"
        fi

        echo "$n,$nb,$time,$gflops,$efficiency,$status" >> "$OUTPUT_FILE"
    done
    echo ""
done

# Copy results to TP1 directory
cp "$OUTPUT_FILE" "/home/khaoula/cours/ci2/S8/Parallel computing/TP1/"

echo "=============================================="
echo "All runs completed!"
echo "Results saved to: /home/khaoula/cours/ci2/S8/Parallel computing/TP1/$OUTPUT_FILE"
echo "=============================================="
echo ""
echo "Run 'python3 exercise5_plot.py' to generate plots"

rm -f hpl_temp_output.txt
