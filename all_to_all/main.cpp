
#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parameters
    const int count = 1024; // elements per process
    const int warmup_iters = 1;
    const int timed_iters = 20;
    bool print_intermediate_steps = false;

    std::vector<double> sendbuf(count * size, static_cast<double>(rank));
    std::vector<double> recvbuf(count * size);

    // Warmup iterations (timed)
    double warmup_total_time = 0.0;
    for (int i = 0; i < warmup_iters; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto start = std::chrono::high_resolution_clock::now();
        MPI_Alltoall(sendbuf.data(), count, MPI_DOUBLE,
                    recvbuf.data(), count, MPI_DOUBLE, MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        warmup_total_time += elapsed;
        if (rank == 0 && print_intermediate_steps) {
            std::cout << "Warmup Iteration " << i+1 << ": " << elapsed << " seconds" << std::endl;
        }
    }

    // Timed iterations
    double total_time = 0.0;
    for (int i = 0; i < timed_iters; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto start = std::chrono::high_resolution_clock::now();
        MPI_Alltoall(sendbuf.data(), count, MPI_DOUBLE,
                    recvbuf.data(), count, MPI_DOUBLE, MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        total_time += elapsed;
        if (rank == 0 && print_intermediate_steps) {
            std::cout << "Iteration " << i+1 << ": " << elapsed << " seconds" << std::endl;
        }
    }

    if (rank == 0) {
        std::cout << "Average warmup time per iteration: " << (warmup_total_time / warmup_iters) << " seconds" << std::endl;
        std::cout << "Average timed iteration: " << (total_time / timed_iters) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
