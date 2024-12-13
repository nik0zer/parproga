#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

// #define B_PARALLEL
#define ALG_PARALLEL

double f(double y) { return exp(y); }

double df_dy(double y) { return exp(y); }

// Метод прогонки
std::vector<double> tridiagonal_matrix_algorithm(const std::vector<double> &a, const std::vector<double> &b,
                                            const std::vector<double> &c,
                                            const std::vector<double> &d) {
  int n = d.size();
  std::vector<double> x(n);
  std::vector<double> alpha(n), beta(n);

  alpha[0] = -c[0] / b[0];
  beta[0] = d[0] / b[0];

  for (int i = 1; i < n - 1; ++i) {
    double denominator = b[i] + a[i] * alpha[i - 1];
    alpha[i] = -c[i] / denominator;
    beta[i] = (d[i] - a[i] * beta[i - 1]) / denominator;
  }

  beta[n - 1] = (d[n - 1] - a[n - 1] * beta[n - 2]) /
                (b[n - 1] + a[n - 1] * alpha[n - 2]);

  x[n - 1] = beta[n - 1];

  for (int i = n - 2; i >= 0; --i) {
    x[i] = alpha[i] * x[i + 1] + beta[i];
  }
  return x;
}


std::vector<double> solve_boundary_value_problem(double b, int num_points) {
  double a = 1.0;
  double h = 1.0 / (num_points - 1);

  std::vector<double> y(num_points, 0.0);

  for (int i = 0; i < num_points; ++i) {
    y[i] = a + (b - a) * i / (num_points - 1);
  }

  double tol = 0.0001;

  while(true) {
    std::vector<double> A(num_points, 0.0);
    std::vector<double> B(num_points, 0.0);
    std::vector<double> C(num_points, 0.0);
    std::vector<double> D(num_points, 0.0);

    A[0] = 0;
    B[0] = 1;
    C[0] = 0;
    D[0] = a;
    A[num_points - 1] = 0;
    B[num_points - 1] = 1;
    C[num_points - 1] = 0;
    D[num_points - 1] = b;

#ifdef ALG_PARALLEL
    #pragma omp parallel for
#endif
    for (int i = 1; i < num_points - 1; ++i) {
      double f_i = f(y[i]);
      double df_i = df_dy(y[i]);
      double f_im1 =
          (i - 1 >= 0) ? f(y[i - 1]) : f(a); //  Граничное условие y(0)=1
      double f_ip1 = (i + 1 < num_points) ? f(y[i + 1])
                                          : f(b); //  Граничное условие y(1)=b

      A[i] = 1.0 - (h * h / 12.0) * df_i - (h * h / 12.0) * df_i;
      B[i] = -2.0 - (10.0 / 12.0) * h * h * df_i;
      C[i] = 1.0 - (h * h / 12.0) * df_i - (h * h / 12.0) * df_i;

      D[i] = h * h * (f_i + (1.0 / 12.0) * (f_ip1 - 2.0 * f_i + f_im1)) -
             y[i] * df_i * h * h * (1.0 + 10.0 / 12.0) -
             y[i - 1] * df_i * (h * h / 12.0) -
             y[i + 1] * df_i * (h * h / 12.0) -
             (A[i] * y[i - 1] + B[i] * y[i] + C[i] * y[i + 1]);

      A[i] = 1.0 - (h * h / 12.0) * df_i - (h * h / 12.0) * df_i;
      B[i] = -2.0 - (10.0 / 12.0) * h * h * df_i;
      C[i] = 1.0 - (h * h / 12.0) * df_i - (h * h / 12.0) * df_i;

      D[i] = h * h * (f_i + (1.0 / 12.0) * (f_ip1 - 2.0 * f_i + f_im1)) -
             y[i] * df_i * h * h * (1.0 + 10.0 / 12.0) -
             y[i - 1] * df_i * (h * h / 12.0) -
             y[i + 1] * df_i * (h * h / 12.0) -
             (A[i] * y[i - 1] + B[i] * y[i] + C[i] * y[i + 1]);
    }

    std::vector<double> y_new = tridiagonal_matrix_algorithm(A, B, C, D);
    double max_diff = 0.0;
    
#ifdef ALG_PARALLEL
    #pragma omp parallel for
#endif
    for (int i = 0; i < num_points; ++i) {
      double diff = std::abs(y_new[i] - y[i]);
      if (diff > max_diff) {
#ifdef ALG_PARALLEL
    #pragma omp critical
    {
#endif        
        max_diff = diff;
#ifdef ALG_PARALLEL
    }
#endif
      }
      y[i] = y_new[i];
    }
    if (max_diff < tol)
      break;
  }

  return y;
}

int main(int argc, char **argv) {
  int num_points_min = 400;
  int num_points_max = 4000;
  int num_points = num_points_max;
  int workers = 4;
  if(argc > 1)
  {
    workers = std::atoi(argv[1]);
  }
  else
  {
    workers = 4;
  }
  omp_set_num_threads(workers);
  double step = 0.1;

  std::cout << std::setprecision(7) << std::fixed;

  auto start = std::chrono::high_resolution_clock::now();

#ifdef B_PARALLEL
  #pragma omp parallel for
#endif
  for (int i = 0; i <= 10; i += 1) {
    double b = i * step;
    std::vector<double> solution = solve_boundary_value_problem(b, num_points);

#ifdef B_PARALLEL
    #pragma omp critical
    {
#endif
    std::cout << "Solution for b = " << b << " and " << num_points
         << " points: " << std::endl;
    for (int i = 0; i < solution.size(); i += solution.size() / 10) {
      std::cout << "y(" << (double)i / (solution.size() - 1) << ") = " << solution[i]
           << std::endl;

    }
    std::cout << "----------------" << std::endl;
#ifdef B_PARALLEL
    }
#endif
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "working time: " << static_cast<double>(duration.count()) << std::endl;

  return 0;
}