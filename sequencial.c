#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(){
    clock_t start, end;
    double cpu_time_used;
    // Début du chronomètre
    start = clock();

    int n = 4000000;
    int *A = malloc(sizeof(int) * n);
    for(int i = 2; i < n; i++){
        A[i] = 1;
    }
    for(int i = 2; i < sqrt(n); i++){
        if(A[i] == 1){
            for(int j = i*i; j < n; j += i){
                A[j] = 0;
            }
        }
    }
    printf("Les nombres premiers sont : \n");
    for(int i = 2; i < n; i++){
        if(A[i] == 1){
            printf("%d\n", i);
        }
    }



    // Fin du chronomètre
    end = clock();

    // Calcul du temps écoulé en secondes
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Affichage du temps d'exécution
    printf("Temps d'exécution : %f secondes\n", cpu_time_used);
    return 0;
}