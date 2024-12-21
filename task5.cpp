#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) std::cerr << "This program requires exactly 2 processes.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int n = 1000000;
    std::vector<char> buffer(n, 'x');

    MPI_Barrier(MPI_COMM_WORLD);

    double t1, t2;

    // PingPong
    if (rank == 0) {
        t1 = MPI_Wtime();
        MPI_Send(buffer.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer.data(), n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        t2 = MPI_Wtime();
        std::cout << "PingPong time on rank 0: " << (t2 - t1) << " seconds\n";
    } else {
        double t1_local = MPI_Wtime();
        MPI_Recv(buffer.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(buffer.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        double t2_local = MPI_Wtime();
        std::cout << "PingPong time on rank 1: " << (t2_local - t1_local) << " seconds\n";
    }

    MPI_Barrier(MPI_COMM_WORLD);

//    // PingPing
//    t1 = MPI_Wtime();
//    if (rank == 0) {
//        MPI_Send(buffer.data(), n, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
//        MPI_Recv(buffer.data(), n, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    } else {
//        MPI_Send(buffer.data(), n, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
//        MPI_Recv(buffer.data(), n, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    }
//    t2 = MPI_Wtime();
//
//    std::cout << "PingPing time on rank " << rank << ": "
//              << (t2 - t1) << " seconds\n";

    MPI_Finalize();
    return 0;
}

// mpicc -std=c++17 -lstdc++ -o task5.out task5.cpp
// mpirun -np 2 task5.out

/**
 * 5. Разработайте программу, в которой два процесса многократно обмениваются
сообщениями длиной n байт. Выполните эксперименты и оцените
зависимость времени выполнения операции от длины сообщения.
Рассмотрите два варианта обмена сообщениями:
➢ a. PingPong — двунаправленная передача. Один процесс передает данные другому,
после чего второй процесс возвращает полученные данные обратно первому. Время
выполнения замеряется после получения обратного ответа на исходном процессе.
➢ b. PingPing — однонаправленная передача. Два процесса одновременно передают
данные друг другу. Время выполнения замеряется после получения сообщения каждым
процессом.
 * */