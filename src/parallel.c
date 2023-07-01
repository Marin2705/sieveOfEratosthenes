#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

#define MAX_THREADS 7

typedef struct {
  size_t n;
  char* A;
  size_t* i;
  size_t plage;
  size_t numThreads;
} ThreadArgs;

void* sieveOfEratosthenes(void* arg) {
  size_t i = *threadArgs->i;
  ThreadArgs* threadArgs = (ThreadArgs*)arg;
  size_t n = threadArgs->n;
  char* A = threadArgs->A;
  size_t plage = threadArgs->plage;
  size_t numThreads = threadArgs->numThreads;

  while (i) {
    size_t i = *threadArgs->i;
    for (size_t j = i * i + plage; j < n; j += i * numThreads) {
      A[j] = 0;
    }

  }

  pthread_exit(NULL);
  return NULL;
}

int main() {
  clock_t start, end;
  double cpu_time_used;
  // Début du chronomètre
  start = clock();

  size_t n = N;
  char* A = initArray(n);
  size_t numThreads = MAX_THREADS;  // Nombre de threads à utiliser
  pthread_t threads[MAX_THREADS];
  ThreadArgs threadArgs[MAX_THREADS];

  size_t range =
      (n - 2) / numThreads;  // Taille de la plage gérée par chaque thread

  for (size_t i = 0; i < numThreads; i++) {
    threadArgs[i].n = n;
    threadArgs[i].A = A;
    threadArgs[i].plage = i;
    threadArgs[i].numThreads = numThreads;

    int result = pthread_create(&threads[i], NULL, sieveOfEratosthenes,
                                (void*)&threadArgs[i]);
    if (result != 0) {
      printf("Erreur lors de la création du thread.\n");
      return 1;
    }
  }

  for (size_t i = 0; i < numThreads; i++) {
    int result = pthread_join(threads[i], NULL);
    if (result != 0) {
      printf("Erreur lors de l'attente du thread.\n");
      return 1;
    }
  }

  // Fin du chronomètre
  end = clock();

  printf("Les nombres premiers sont :\n");
  printArray(n, A);

  free(A);

  // Calcul du temps écoulé en secondes
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  // Affichage du temps d'exécution
  printf("Temps d'exécution : %f secondes\n", cpu_time_used);
  return 0;
}
