#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 7

typedef struct {
    int n;
    int* A;
    int start;
    int end;
} ThreadArgs;

void* sieveOfEratosthenes(void* arg) {
    ThreadArgs* threadArgs = (ThreadArgs*)arg;
    int n = threadArgs->n;
    int* A = threadArgs->A;
    int start = threadArgs->start;
    int end = threadArgs->end;

    for (int i = 2; i * i < n; i++) {
        if (A[i] == 1) {
            for (int j = i * i; j < n; j += i) {
                if (j >= start && j < end) {
                    A[j] = 0;
                }
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    // Début du chronomètre
    start = clock();

    int n = 4000000;
    int* A = malloc(sizeof(int) * n);
    for (int i = 2; i < n; i++) {
        A[i] = 1;
    }

    int numThreads = MAX_THREADS; // Nombre de threads à utiliser
    pthread_t threads[MAX_THREADS];
    ThreadArgs threadArgs[MAX_THREADS];

    int range = (n - 2) / numThreads; // Taille de la plage gérée par chaque thread

    for (int i = 0; i < numThreads; i++) {
        threadArgs[i].n = n;
        threadArgs[i].A = A;
        threadArgs[i].start = 2 + i * range;
        threadArgs[i].end = (i == numThreads - 1) ? n : (2 + (i + 1) * range);

        int result = pthread_create(&threads[i], NULL, sieveOfEratosthenes, (void*)&threadArgs[i]);
        if (result != 0) {
            printf("Erreur lors de la création du thread.\n");
            return 1;
        }
    }

    for (int i = 0; i < numThreads; i++) {
        int result = pthread_join(threads[i], NULL);
        if (result != 0) {
            printf("Erreur lors de l'attente du thread.\n");
            return 1;
        }
    }

    printf("Les nombres premiers sont :\n");
    for (int i = 2; i < n; i++) {
        if (A[i] == 1) {
            printf("%d\n", i);
        }
    }

    free(A);

    // Fin du chronomètre
    end = clock();

    // Calcul du temps écoulé en secondes
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Affichage du temps d'exécution
    printf("Temps d'exécution : %f secondes\n", cpu_time_used);
    return 0;
}
