
#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

int main(int argc, char** argv) {
#include <cmath>

    auto compute_stddev = [](double sum_sq, double sum, int n) -> double {
        if (n <= 1) return 0.0;
        double mean = sum / n;
        double var = (sum_sq / n) - (mean * mean);
        return std::sqrt(var * n / (n - 1));
    };
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parameters
    int count = 1024; // elements per process (default)
    const int warmup_iters = 1;
    const int timed_iters = 10;
    bool print_intermediate_steps = true;




    // Check command line arguments for count
    if (argc > 1) {
        try {
            count = std::stoi(argv[1]);
        } catch (...) {
            if (rank == 0) {
                std::cerr << "Invalid count argument, using default: " << count << std::endl;
            }
        }
    }

    if (rank == 0 )
    {
        std::cout << "Running All-to-All Benchmark with " << size << " processes." << std::endl;
        std::cout << "Each process will send and receive " << count << " elements." << std::endl;
        std::cout << "Warmup iterations: " << warmup_iters << ", Timed iterations: " << timed_iters << std::endl;
        
    }


    std::vector<double> sendbuf(count * size, static_cast<double>(rank));
    std::vector<double> recvbuf(count * size);

    // Warmup iterations (timed)
    double warmup_total_time = 0.0;
    double warmup_total_time_sq = 0.0;
    for (int i = 0; i < warmup_iters; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto start = std::chrono::high_resolution_clock::now();
        MPI_Alltoall(sendbuf.data(), count, MPI_DOUBLE,
                    recvbuf.data(), count, MPI_DOUBLE, MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        warmup_total_time += elapsed;
        warmup_total_time_sq += elapsed * elapsed;
        if (rank == 0 && print_intermediate_steps) {
            std::cout << "Warmup Iteration " << i+1 << ": " << elapsed << " seconds" << std::endl;
        }
    }

    // Timed iterations
    double total_time = 0.0;
    double total_time_sq = 0.0;
    for (int i = 0; i < timed_iters; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto start = std::chrono::high_resolution_clock::now();
        MPI_Alltoall(sendbuf.data(), count, MPI_DOUBLE,
                    recvbuf.data(), count, MPI_DOUBLE, MPI_COMM_WORLD);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        total_time += elapsed;
        total_time_sq += elapsed * elapsed;
        if (rank == 0 && print_intermediate_steps) {
            std::cout << "Iteration " << i+1 << ": " << elapsed << " seconds" << std::endl;
        }
    }

    if (rank == 0) {
        double warmup_avg = warmup_total_time / warmup_iters;
        double warmup_stddev = compute_stddev(warmup_total_time_sq, warmup_total_time, warmup_iters);

        double timed_avg = total_time / timed_iters;
        double timed_stddev = compute_stddev(total_time_sq, total_time, timed_iters);

        std::cout << "Average warmup time per iteration: " << warmup_avg << " seconds\n";
        std::cout << "Warmup stddev: " << warmup_stddev << " seconds\n";
        std::cout << "Average timed iteration: " << timed_avg << " seconds\n";
        std::cout << "Timed stddev: " << timed_stddev << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
