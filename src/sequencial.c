#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main() {
  // Initialisation
  struct timeval stop, start;
  size_t n = N;
  char* A = initArray(n);

  // Début du chronomètre
  gettimeofday(&start, NULL);

  // Lancement de l'algorithme
  sequancialSieveOfEratosthenes(n, A);

  // Fin du chronomètre

  gettimeofday(&stop, NULL);

  // Affichage des nombres
  printf("Les nombres premiers sont : \n");
  // printArray(n, A);
  free(A);

  // Calculate the duration in seconds
  float duration = timeval_diff_seconds(start, stop);

  // Output the result
  printf("Duration: %.6f seconds.\n", duration);

  return 0;
}
