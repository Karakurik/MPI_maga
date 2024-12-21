#include <bits/stdc++.h>
#include "mpi.h"
#include <random>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 8;
    static double a[N][N];

    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, 100.0);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (j >= i) a[i][j] = dist(rng);
                else a[i][j] = 0.0;
            }
        }
    } else {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                a[i][j] = 0.0;
            }
        }
    }

    int blocklengths[N];
    int displs[N];
    for (int i = 0; i < N; i++) {
        blocklengths[i] = N - i;
        displs[i] = i * N + i;
    }

    MPI_Datatype upperTriType;
    MPI_Type_indexed(N, blocklengths, displs, MPI_DOUBLE, &upperTriType);
    MPI_Type_commit(&upperTriType);

    MPI_Bcast(&a[0][0], 1, upperTriType, 0, MPI_COMM_WORLD);

    cout << "Process " << rank << ":\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << fixed << setw(8) << setprecision(2) << a[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    MPI_Type_free(&upperTriType);
    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task12.out task12.cpp
// mpirun -np 4 task12.out

/**
 * 12. На одном процессе объявите и заполните верхнюю треугольную матрицу.
Создайте новый тип данных, исключая нулевые элементы, и передайте
элементы матрицы всем процессам.
Требования:
➢ Для создания нового типа данных используйте функцию MPI_Type_indexed
➢ Используйте MPI_Bcast для передачи ненулевых элементов матрицы всем
процессам.
➢ Напечатайте полученные элементы матрицы на каждом процессе.
 * */