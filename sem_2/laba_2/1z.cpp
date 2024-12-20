#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

#define ISIZE 5000
#define JSIZE 5000
int main(int argc, char **argv) {
  constexpr auto length = (ISIZE + 1) * (JSIZE + 1) * sizeof(double);
  double *a = (double *)(mmap(NULL, length, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i * JSIZE + j] = 10 * i + j;
    }
  }
  //начало измерения времени
  auto start = std::chrono::high_resolution_clock::now();
  for (i = 2; i < ISIZE; i++) {
    for (j = 4; j < JSIZE; j++) {
      a[i * JSIZE + j] = sin(4 * a[(i - 2) * JSIZE + j - 4]);
    }
  }
  //окончание измерения времени
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "working time: " << static_cast<double>(duration.count()) << std::endl;
  ff = fopen("result1.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i * JSIZE + j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
}