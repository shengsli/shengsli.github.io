// A well behaved version of "Hello World".
// The random sleep() just causes different 
// interleavings, but each thread always gets
// a distinct value of i.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define P 8

void *sayhello (void *id)
{  
  sleep(rand()%5);
  printf("Hello from thread %d\n", (int) id);
}

int main (int argc, char *argv[]) { 
  int i;
  pthread_t thread[P];
  time_t t;

  t = time(NULL);  // seed the random number
  srand((int) t);  // generator from outside

  printf("Hello from the main thread\n");
  for (i=0; i<P; i++) {
    pthread_create(&thread[i], NULL, sayhello, (void *)i);
  }

  for (i=0; i<P; i++) {
    pthread_join(thread[i], NULL);
  }
  printf("Goodbye from the main thread\n");
  exit(0);
}
