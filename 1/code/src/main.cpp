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
    
    int n;
    MPI_Init(&argc, &argv);

    std::shared_ptr<Factorial> factorial = std::shared_ptr<Factorial>(new Factorial(MPI_COMM_WORLD, std::stoi(std::string(argv[1]))));
    factorial->count();
	std::cout << "result: " << factorial->get_result() << std::endl;

    MPI_Finalize();
}