#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
void checkERR(int);
 
int main(int argc, char ** argv) {
	checkERR( MPI_Init(&argc, &argv) );
	int i, size, rank;
 
	checkERR( MPI_Comm_size(MPI_COMM_WORLD, &size) );
	checkERR( MPI_Comm_rank(MPI_COMM_WORLD, &rank) );
 
	MPI_Request request;
	MPI_Status status;
 
	int message;
 
	if(rank != 0) {
		checkERR( MPI_Irecv(&message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request) );
		checkERR( MPI_Wait(&request, &status) );	
		printf("rank(%d) RECV %d from rank(%d)\n", rank, message, 0);
		fflush(stdout);
	} else {
		for(i = 1; i < size; i++) {
			checkERR( MPI_Isend(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request) );
			checkERR( MPI_Wait(&request, &status) );	
			printf("rank(%d) SEND %d to rank(%d)\n", rank, i, i);
			fflush(stdout);
		}
	}		
	checkERR( MPI_Finalize() );
}
 
void checkERR(int err) {
	if(err != 0)
		printf("ERR CODE: %d!\n", err);
}
