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
    vector<double> x(N), y(N);

    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, 10.0);

        for (int i = 0; i < N; i++) {
            x[i] = dist(rng);
            y[i] = dist(rng);
        }
    }

    int blockSize = (N / size) + (N % size % 2);
    vector<double> xLocal(blockSize), yLocal(blockSize);

    MPI_Scatter(x.data(), blockSize, MPI_DOUBLE, xLocal.data(), blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Scatter(y.data(), blockSize, MPI_DOUBLE, yLocal.data(), blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localDot = 0.0;
    for (int i = 0; i < blockSize; i++) {
        localDot += xLocal[i] * yLocal[i];
    }

    double globalDot = 0.0;
    MPI_Reduce(&localDot, &globalDot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Scalar product: " << globalDot << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task7.out task7.cpp
// mpirun -np 2 task7.out

/**
 * 7. Напишите программу для вычисления скалярного произведения двух
векторов x и y.
Требования:
➢ Для распределения элементов векторов x и y по процессам используйте функцию
MPI_Scatter
➢ Для получения итоговой суммы на нулевом процессе используйте функцию
MPI_Reduce с операцией MPI_SUM.
 * */