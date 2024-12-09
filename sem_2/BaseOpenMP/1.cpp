#include <iostream>
#include <omp.h>

int main() {
  #pragma omp parallel
  {
    auto thread_num = omp_get_thread_num();
    auto num_threads = omp_get_num_threads();
    auto out_str = std::string("Hello World") + "[THREAD] " + 
    std::to_string(thread_num) + " [MAX_THREADS] " + std::to_string(num_threads);
    #pragma omp critical
    {
        std::cout << out_str << std::endl;
    }
  }
  return 0;
}