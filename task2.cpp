#include <bits/stdc++.h>
#include "mpi.h"
#include <random>

int main(int argc, char **argv) {

    int rank, count;
    int const SIZE = 10;
    int a[SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    if (count != 2) {
        if (rank == 0) std::cerr << "This program requires exactly 2 processes.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<> dist(0, 100);

        for (int i = 0; i < SIZE; ++i) {
            a[i] = dist(rnd);
        }

        MPI_Send(a, SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD);
    }

    if (rank == 1) {
        MPI_Recv(a, SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", a[i]);
        }
    }

    MPI_Finalize();
}

// mpicc -std=c++17 -lstdc++ -o task2.out task2.cpp
// mpirun -np 2 task2.out

/**
 * 2. Напишите программу, которая будет запускаться на двух процессах. На
нулевом процессе создайте массив a из 10 случайных элементов.
Перешлите этот массив целиком первому процессу. На первом процессе
распечатайте полученный массив.
Требования:
➢ Используйте функцию MPI_Send для отправки массива и MPI_Recv для его получения.
➢ Программа должна корректно работать с двумя процессами, и вывод на первом
процессе должен содержать элементы массива, полученные от нулевого процесса.
 * */