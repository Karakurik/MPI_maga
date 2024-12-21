#include <bits/stdc++.h>
#include "mpi.h"
#include <random>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0) cerr << "Need at least 3 processes\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int N = 8;
    static double a[N][N];
    static double b[4][N], c[4][N];

    MPI_Datatype oddType, evenType;

    MPI_Type_vector(4, N, 2*N, MPI_DOUBLE, &oddType);
    MPI_Type_commit(&oddType);
    MPI_Type_vector(4, N, 2*N, MPI_DOUBLE, &evenType);
    MPI_Type_commit(&evenType);

    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, 100.0);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                a[i][j] = dist(rng);
            }
        }
        MPI_Send(&a[1][0], 1, oddType, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&a[0][0], 1, evenType, 2, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(b, 4*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << fixed << setprecision(2);
        cout << "Matrix b on process 1:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < N; j++) {
                cout << setw(7) << b[i][j] << " ";
            }
            cout << "\n";
        }
    } else if (rank == 2) {
        MPI_Recv(c, 4*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << fixed << setprecision(2);
        cout << "Matrix c on process 2:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < N; j++) {
                cout << setw(7) << c[i][j] << " ";
            }
            cout << "\n";
        }
    }

    MPI_Type_free(&oddType);
    MPI_Type_free(&evenType);
    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task10.out task10.cpp
// mpirun -np 5 task10.out

/**
 * 10. Объявите и заполните на процессе 0 матрицу a[8][8]. Создайте два новых
типа данных
• для передачи нечетных строк матрицы a в матрицу b[4][8],
• для передачи четных строк матрицы a в матрицу c[4][8].
Требования:
➢ Используйте MPI_Type_vector для создания типа данных.
➢ Выполните передачу данных с использованием созданного типа.
➢ Выведите матрицы b и c на соответствующих процессах.
 * */