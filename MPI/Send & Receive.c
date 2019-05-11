#include <stdio.h>
#include <mpi.h>
 
/*
checkERR() is just a helper function that I use here to print out any error codes, etc that are thrown
by the MPI functions. You can omit this from your code if you want to or are handling this in a different way.
 
You will see something like this again when we start working on GPU code. It is a good practice to get into somehow 
regardless of how you decide to do it.
*/
void checkERR(int);
 
int main(int argc, char ** argv) {
	checkERR( MPI_Init(&argc, &argv) );
	int i, size, pid;
 
	checkERR( MPI_Comm_size(MPI_COMM_WORLD, &size) );
	checkERR( MPI_Comm_rank(MPI_COMM_WORLD, &pid) );
 
	int message;
	for(i = 1; i < size; i++) {
		if (pid == 0) { /* send from master */
			message = -1 * i;
			/* 
				send(data, count, type, dest, tag, communicator)
			*/
			checkERR( MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD) );
			printf("pid(%d) SEND %d to pid(%d)\n", 0, message, i);
			fflush(stdout);
		} else if (pid == i) { /* receive at slaves */
			/*
				recv(data, count, type, source, tag, communcator, status)
			*/
			checkERR( MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) );
			printf("pid(%d) RECV %d from pid(%d)\n", i, message, 0);
			fflush(stdout);
		}
	}
	checkERR( MPI_Finalize() );
}
 
void checkERR(int err) {
	if(err != 0)
		printf("ERR CODE: %d!\n", err);
}
