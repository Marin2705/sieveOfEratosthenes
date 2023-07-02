#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

int main() {
  // Initialisation
  clock_t start, end;
  double cpu_time_used;
  size_t n = N;
  char* A = initArray(n);

  // Début du chronomètre
  start = clock();

  // Lancement de l'algorithme
  sequancialSieveOfEratosthenes(n, A);

  // Fin du chronomètre
  end = clock();

  // Affichage des nombres
  printf("Les nombres premiers sont : \n");
  printArray(n, A);
  free(A);

  // Calcul du temps écoulé en secondes
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  // Affichage du temps d'exécution
  printf("Temps d'execution : %f secondes\n", cpu_time_used);
  return 0;
}
