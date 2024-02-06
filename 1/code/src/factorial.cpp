#include "factorial.h"
#include <iostream>

void Factorial::count()
{
    MPI_Status status;
    int buffer;
    for (int i = _start_num; i <= _finish_num; i++)
    {
        _factorial_count *= i;
    }
    if(_rank == 0)
    {
        for(int i = 0; i < _size - 1; i++)
        {
            MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            _factorial_count *= buffer;
        }
    }
    else
    {
        MPI_Send(&_factorial_count, 1, MPI_INT, 0, 0, _comm);
    }
    _result = _factorial_count;
    if(_rank == 0)
    {
        for(int i = 1; i < _size; i++)
        {
            MPI_Send(&_factorial_count, 1, MPI_INT, i, 0, _comm);
        }
    }
    else
    {
        MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        _result = buffer;
    }
}

Factorial::Factorial(MPI_Comm comm, int n)
{
    _comm = comm;
    MPI_Comm_size(_comm, &_size);
	MPI_Comm_rank(_comm, &_rank);
    _n = n;
    if(_rank == 0)
    {
        _start_num = 1;
    }
    else
    {
        _start_num = (_n - _n % _size + _size) / _size * (_rank);
    }

    if(_rank == _size - 1)
    {
        _finish_num = _n;
    }
    else
    {
        _finish_num = (_n - _n % _size + _size) / _size * (_rank + 1) - 1;
    }
}

int Factorial::get_result()
{
    return _result;
}