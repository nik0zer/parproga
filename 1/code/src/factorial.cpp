#include "factorial.h"
#include <iostream>

void Factorial::count()
{
    MPI_Status status;
    unsigned long buffer;
    for (int i = _start_num; i <= _finish_num && i <= _n; i++)
    {
        _factorial_count *= i;
        
    }
    if(_rank == 0)
    {
        for(int i = 0; i < _size - 1; i++)
        {
            MPI_Recv(&buffer, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            _factorial_count *= buffer;
        }
    }
    else
    {
        MPI_Send(&_factorial_count, 1, MPI_UNSIGNED_LONG, 0, 0, _comm);
    }
    _result = _factorial_count;
    if(_rank == 0)
    {
        for(int i = 1; i < _size; i++)
        {
            MPI_Send(&_factorial_count, 1, MPI_UNSIGNED_LONG, i, 0, _comm);
        }
    }
    else
    {
        MPI_Recv(&buffer, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        _result = buffer;
    }
}

Factorial::Factorial(MPI_Comm comm, int n)
{
    _comm = comm;
    MPI_Comm_size(_comm, &_size);
	MPI_Comm_rank(_comm, &_rank);
    _n = n;
    _finish_num = 0;
    for(int i = 0; i <= _rank; i++)
    {
        _start_num = _finish_num + 1;
        if(i == _size - 1)
        {
            _finish_num = _n;
        }
        else
        {
            _finish_num = (_n - _start_num) / (_size - i) + _start_num;
        }
    }
}

unsigned long Factorial::get_result()
{
    return _result;
}