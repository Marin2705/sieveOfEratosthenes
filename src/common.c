#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"

void printArray(size_t n, char* A) {
  printf("[");
  for (size_t i = 2; i < n; i++) {
    if (A[i] == 1) {
      printf("%Iu,", i);
    }
  }
  printf("]\n");
}

char* initArray(size_t n) {
  char* A = (char *)malloc(sizeof(char) * n);
  for (size_t i = 2; i < n; i++)
    A[i] = 1;
  return A;
}

void sequancialSieveOfEratosthenes(size_t n, char* A) {
  for (size_t i = 2; i < sqrt(n); i++) {
    if (A[i] == 1) {
      for (size_t j = i * i; j < n; j += i) {
        A[j] = 0;
      }
    }
  }
}
