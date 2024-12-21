#include <bits/stdc++.h>
#include "mpi.h"
#include <random>

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 10;
    vector<double> x(N);
    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(-10.0, 10.0);
        for (int i = 0; i < N; i++) {
            x[i] = dist(rng);
        }
    }

    int blockSize = (N / size) + (N % size % 2);
    vector<double> xLocal(blockSize);

    MPI_Scatter(x.data(), blockSize, MPI_DOUBLE, xLocal.data(), blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localSum = 0.0;
    for (auto val: xLocal) {
        localSum += fabs(val);
    }

    double globalSum = 0.0;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << globalSum << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task6.out task6.cpp
// mpirun -np 5 task6.out

/**
 * 6. Напишите программу для вычисления нормы вектора 𝑥 1 = σ𝑖=0
𝑛−1 𝑥𝑖.
Требования:
➢ Для распределения элементов вектора x по процессам используйте функцию
MPI_Scatter.
➢ Для вычисления итоговой суммы на нулевом процессе примените функцию MPI_Reduce
с операцией MPI_SUM.
 * */