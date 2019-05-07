#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
void checkERR(int);
 
int main(int argc, char ** argv) {
	checkERR( MPI_Init(&argc, &argv) );
	int i, size, rank;
 
	checkERR( MPI_Comm_size(MPI_COMM_WORLD, &size) );
	checkERR( MPI_Comm_rank(MPI_COMM_WORLD, &rank) );
 
	int recv_buff = -1, *send_buff = NULL;
	int src = 0; 
	/* 
	   this is the normal, rank 0 is the src here, so only rank 0 gets 
	   an allocated array! nobody else does. this is the send buffer
	   everyone gets a recv buffer, but its also nonsensical at the moment
	*/
	if(rank == src) {
		/* allocate/populate the send buffer */
		send_buff = (int *)malloc(sizeof(int) * size);
		for(i = 0; i < size; i++) {
			send_buff[i] = 2*i;
		}
		printf("Printing the send_buff from RANK[%d]\n", src);
		for(i = 0; i < size; i++) {
			printf("%d, ", send_buff[i]);
		}
		printf("\n");
	}
 
	checkERR( MPI_Scatter(send_buff, 1, MPI_INT, 
		                  &recv_buff, 1, MPI_INT,
		                  src, MPI_COMM_WORLD) );
 
	printf("RANK[%d] now has %d in its recv_buff!\n", rank, recv_buff);
	fflush(stdout);
	recv_buff = rank; /* change the buffer */
 
	checkERR( MPI_Gather(&recv_buff, 1, MPI_INT,
		                 send_buff, 1, MPI_INT,
		                 src, MPI_COMM_WORLD) );
	/* notice the changes above */
 
	if(rank == src) {
		printf("Printing the new send_buff from RANK[%d]\n", src);
		for(i = 0; i < size; i++)
			printf("%d, ", send_buff[i]);
		printf("\n");
	}
 
	if(rank == src) {
		free(send_buff); /* always free allocated memory! */
	}
 
	MPI_Finalize();
	return 0;
}
 
void checkERR(int err) {
	if(err != 0)
		printf("ERR CODE: %d!\n", err);
}
