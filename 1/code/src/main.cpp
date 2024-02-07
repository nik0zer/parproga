#include <iostream>
#include <memory>
#include <string>
#include "factorial.h"

int main(int argc, char **argv)
{	
    if(argc <= 1)
    {
        return -1;
    }
    
    int n, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::shared_ptr<Factorial> factorial = std::shared_ptr<Factorial>(new Factorial(MPI_COMM_WORLD, std::stoi(std::string(argv[1]))));
    factorial->count();

    if(rank == 0)
    {
        std::cout << "result: " << factorial->get_result() << std::endl;
    }

    MPI_Finalize();
}