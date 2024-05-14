#include <iostream>
#include "integration.h"

int main()
{
    double a = 0.01, b = 10000, error_rate = 0.001;
    int processes = 10;
    // std::cin >> processes >> a >> b >> error_rate;
    std::cout << my_integrate(processes, a, b, error_rate);
}