#ifndef __COMMON_H
#define __COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define N_SEM 3
#define N 100000

int init_semaphore(void);
int detruire_semaphore(void);
int val_sem(int, int);
int P(int);
int V(int);
void printArray(size_t, char*);
char* initArray(size_t);
void sequancialSieveOfEratosthenes(size_t, char*);
#endif
