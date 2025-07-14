#!/bin/bash

# Slurm job options (job-name, compute nodes, job time)
#SBATCH --job-name=benchmark_all_to_all
#SBATCH --time=02:00:00
#SBATCH --nodes=128
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --account=z19-upgrade2025
#SBATCH --reservation=z19-upgrade2025
#SBATCH --qos=reservation
#SBATCH --partition=standard


export OMP_NUM_THREADS=1

export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

echo "Reservation: $SLURM_JOB_RESERVATION"

srun --distribution=block:block --hint=nomultithread ./test 1024