#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int arrInt[10];
    double arrDouble[8];
    if (rank == 0) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dist1(0, 100);
        uniform_real_distribution<double> dist2(0, 100);
        for (int i = 0; i < 10; i++) arrInt[i] = dist1(rng);
        for (int i = 0; i < 8; i++) arrDouble[i] = dist2(rng);
    }

    int packSizeInt, packSizeDouble;
    MPI_Pack_size(10, MPI_INT, MPI_COMM_WORLD, &packSizeInt);
    MPI_Pack_size(8, MPI_DOUBLE, MPI_COMM_WORLD, &packSizeDouble);
    int packSize = packSizeInt + packSizeDouble;

    vector<char> buffer(packSize);
    int position = 0;
    if (rank == 0) {
        MPI_Pack(arrInt, 10, MPI_INT, buffer.data(), packSize, &position, MPI_COMM_WORLD);
        MPI_Pack(arrDouble, 8, MPI_DOUBLE, buffer.data(), packSize, &position, MPI_COMM_WORLD);
    }

    MPI_Bcast(buffer.data(), packSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    position = 0;
    MPI_Unpack(buffer.data(), packSize, &position, arrInt, 10, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(buffer.data(), packSize, &position, arrDouble, 8, MPI_DOUBLE, MPI_COMM_WORLD);

    cout << "Process " << rank << ":\nInts: ";
    for (int i = 0; i < 10; i++) cout << arrInt[i] << " ";
    cout << "\nDoubles: ";
    for (int i = 0; i < 8; i++) cout << arrDouble[i] << " ";
    cout << "\n\n";

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task13.out task13.cpp
// mpirun -np 5 task13.out

/**
 * 13. На процессе 0 задайте массив из 10 целых чисел и массив из 8
вещественных чисел. Упакуйте оба массива в единый буфер. Передайте
упакованные данные всем процессам. На каждом процессе распакуйте
данные из буфера и напечатайте их.
Требования:
• Вычислите размер буфера в соответствии с количеством и типом данных, которые
будут в него упакованы, с помощью функции MPI_Pack_size.
• Для упаковки данных используйте функцию MPI_Pack.
• Передачу упакованного буфер выполните с помощью функции MPI_Bcast.
• Распаковку данных реализуйте функцией MPI_Unpack и проверьте результат на всех
процессах.
 * */