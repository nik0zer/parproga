#include <stdio.h>
#include <mpi.h>

#define BUF_SIZE 2

int main (int argc, char ** argv)
{

int size = 0, rank = 0;

int sbuf[BUF_SIZE];

int rbuf[BUF_SIZE];

MPI_Init (&argc, &argv);

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

sbuf[0] = rank;

sbuf[1] = -rank - 2;

MPI_Reduce(sbuf, rbuf, BUF_SIZE, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

if (rank == 0)
{
printf ("%d %d\n", rbuf[0], rbuf[1]);
}
MPI_Finalize();
return 0;
}
