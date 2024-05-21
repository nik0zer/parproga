#include <iostream>
#include "integration.h"

int main()
{
    double a = 0.00001, b = 10000, error_rate = 0.0000001;
    int processes = 7;
    // std::cin >> processes >> a >> b >> error_rate;
    std::cout << my_integrate(processes, a, b, error_rate);
}