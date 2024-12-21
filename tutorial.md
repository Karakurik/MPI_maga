# MPI

1. Так как MPI-компилятор находится в библиотеке **open-mpi**, то для начала её и устанавливаем через терминал:
   `brew install open-mpi`

2. После установки необходимые компиляторы mpicc и mpic++ находятся в директории:
   `/opt/homebrew/Cellar/open-mpi/5.0.6/bin`
   (Советую перейти и проверить их наличие)

2.1 На этом этапе можно:
- Создать проект,
- Компилировать и запускать через терминал  
(если хотите привязать `MPI` к среде разработки, то переходите к пункту `3`, там разбирается конкретный случай с `CLion`).

## 2.1.1 Создаем проект (пример кода)

```c++
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
int ierr, num_procs, my_id;

  /* find out MY process ID, and how many processes were started. */

  ierr = MPI_Init(&argc, &argv);

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  printf("Hello world! I'm process %i out of %i processes\n",
         my_id, num_procs);

  ierr = MPI_Finalize();
}
```

В терминале переходим в директорию с файлом .cpp и компилируем с помощью команды:
`mpicc -std=c++17 -lstdc++ -o file.out file.cpp`


(В данном случае параметр -o создаёт загрузочный файл от исходного, после него указывается имя создаваемого файла, по умолчанию создаётся файл a.out.)

## 2.1.2 Запускаем загрузочный файл

С помощью команды mpirun, указываем количество процессов через флаг -np (в данном примере 4 процесса):
`mpirun -np 4 file.out`

```
Вывод (пример):
Hello world! I'm process 1 out of 4 processes
Hello world! I'm process 2 out of 4 processes
Hello world! I'm process 3 out of 4 processes
Hello world! I'm process 0 out of 4 processes
```

3. Создаём проект в CLion и переходим в файл CMakeList.txt.  
   Добавляем директории `mpicc/include`, чтобы CLion знал, откуда брать файл `mpi.h`:

   `include_directories(/opt/homebrew/Cellar/open-mpi/4.1.4_2/include)`

   После этого обновляем CMakeList.txt.

4. Открываем файл main.cpp и импортируем mpi.h:

```c++
#include <mpi.h>

Пишем код (для примера можно взять 1-е задание из задач MPI):

#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    printf("Hello world!\n");
    MPI_Finalize();
}
```

Открываем терминал внутри CLion и пишем команду для компиляции файла:
`mpicc -std=c++17 -lstdc++ -o main.out main.cpp`

- `mpicc` — команда для компиляции  
- `main.cpp` — название компилируемого файла  
- Флаг `-o` — служит для указания загрузочного файла, который позже будем запускать (если не включать этот флаг, то автоматически создастся файл a.out)  
- `main.out` — название загрузочного файла (обязательно расширение .out)

Если появляются ошибки при компиляции, устраняем их. Если всё ок, то запускаем загрузочный файл:
`mpirun main.out`

Если нужно явно указать количество потоков, то добавляем флаг -np (в примере 4 процесса):
`mpirun -np 4 main.out`
