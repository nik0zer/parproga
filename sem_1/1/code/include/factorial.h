#ifndef FACTORIAL_H
#define FACTORIAL_H
#include <mpi.h>

class Factorial
{
private:
    unsigned long _factorial_count = 1;
    int _start_num;
    int _finish_num;
    int _n;
    unsigned long _result;

    MPI_Comm _comm;
    int _rank;
    int _size;
    
public:
    void count();
    unsigned long get_result();
    Factorial(MPI_Comm comm, int n);
};



#endif