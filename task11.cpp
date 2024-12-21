#include <bits/stdc++.h>
#include "mpi.h"
#include <random>
using namespace std;

static const int N = 8;

MPI_Datatype createRowType(int row1, int row2) {
    MPI_Datatype newType;
    int blocklengths[2] = { N, N };
    MPI_Aint displs[2];
    displs[0] = (row1 * N) * sizeof(double);
    displs[1] = (row2 * N) * sizeof(double);
    MPI_Datatype types[2] = { MPI_DOUBLE, MPI_DOUBLE };
    MPI_Type_create_struct(2, blocklengths, displs, types, &newType);
    MPI_Type_commit(&newType);
    return newType;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 4) {
        if (rank == 0) cerr << "Requires at least 4 processes\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    static double a[N][N];
    static double d[2][N];

    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, 100.0);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                a[i][j] = dist(rng);
            }
        }
        for (int proc = 0; proc < 4; proc++) {
            int row1 = proc;
            int row2 = proc + 4;
            if (proc == 0) {
                for (int j = 0; j < N; j++) {
                    d[0][j] = a[row1][j];
                    d[1][j] = a[row2][j];
                }
            } else {
                MPI_Datatype rowType = createRowType(row1, row2);
                MPI_Send(&a[0][0], 1, rowType, proc, 0, MPI_COMM_WORLD);
                MPI_Type_free(&rowType);
            }
        }
    } else {
        MPI_Recv(d, 2*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    cout << fixed << setprecision(2);
    cout << "Process " << rank << ":\n";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(7) << d[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task11.out task11.cpp
// mpirun -np 4 task11.out

/**
 * 11. Создайте новый тип данных для рассылки строк матрицы a[8][8] по четырем
процессам в матрицу d[2][8] следующим образом:
• Процесс 0 получает строки 0 и 4.
• Процесс 1 получает строки 1 и 5.
• Процесс 2 получает строки 2 и 6.
• Процесс 3 получает строки 3 и 7.
Требования:
➢ Используйте MPI_Type_create_struct для описания типа.
➢ Реализуйте передачу данных с использованием созданного типа.
➢ Убедитесь в корректной передаче и выведите локальные результаты.
 * */