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
  int N;
  int rank, tag, origin, destination;
  MPI_Status status;
  
  char hostname[1024];
  gethostname(hostname, 1024);
  sscanf(argv[1], "%d", &N);
  //printf("how many times?  %d \n",N);
  
  int message_out =0;
  int message_in = 0;

  //MPI_Request request_out, request_in;
  /* timing */
  timestamp_type time1, time2;
  get_timestamp(&time1);
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  tag = 99;
  //process #0 initialize the comunicatioin
  
  for(int i=0; i< N; i++){   
    //MPI_Irecv(message_in,  1, MPI_INT, origin,      tag, MPI_COMM_WORLD, &request_in);
     if(rank ==0){
        MPI_Send(&message_out, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
   	MPI_Recv(&message_in,  1, MPI_INT, size-1,      tag, MPI_COMM_WORLD, &status);
	printf("\n rank %d hosted on %s received from %d the message %d \n", rank, hostname, size-1, message_in);
	message_out = message_in;
    }else{       
       origin = (rank - 1) % size;
       MPI_Recv(&message_in,  1, MPI_INT, origin,      tag, MPI_COMM_WORLD, &status);
       message_out = message_in+ rank;
       destination = (rank  + 1) % size;
       MPI_Send(&message_out, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
       printf("\n rank %d hosted on %s received from %d the message %d \n", rank, hostname, origin, message_in);
      }
    //MPI_Isend(message_out, 1, MPI_INT, destination, tag, MPI_COMM_WORLD, &request_out);
  
  
  }
  MPI_Finalize();
  if(rank ==0){
    if(message_in >= 0.5*N*size*(size-1)){
      printf("All of the threads added their rank correctly in each iteration: final message = %d \n",message_in );
    }else{
      printf("Not every thread addes their rank...\n");
    }
  }
  
  /* timing */
  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  double latency = elapsed/N;
  printf("The latency  is %f seconds.\n", latency);
  return 0;
}
