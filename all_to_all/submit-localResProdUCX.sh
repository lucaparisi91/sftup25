#!/bin/bash

# Slurm job options (job-name, compute nodes, job time)
#SBATCH --job-name=ucx_group_prod_benchmark_all_to_all
#SBATCH --time=02:00:00
#SBATCH --nodes=128
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --account=z19-upgrade2025
#SBATCH --reservation=z19-upgrade2025.group
#SBATCH --qos=reservation
#SBATCH --partition=standard

export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH
module use /mnt/lustre/a2fs-work4/work/y07/shared/archer2-lmod/others/dev
module load spack
spack load hpctoolkit

#srun --distribution=block:block --hint=nomultithread --nodes=$SLURM_NNODES --tasks=$SLURM_NNODES bash -c 'ldd ./test 2>&1 | tee ldd.$(hostname).txt'


export OMP_NUM_THREADS=1

export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

echo "Reservation: $SLURM_JOB_RESERVATION"
echo "Nodes: $SLURM_JOB_NUM_NODES"

fi_info -e > ofi_v0.4.txt


export MPICH_ENV_DISPLAY=1
export MPICH_VERSION_DISPLAY=1
export MPICH_OFI_VERBOSE=1
#export FI_OFI_RXM_RX_SIZE=65536
#export FI_OFI_RXM_TX_SIZE=65536
#export FI_OFI_RXM_MSG_RX_SIZE=128
#export FI_OFI_RXM_ENABLE_DIRECT_SEND=false

NELEMS=1024

module load craype-network-ucx 
module load cray-mpich-ucx 

module list
#srun --distribution=block:block --hint=nomultithread  hpcrun -o hpctoolkit_all_to_all_Imgv0.4N${SLURM_NNODES} ./test $NELEMS
srun --distribution=block:block --hint=nomultithread ./test $NELEMS