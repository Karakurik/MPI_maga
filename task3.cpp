#include <bits/stdc++.h>
#include "mpi.h"
#include <random>

int main(int argc, char **argv) {
    int rank, count;

    const int LENGTH = 10;
    int a[LENGTH];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<> dist(0, 100);

        for (int i = 0; i < 10; ++i) {
            a[i] = dist(rnd);
        }

        MPI_Send(&a, 10, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&a, 10, MPI_INT, 2, 1, MPI_COMM_WORLD);
        MPI_Send(&a, 10, MPI_INT, 3, 1, MPI_COMM_WORLD);
    } else {
        MPI_Probe(1, 1, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(&a, count, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);

        printf("process %d: ", rank);
        for (int i = 0; i < 10; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}

// mpicc -std=c++17 -lstdc++ -o task3.out task3.cpp
// mpirun -np 4 task3.out

/**
 * 3. Напишите программу, осуществляющую пересылку данных одномерного
массива из процесса с номером 1 на остальные процессы группы.
Требования:
➢ Используйте функцию MPI_Send для отправки массива и MPI_Recv для его получения.
➢ На принимающих процессах до вызова функции приема (MPI_Recv) определите есть ли
сообщение для приема с помощью MPI_Probe.
➢ Выделите память для приемного буфера соответствующего размера, а затем вызовите
MPI_Recv для получения данных.
➢ После получения сообщения распечатайте его содержимое на экране.
 * */