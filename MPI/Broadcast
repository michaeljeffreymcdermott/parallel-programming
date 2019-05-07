#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
void checkERR(int);
 
int main(int argc, char ** argv) {
	checkERR( MPI_Init(&argc, &argv) );
	int i, size, rank;
 
	checkERR( MPI_Comm_size(MPI_COMM_WORLD, &size) );
	checkERR( MPI_Comm_rank(MPI_COMM_WORLD, &rank) );
 
	int message;
 
	if(rank == 0) {
		message = 999;
	} else {
		message = -1;
	}
 
	printf("RANK(%d) before bcast message = %d!\n", rank, message);
	checkERR( MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD) );
	printf("RANK(%d) after bcast message = %d!\n", rank, message);
	fflush(stdout);
	MPI_Finalize();
	return 0;
 
}
 
void checkERR(int err) {
	if(err != 0)
		printf("ERR CODE: %d!\n", err);
}
