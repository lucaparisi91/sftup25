#!/bin/bash

# Slurm job options (job-name, compute nodes, job time)
#SBATCH --job-name=benchmark_all_to_all
#SBATCH --time=00:05:00
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --account=z19-upgrade2025
#SBATCH --reservation=z19-upgrade2025
#SBATCH --qos=reservation
#SBATCH --partition=standard

module load cpe/23.09

#module use /mnt/lustre/a2fs-work4/work/y07/shared/archer2-lmod/others/dev
#module load spack
#spack load hpctoolkit

export OMP_NUM_THREADS=1

export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

echo "Reservation: $SLURM_JOB_RESERVATION"
echo "Nodes: $SLURM_JOB_NUM_NODES"

export MPICH_ENV_DISPLAY=1 
export MPICH_VERSION_DISPLAY=1
#srun --distribution=block:block --hint=nomultithread hpcrun ./test 1024
srun --distribution=block:block --hint=nomultithread ./test 1024