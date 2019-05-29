// This version of "Hello World" doesn't behave! The problem
// is that the "id" parameter is passed to the thread by 
// reference (ie as a pointer to its true location. But 
// that location is shared, and is being updated all the
// time by the loops in the main function. Thus
// the value actually picked up by sayhello depends upon 
// the interleaving of these events. The sleep() just
// encourages the effect.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define P 100

void *sayhello (void *id)
{
  sleep(rand()%5);
  printf("Hello from thread %d\n", *(int *)id);
}

int main (int argc, char *argv[]) { 
  int i;
  time_t t;
  pthread_t thread[P];

  t = time(NULL);  // seed the random number
  srand((int) t);  // geberator from outside

  printf("Hello from the main thread\n");
  for (i=0; i<P; i++) {
    pthread_create(&thread[i], NULL, sayhello, &i);
  }

  for (i=0; i<P; i++) {
    pthread_join(thread[i], NULL);
  }
  printf("Goodbye from the main thread\n");
  exit(0);
}
  
