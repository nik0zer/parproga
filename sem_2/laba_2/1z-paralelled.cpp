#include <chrono>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv) {
  int rank, size;
  constexpr auto length = (ISIZE + 1) * (JSIZE + 1) * sizeof(double);
  double *a = (double *)(mmap(NULL, length, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
  std::cout << a << std::endl;
  int i, j;
  FILE *ff;
  std::chrono::_V2::system_clock::time_point start;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 3 && size != 5) {
    if (rank == 0) {
      printf("нужно больше 3 или 5 процесса\n");
    }
    MPI_Finalize();
    return 1;
  }

  if (size == 3) {

    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        a[i * JSIZE + j] = 10 * i + j;
      }
    }

    if (rank == 1) {
      start = std::chrono::high_resolution_clock::now();
    }

    if (rank >= 1 && rank <= 2) {
      for (j = 4 + rank - 1; j < JSIZE; j += 2) {
        for (i = 2; i < ISIZE; i++) {
          a[i * JSIZE + j] = sin(4 * a[(i - 2) * JSIZE + j - 4]);
        }
      }

      MPI_Send(a, length, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 1) {
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      std::cout << "working time: " << static_cast<double>(duration.count())
                << std::endl;
    }

    if (rank == 0) {
      double *receivedPart =
          (double *)(mmap(NULL, length, PROT_READ | PROT_WRITE,
                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
      for (int sourceRank = 1; sourceRank <= 2; sourceRank++) {
        MPI_Recv(receivedPart, length, MPI_BYTE, sourceRank, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        for (int j = sourceRank - 1; j < JSIZE; j += 2) {
          for (int i = 0; i < ISIZE; i++) {
            a[i * JSIZE + j] = receivedPart[i * JSIZE + j];
          }
        }
      }

      ff = fopen("result1paralelled.txt", "w");
      for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
          fprintf(ff, "%f ", a[i * JSIZE + j]);
        }
        fprintf(ff, "\n");
      }
      fclose(ff);
    }

  } else {
    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        a[i * JSIZE + j] = 10 * i + j;
      }
    }

    if (rank == 1) {
      start = std::chrono::high_resolution_clock::now();
    }

    if (rank >= 1 && rank <= 4) {
      for (j = 4 + rank - 1; j < JSIZE; j += 4) {
        for (i = 2; i < ISIZE; i++) {
          a[i * JSIZE + j] = sin(4 * a[(i - 2) * JSIZE + j - 4]);
        }
      }

      MPI_Send(a, length, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 1) {
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      std::cout << "working time: " << static_cast<double>(duration.count())
                << std::endl;
    }

    if (rank == 0) {
      double *receivedPart =
          (double *)(mmap(NULL, length, PROT_READ | PROT_WRITE,
                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
      for (int sourceRank = 1; sourceRank <= 4; sourceRank++) {
        MPI_Recv(receivedPart, length, MPI_BYTE, sourceRank, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        for (int j = sourceRank - 1; j < JSIZE; j += 4) {
          for (int i = 0; i < ISIZE; i++) {
            a[i * JSIZE + j] = receivedPart[i * JSIZE + j];
          }
        }
      }

      ff = fopen("result1paralelled.txt", "w");
      for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
          fprintf(ff, "%f ", a[i * JSIZE + j]);
        }
        fprintf(ff, "\n");
      }
      fclose(ff);
    }
  }

  MPI_Finalize();
  return 0;
}