#include <bits/stdc++.h>
#include "mpi.h"

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Number of process: %d, size: %d\n", rank, size);
    MPI_Finalize();
}

// mpicc -std=c++17 -lstdc++ -o task1.out task1.cpp
// mpirun -np 4 task1.out

/**
 * 1. Напишите программу, которая выводит для каждого процесса его
уникальный номер (ранг) и общее количество процессов в группе
коммуникатора MPI_COMM_WORLD.
Требования:
➢ Используйте функции MPI_Comm_rank для получения ранга процесса и MPI_Comm_size
для определения общего числа процессов.
➢ Выведите результат для каждого процесса, запущенного в MPI-среде.
 * */