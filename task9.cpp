#include <bits/stdc++.h>
#include "mpi.h"
#include <random>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 8;
    vector<double> A, x(N), z(N);
    int blockSize = (N / size) + (N % size % 2);
    vector<double> localA(blockSize * N);

    if (rank == 0) {
        A.resize(N * N);
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, 10.0);
        for (int i = 0; i < N * N; i++) A[i] = dist(rng);
        for (int i = 0; i < N; i++) x[i] = dist(rng);
    }

    MPI_Scatter(A.data(), blockSize * N, MPI_DOUBLE, localA.data(), blockSize * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(x.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<double> localZ(blockSize, 0.0);
    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < N; j++) {
            localZ[i] += localA[i * N + j] * x[j];
        }
    }

    MPI_Gather(localZ.data(), blockSize, MPI_DOUBLE, z.data(), blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (auto val : z) cout << val << " ";
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task9.out task9.cpp
// mpirun -np 4 task9.out

/**
 * 9. Напишите программу для умножения матрицы на вектор 𝒛 = 𝑨 ∙ 𝒙.
Распределите матрицу A равномерно по строкам между процессами,
вектор x передайте всем процессам. После выполнения операции
умножения на каждом процессе выполните сбор локальных частей
результирующего вектора на одном процессе.
Требования:
➢ Используйте функцию MPI_Bcast для рассылки вектора x, MPI_Scatter для
распределения матрицы
➢ Для сбора локальных частей результирующего вектора в глобальный вектор
используйте функцию MPI_Gather
 * */