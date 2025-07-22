#!/bin/bash

# Slurm job options (job-name, compute nodes, job time)
#SBATCH --job-name=benchmark_all_to_all
#SBATCH --time=02:00:00
#SBATCH --nodes=128
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --account=z19-upgrade2025
#SBATCH --qos=standard
#SBATCH --partition=standard

module load cpe/23.09
export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH
module use /mnt/lustre/a2fs-work4/work/y07/shared/archer2-lmod/others/dev
module load spack
spack load hpctoolkit

export OMP_NUM_THREADS=1

export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

echo "Reservation: $SLURM_JOB_RESERVATION"
echo "Nodes: $SLURM_JOB_NUM_NODES"

fi_info -e > ofi_prod.txt


export MPICH_ENV_DISPLAY=1 
export MPICH_VERSION_DISPLAY=1
export MPICH_COLL_DISPLAY=1
export MPICH_STATS_DISPLAY=1

# OFI/libfabric debug settings
#export FI_LOG_LEVEL=info
#export FI_LOG_PROV=all
export MPICH_OFI_VERBOSE=1

module load craype-network-ucx 
module load cray-mpich-ucx 

srun --distribution=block:block --hint=nomultithread hpcrun -o all_to_all_prod_ucx ./test 1024
#srun --distribution=block:block --hint=nomultithread ./test 1024