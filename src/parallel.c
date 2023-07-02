#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

#define MAX_THREADS 16

typedef struct {
  size_t id;
  char* A;
  size_t* i;
  size_t* start;
  size_t* end;
} ThreadArgs;

void* sieveOfEratosthenes(void* arg) {
  ThreadArgs* threadArgs = (ThreadArgs*)arg;
  char* A = threadArgs->A;
  size_t id = threadArgs->id;

  while (1) {
    P(0);  // Redemander leurs depart
    size_t i = *threadArgs->i;
    size_t start = *threadArgs->start;
    size_t end = *threadArgs->end;
    if (end == 0) break;
    for (size_t j = start; j < end; j += i) {
      A[j] = 0;
    }
    V(1);  // signaler au père que le traitement est finis
    P(2);  // attendre que tous les traitement soit finis (eviter qu'un fils
           // vole P(0) à un autre)
    V(3);  // Qu'il on reprit
  }

  pthread_exit(NULL);
  return NULL;
}

int main() {
  // Début du chronomètre
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  struct timeval stop, start;
  gettimeofday(&start, NULL);

  char* A = initArray(N);
  pthread_t threads[MAX_THREADS];
  ThreadArgs threadArgs[MAX_THREADS];
  size_t* dataI = calloc(MAX_THREADS, sizeof(size_t));
  size_t* dataStart = calloc(MAX_THREADS, sizeof(size_t));
  size_t* dataEnd = calloc(MAX_THREADS, sizeof(size_t));

  init_semaphore();
  val_sem(0, 0);
  val_sem(1, 0);
  val_sem(2, 0);
  val_sem(3, 0);

  for (size_t t = 0; t < MAX_THREADS; t++) {
    threadArgs[t].id = t;
    threadArgs[t].A = A;
    threadArgs[t].i = dataI + t;
    threadArgs[t].start = dataStart + t;
    threadArgs[t].end = dataEnd + t;
    int result = pthread_create(threads + t, NULL, sieveOfEratosthenes,
                                (void*)(threadArgs + t));
    if (result != 0) {
      perror("Erreur lors de la création du thread.\n");
      return 1;
    }
  }

  for (size_t i = 2; i < (size_t)sqrt(N); i++) {
    if (A[i] == 1) {
      size_t i2 = i * i;
      size_t step = (N - i2) / MAX_THREADS + 1;
      for (size_t t = 0; t < MAX_THREADS; t++) {
        /* Mise à jour des arguments */
        *threadArgs[t].i = i;
        *threadArgs[t].start = i2 + t * step;
        *threadArgs[t].end = i2 + (t + 1) * step;
        if (*threadArgs[t].end > N) {
          *threadArgs[t].end = N;
        }
      }
      for (size_t t = 0; t < MAX_THREADS; t++) {
        V(0);
      }
      for (size_t t = 0; t < MAX_THREADS; t++) {
        P(1);  // Attendre que les fils ai finis
      }
      for (size_t t = 0; t < MAX_THREADS; t++) {
        V(2);  // Terminer leurs traitement
      }
      for (size_t t = 0; t < MAX_THREADS; t++) {
        P(3);  // Terminer leurs traitement
      }
    }
  }

  for (size_t t = 0; t < MAX_THREADS; t++) {
    *(dataEnd + t) = 0;
  }

  for (size_t t = 0; t < MAX_THREADS; t++) {
    V(0);
  }

  for (size_t t = 0; t < MAX_THREADS; t++) {
    int result = pthread_join(threads[t], NULL);
    if (result != 0) {
      perror("Erreur lors de l'attente du thread.\n");
      return 1;
    }
  }
  detruire_semaphore();
  // Fin du chronomètre

  gettimeofday(&stop, NULL);

  printf("Les nombres premiers sont :\n");
  // printArray(N, A);
  free(A);

  // Calculate the duration in seconds
  float duration = timeval_diff_seconds(start, stop);

  // Output the result
  printf("Duration: %.6f seconds.\n", duration);

  return 0;
}
