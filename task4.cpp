#include <bits/stdc++.h>
#include "mpi.h"
#include <random>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 9;
    int workersCount = size - 1;
    int blockSize = (N / workersCount) + (N % workersCount % 2);

    vector<int> xFull, yFull;
    vector<int> zAddFull, zMulFull;

    vector<int> xLocal(blockSize), yLocal(blockSize);

    if (rank == 0) {
        xFull.resize(N);
        yFull.resize(N);
        zAddFull.resize(N);
        zMulFull.resize(N);

        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dist(0, 100);

        for (int i = 0; i < N; i++) {
            xFull[i] = dist(rng);
            yFull[i] = dist(rng);
        }

        cout << "X = ";
        for (int i = 0; i < N; i++) cout << xFull[i] << " ";
        cout << "\nY = ";
        for (int i = 0; i < N; i++) cout << yFull[i] << " ";
        cout << "\n\n";

        for (int worker = 1; worker < size; worker++) {
            int startIndex = (worker - 1) * blockSize;
            MPI_Send(&xFull[startIndex], blockSize, MPI_INT, worker, 0, MPI_COMM_WORLD);
            MPI_Send(&yFull[startIndex], blockSize, MPI_INT, worker, 1, MPI_COMM_WORLD);
        }

        for (int worker = 1; worker < size; worker++) {
            int startIndex = (worker - 1) * blockSize;
            vector<int> zAddBlock(blockSize), zMulBlock(blockSize);

            MPI_Recv(zAddBlock.data(), blockSize, MPI_INT, worker, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(zMulBlock.data(), blockSize, MPI_INT, worker, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < blockSize; i++) {
                zAddFull[startIndex + i] = zAddBlock[i];
                zMulFull[startIndex + i] = zMulBlock[i];
            }
        }

        cout << "Z_add = ";
        for (auto val: zAddFull) cout << val << " ";
        cout << "\nZ_mul = ";
        for (auto val: zMulFull) cout << val << " ";
        cout << "\n\n";
    } else {
        MPI_Recv(xLocal.data(), blockSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(yLocal.data(), blockSize, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> zAddLocal(blockSize), zMulLocal(blockSize);
        for (int i = 0; i < blockSize; i++) {
            zAddLocal[i] = xLocal[i] + yLocal[i];
            zMulLocal[i] = xLocal[i] * yLocal[i];
        }

        MPI_Send(zAddLocal.data(), blockSize, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(zMulLocal.data(), blockSize, MPI_INT, 0, 3, MPI_COMM_WORLD);

        cout << "rank=" << rank << "\n";
        cout << "zAddLocal = ";
        for (auto val: zAddLocal) cout << val << " ";
        cout << "\nzMulLocal = ";
        for (auto val: zMulLocal) cout << val << " ";
        cout << "\n\n";
    }

    MPI_Finalize();
}

// mpicc -std=c++17 -lstdc++ -o task4.out task4.cpp
// mpirun -np 4 task4.out

/**
 * 4. Пусть векторы x и y заданы на нулевом процессе. Напишите программу, в
которой эти векторы равными блоками пересылаются остальным процессам.
Каждый процесс по своей части векторов вычисляет элементы
результирующего массива z по следующим формулам:
a. z[i] = x[i] + y[i]
b. z[i] = x[i] * y[i]
После вычислений каждый процесс отправляет свои результаты на нулевой
процесс, который собирает и выводит итоговые значения массивов.
Требования:
➢ Используйте функцию MPI_Send для отправки массива и MPI_Recv для его получения.
➢ Выведите результат для каждого процесса, запущенного в MPI-среде.
 * */