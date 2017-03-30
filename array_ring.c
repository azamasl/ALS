/* Communication ping-pong:
 * Exchange between messages between mpirank
 * 0 <-> 1, 2 <-> 3, ....
 */
#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include <mpi.h>

int main( int argc, char *argv[])
{
  int size;// # of the processes
  //int N;
  int rank, tag, origin, destination;
  MPI_Status status;
  
  //char hostname[1024];
  //gethostname(hostname, 1024);
  // sscanf(argv[1], "%d", &N);
  //printf("how many times?  %d \n",N);

  //int size on my machine is 4 bytes so 2MB memorry translates to int array of size 0.5M
  int arraysize=  500000;  
  int *message_out = calloc(arraysize, sizeof(int));
  int  *message_in = calloc(arraysize, sizeof(int)); 

  for(int n = 0; n < arraysize; ++n)
    message_out[n] = rank;
    
  /* timing */
  timestamp_type time1, time2;
  get_timestamp(&time1);
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  tag = 99;
  //process #0 initialize the comunicatioin
  
     if(rank ==0){
        MPI_Send(message_out, arraysize, MPI_INT, 1, tag, MPI_COMM_WORLD);
   	MPI_Recv(message_in,  arraysize, MPI_INT, size-1,      tag, MPI_COMM_WORLD, &status);
	printf("\n rank %d received from %d the message: \n", rank,  size-1);
	//for(int n = 0; n < arraysize; ++n)
	//printf("  %d\n", message_in[n]);

    }else{       
       origin = (rank - 1) % size;
       MPI_Recv(message_in,  arraysize, MPI_INT, origin,      tag, MPI_COMM_WORLD, &status);
       destination = (rank  + 1) % size;
       MPI_Send(message_out, arraysize, MPI_INT, destination, tag, MPI_COMM_WORLD);

       	printf("\n rank %d received from %d the message: \n", rank,  origin);
	//for(int n = 0; n < arraysize; ++n)
	//fprintf("  %d\n", message_in[n]);
      }

  MPI_Finalize();
    /* Clean up */
  free(message_in);
  free(message_out);
  /* timing */
  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  double bandwidth = 2/elapsed;
  printf("The bandwidth  is %f MB/seconds.\n", bandwidth);
  return 0;
}
