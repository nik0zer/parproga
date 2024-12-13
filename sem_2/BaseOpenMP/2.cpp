#include <iostream>
#include <omp.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "[Not enough command line args]" << std::endl;
    return 1;
  }

  long long n = std::stoll(argv[1]);
  double sum = 0.0;

  long long chunk_size = n / omp_get_max_threads();

  #pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    long long start = thread_id * chunk_size;
    long long end = (thread_id == omp_get_num_threads() -1) ? n : start + chunk_size; 
    double partial_sum = 0.0;

    for (long long i = start + 1; i <= end; ++i) {
      partial_sum += 1.0 / i;
    }

    #pragma omp critical
    {
      sum += partial_sum;
    }
  }

  std::cout.precision(5);
  std::cout << "[SUM] " << sum << std::endl;
  return 0;
}