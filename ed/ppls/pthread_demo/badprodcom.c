// A BROKEN simple producer/consumer program using semaphores and threads
//
// Taken from Greg Andrews' textbook website
// http://www.cs.arizona.edu/people/greg/mpdbook/programs/
//
// then BROKEN by taking out the semaphores!
//
// compile: gcc -lpthread prodcom.c -o prodcom
// run:     ./prodcom numIters  (eg ./prodcom 20)

 
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define SHARED 1

void *Producer(void *);  // the two threads
void *Consumer(void *);

sem_t empty, full;       // the global semaphores
int data;                // shared buffer
int numIters;

// main() :    read command line and create threads, then
//             print result when the threads have quit

int main(int argc, char *argv[]) {
  // thread ids and attributes
  pthread_t pid, cid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  numIters = atoi(argv[1]);
  sem_init(&empty, SHARED, 1);  // sem empty = 1
  sem_init(&full, SHARED, 0);   // sem full = 0 

  printf("running for %d iterations\n",numIters);
  printf("main started\n");
  pthread_create(&pid, &attr, Producer, NULL);
  pthread_create(&cid, &attr, Consumer, NULL);
  pthread_join(pid, NULL);
  pthread_join(cid, NULL);
  printf("main done\n");
}

// deposit 1, ..., numIters into the data buffer
void *Producer(void *arg) {
  int produced;
  printf("Producer created\n");
  for (produced = 1; produced <= numIters; produced++) {
    data = produced;
  }
}

// fetch numIters items from the buffer and sum them
void *Consumer(void *arg) {
  int total = 0, consumed;
  printf("Consumer created\n");
  for (consumed = 0; consumed < numIters; consumed++) {
    total = total+data;
  }
  printf("after %d iterations, the total is %d (should be %d)\n", numIters, total, numIters*(numIters+
1)/2);
}
