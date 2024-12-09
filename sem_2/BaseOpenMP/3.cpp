#include <iostream>
#include <omp.h>

int main() {
  int shared_var = 0;

  #pragma omp parallel
  {
    #pragma omp for ordered
    for (int i = 0; i < omp_get_num_threads(); ++i) {
      #pragma omp ordered
      {
        int thread_num = omp_get_thread_num();
        shared_var++;
        std::cout << "[THREAD] " << thread_num << " [VAR] " << shared_var << std::endl;
      }
    }
  }
  return 0;
}