## Инициализация и завершение работы

### `MPI_Init`
```c++
int MPI_Init(int *argc, char ***argv);
```
Инициализирует среду MPI. Вызывается в самом начале `main` до использования любых других функций MPI.

### `MPI_Finalize`
```c++
int MPI_Finalize(void);
```
Завершает работу MPI. Вызывается в конце программы, когда больше не нужно взаимодействовать через MPI.

### `MPI_Abort`
```c++
int MPI_Abort(MPI_Comm comm, int errorcode);
```
Принудительно завершает все процессы в коммуникаторе `comm`. Обычно вызывается при обнаружении фатальной ошибки.

---

## Определение параметров процесса

### `MPI_Comm_rank`
```c++
int MPI_Comm_rank(MPI_Comm comm, int *rank);
```
Возвращает **ранг** (номер) процесса в коммуникаторе (часто `MPI_COMM_WORLD`). Нумерация рангов: от 0 до size-1.

### `MPI_Comm_size`
```c++
int MPI_Comm_size(MPI_Comm comm, int *size);
```
Возвращает общее число процессов в коммуникаторе.

---

## Точка синхронизации

### `MPI_Barrier`
```c++
int MPI_Barrier(MPI_Comm comm);
```
Все процессы останавливаются на вызове `MPI_Barrier`, пока каждый не дойдёт до него (глобальная синхронизация).

---

## Работа со временем

### `MPI_Wtime`
```c++
double MPI_Wtime(void);
```
Возвращает время (в секундах) с некоторой точки отсчёта, зависящей от реализации MPI. Используется для замера времени выполнения.

---

## Точка-точка (point-to-point) взаимодействие

### `MPI_Send`
```c++
int MPI_Send(const void *buf, int count, MPI_Datatype datatype,
             int dest, int tag, MPI_Comm comm);
```
Отправляет `count` элементов типа `datatype` из буфера `buf` процессу `dest` с «меткой» `tag`. Блокируется до тех пор, пока буфер не станет снова доступным.

### `MPI_Recv`
```c++
int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
             int source, int tag, MPI_Comm comm, MPI_Status *status);
```
Принимает `count` элементов типа `datatype` от процесса `source` с меткой `tag`. Параметр `status` даёт информацию о фактически полученных данных (количество, источник и т.п.).

---

## Дополнительные функции точка-точка

### `MPI_Probe`
```c++
int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status);
```
Позволяет «прощупать» (проверить наличие) входящего сообщения, не считывая его. Возвращает `status`, по которому можно узнать реальное число элементов.

### `MPI_Get_count`
```c++
int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count);
```
Определяет, сколько элементов типа `datatype` фактически было получено при `MPI_Recv`, используя `status`.

---

## Коллективные операции

### `MPI_Scatter`
```c++
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm);
```
Разбивает данные на **root**-процессе на равные блоки и рассылает всем процессам.

### `MPI_Gather`
```c++
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm);
```
Собирает данные от всех процессов в **root**-процессе.

### `MPI_Bcast`
```c++
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype,
              int root, MPI_Comm comm);
```
Рассылает данные из `buffer` на **root**-процессе всем остальным процессам.

### `MPI_Reduce`
```c++
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
```
Применяет операцию `op` (например, `MPI_SUM`, `MPI_MIN`, `MPI_MAX`, `MPI_MINLOC`) к данным во всех процессах и помещает результат в `recvbuf` на **root**.

---

## Создание пользовательских типов данных

### `MPI_Type_vector`
```c++
int MPI_Type_vector(int count, int blocklength, int stride,
                    MPI_Datatype oldtype, MPI_Datatype *newtype);
```
Создаёт тип, описывающий «вектор» блоков (подряд идущих `blocklength` элементов) с шагом `stride`.

### `MPI_Type_create_struct`
```c++
int MPI_Type_create_struct(int count, const int blocklengths[],
                           const MPI_Aint displacements[],
                           const MPI_Datatype types[],
                           MPI_Datatype *newtype);
```
Позволяет описать произвольную структуру данных из нескольких «полей» разного типа и со смещениями `displacements`.

### `MPI_Type_indexed`
```c++
int MPI_Type_indexed(int count, const int array_of_blocklengths[],
                     const int array_of_displacements[],
                     MPI_Datatype oldtype, MPI_Datatype *newtype);
```
Описывает набор разрозненных блоков, каждый из которых имеет свою длину и смещение.

### `MPI_Type_commit`
```c++
int MPI_Type_commit(MPI_Datatype *datatype);
```
Подтверждает созданный тип, делая его готовым к использованию при `MPI_Send`/`MPI_Recv`.

### `MPI_Type_free`
```c++
int MPI_Type_free(MPI_Datatype *datatype);
```
Освобождает ресурсы, связанные с пользовательским типом.

---

## Упаковка и распаковка

### `MPI_Pack_size`
```c++
int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size);
```
Вычисляет, сколько байт нужно для упаковки `incount` элементов `datatype`. Используется, чтобы заранее выделить буфер.

### `MPI_Pack`
```c++
int MPI_Pack(const void *inbuf, int incount, MPI_Datatype datatype,
             void *outbuf, int outsize, int *position, MPI_Comm comm);
```
Упаковывает данные из `inbuf` (элементов `incount` типа `datatype`) в «сырой» буфер `outbuf`, начиная с `position`, которую по мере упаковки нужно сдвигать.

### `MPI_Unpack`
```c++
int MPI_Unpack(const void *inbuf, int insize, int *position,
               void *outbuf, int outcount, MPI_Datatype datatype,
               MPI_Comm comm);
```
Из «сырого» буфера `inbuf` извлекает (распаковывает) данные в `outbuf` с учётом текущего `position`.

---

## Структуры и константы

- **`MPI_Status`** — структура с информацией о принятом сообщении (поля `MPI_SOURCE`, `MPI_TAG`, `MPI_ERROR`).
- **`MPI_COMM_WORLD`** — глобальный коммуникатор, включающий все процессы.
- **`MPI_CHAR`, `MPI_INT`, `MPI_DOUBLE`** и т.д. — предопределённые типы MPI.
- **`MPI_SUM`, `MPI_MINLOC`, `MPI_MAX`, `MPI_PROD`** и другие — стандартные операции, которые применяются, например, в `MPI_Reduce` или `MPI_Allreduce`.
