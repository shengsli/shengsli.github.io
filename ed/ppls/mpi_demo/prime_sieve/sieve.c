// An MPI implementation of the Sieve of Eratosthenes
//
// FIRST, READ THE INSTRUCTIONS IN
//
//    http://www.inf.ed.ac.uk/teaching/courses/ppls/mpiHowTo.html
//
// This program comes in two parts. You will also need the file generate.c
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

  MPI_Comm predComm, succComm; 
  MPI_Status status;  
  int myprime, candidate;

  int firstoutput = 1;  // a C style boolean

  MPI_Init (&argc, &argv); 
  MPI_Comm_get_parent (&predComm);

  // The first thing we receive is definitely prime (this
  // is a property of the algorithm)
  MPI_Recv(&myprime, 1, MPI_INT, 0, 0, predComm, &status);
  printf ("%d is a prime\n", myprime);

  // Now receive and handle a sequence of candidates
  MPI_Recv(&candidate, 1, MPI_INT, 0, 0, predComm, &status);
  while (candidate!=-1) {
    if (candidate%myprime) { // not sieved out
      if (firstoutput) {     // create my successor sieve if necessary
        MPI_Comm_spawn("sieve", argv, 1, MPI_INFO_NULL, 0, MPI_COMM_WORLD, 
                       &succComm, MPI_ERRCODES_IGNORE);
        firstoutput = 0;
      }
      MPI_Send(&candidate, 1, MPI_INT, 0, 0, succComm); // pass on the candidate
    }
    MPI_Recv(&candidate, 1, MPI_INT, 0, 0, predComm, &status); // get next candidate
  }

  // Pass on the "no more candidates" signal and shut down
  if (!firstoutput) MPI_Send(&candidate, 1, MPI_INT, 0, 0, succComm); 
  MPI_Finalize();
}

