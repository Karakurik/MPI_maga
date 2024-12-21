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
    vector<double> arr(N);

    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(-100.0, 100.0);
        for (int i = 0; i < N; i++) {
            arr[i] = dist(rng);
        }
    }

    int blockSize = (N / size) + (N % size % 2);
    vector<double> localVec(blockSize);

    MPI_Scatter(arr.data(), blockSize, MPI_DOUBLE, localVec.data(), blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localMinVal = numeric_limits<double>::max();
    int localMinIdx = -1;
    for (int i = 0; i < blockSize; i++) {
        if (localVec[i] < localMinVal) {
            localMinVal = localVec[i];
            localMinIdx = i;
        }
    }

    struct {
        double val;
        int idx;
    } localData, globalData;

    localData.val = localMinVal;
    localData.idx = localMinIdx + rank * blockSize;

    MPI_Reduce(&localData, &globalData, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << fixed << setprecision(3);
        cout << "Minimum value: " << globalData.val << endl;
        cout << "Global index:  " << globalData.idx << endl;
    }

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task8.out task8.cpp
// mpirun -np 2 task8.out

/**
 * 8. В массиве вещественных чисел найдите минимальное значение и
глобальный индекс минимального элемента.
Требования:
➢ Для распределения элементов массива по процессам используйте функцию
MPI_Scatter
➢ Для нахождения минимального значения и его индекса используйте функцию
MPI_Reduce с операцией MPI_MINLOC.
 * */