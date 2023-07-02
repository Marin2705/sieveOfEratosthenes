#include "common.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

// Function to calculate the time difference in seconds
float timeval_diff_seconds(struct timeval start, struct timeval stop) {
  // Calculate the difference in microseconds
  long long diff_usec =
      (stop.tv_sec - start.tv_sec) * 1000000LL + (stop.tv_usec - start.tv_usec);

  // Convert microseconds to seconds and return the result
  return diff_usec / 1000000.0f;
}

static int semid = -1;

/*
   Crée un groupe de N_SEM sémaphores utilisables dans le processus qui
   fait l'appel et dans ses fils.
   Cette fonction devra renvoyer 0 en cas de réussite, -1 si elle
   a déjà été appelée, -2 en cas d'échec de création.
*/
int init_semaphore() {
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;
  arg.val = 0;

  if (semid != -1) {
    perror("[init_semaphore] déjà initialisé");
    return -1;
  }
  if ((semid = semget(IPC_PRIVATE, N_SEM, 0666)) == -1) {
    perror("[init_semaphore] semget échoue");
    return -2;
  }

  for (int i = 0; i < N_SEM; i++)
    if (semctl(semid, i, SETVAL, arg) == -1)
      perror("[init_semaphore] semctl échoue");
  return 0;
}

/*
   Détruit le groupe de sémaphores créé par "init_semaphore",
   en fin de programme.
   Retourne -1 si "init_semaphore" n'a pas été appelé avant,
   la valeur de retour de "semctl" en cas de réussite
*/
int detruire_semaphore() {
  if (semid == -1) {
    perror("[detruire_semaphore] non initialisé");
    return -1;
  }
  int ret = semctl(semid, 0, IPC_RMID, 0);
  if (ret == -1) perror("[detruire_semaphore] semctl échoue");
  semid = -1;
  return ret;
}

/*
   Attribue la valeur "val" au sémaphore "sem" du groupe de sémaphores créé par
   "init_semaphore".
   Retourne la valeur de retour de "semctl" en cas de réussite,
   -1 si "init_semaphore" n'a pas été appelé avant,
   -2 si le numéro de sémaphore est incorrect.
*/
int val_sem(int sem, int val) {
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;
  arg.val = val;

  if (semid == -1) {
    perror("[val_sem] non initialisé");
    return -1;
  }
  if (sem < 0 || sem >= N_SEM) {
    perror("[val_sem] identifiant invalide");
    return -2;
  }
  int ret = semctl(semid, sem, SETVAL, arg);
  if (ret == -1) perror("[val_sem] semctl échoue");
  return ret;
}

/*
   Réalise l'opération (P) sur le sémaphore numéro "sem" du groupe
   de sémaphores créé par "init_semaphore".
   Retourne la valeur de retour de "semop" en cas de réussite,
   -1 si "init_semaphore" n'a pas été appelé avant,
   -2 si le numéro de sémaphore est incorrect.
*/

int P(int sem) {
  if (semid == -1) {
    perror("[P] non initialisé");
    return -1;
  }
  if (sem < 0 || sem >= N_SEM) {
    perror("[P] identifiant invalide");
    return -2;
  }
  struct sembuf op_P = {sem, -1, 0};
  int ret = semop(semid, &op_P, 1);
  if (ret == -1) perror("[P] semop échoue");
  return ret;
}

int V(int sem) {
  if (semid == -1) {
    perror("[V] non initialisé");
    return -1;
  }
  if (sem < 0 || sem > N_SEM) {
    perror("[V] identifiant invalide");
    return -2;
  }

  struct sembuf op_V = {sem, 1, 0};
  int ret = semop(semid, &op_V, 1);
  if (ret == -1) perror("[P] semop échoue");
  return ret;
}

void printArray(size_t n, char *A) {
  printf("[");
  for (size_t i = 2; i < n; i++) {
    if (A[i] == 1) {
      printf("%zu,", i);
    }
  }
  printf("]\n");
}

char *initArray(size_t n) {
  char *A = (char *)malloc(sizeof(char) * n);
  for (size_t i = 2; i < n; i++) A[i] = 1;
  return A;
}

void sequancialSieveOfEratosthenes(size_t n, char *A) {
  for (size_t i = 2; i < sqrt(n); i++) {
    if (A[i] == 1) {
      for (size_t j = i * i; j < n; j += i) {
        A[j] = 0;
      }
    }
  }
}
