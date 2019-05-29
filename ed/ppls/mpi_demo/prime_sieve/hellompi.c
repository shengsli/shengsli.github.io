// An MPI implementation of the Sieve of Eratosthenes
//
// FIRST, READ THE INSTRUCTIONS IN
//
//    http://www.inf.ed.ac.uk/teaching/courses/ppls/mpiHowTo.html
//
// This program comes in two parts. You will also need the file sieve.c
//
// To compile do
//
//    [mymachine]: mpicc -o generate generate.c
//    [mymachine]: mpicc -o sieve sieve.c
//
// and to run the program do
//
//    [mymachine]: mpirun --map-by socket:OVERSUBSCRIBE ./generate 25
// 
// where the number (here 25) gives the upper bound on the region
// within which to search for primes.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Comm nextComm;                    // this will house the first sieve
  int candidate = 2, N = atoi(argv[1]);  

  MPI_Init(&argc, &argv);         

  // Create the first sieve
  MPI_Comm_spawn("sieve", argv, 1, MPI_INFO_NULL, 0, 
                 MPI_COMM_WORLD, &nextComm, MPI_ERRCODES_IGNORE);

  // Generate the sequence of candidates
  while (candidate<N) {
    MPI_Send(&candidate, 1, MPI_INT, 0, 0, nextComm);
    candidate++;
  }

  // Send the "no more candidates" signal
  candidate = -1;
  MPI_Send(&candidate, 1, MPI_INT, 0, 0, nextComm);
  MPI_Finalize();
}

