#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define N 1000
int matrix_1[N][N];
int matrix_2[N][N];
int Result[N][N];

void multiplyMatrices() {
   for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
         Result[i][j] = 0;
      }
   }

      for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
         for (int k = 0; k < N; ++k) {
            Result[i][j] += matrix_1[i][k] * matrix_2[k][j];
         }
      }
   }
}


void display() {

   printf("\nOutput Matrix:\n");
   for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
         printf("%d  ", Result[i][j]);
         
            
      }
      printf("\n");
   }
}

int main() {

 srand(time(NULL));
    int i,j,k,temp;
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{   
         temp=rand()%100;
            matrix_1[i][j]= temp;
            temp=rand()%100;
            matrix_2[i][j] = temp;
	}
     clock_t begin = clock(); 

 
   multiplyMatrices();
    
    clock_t end = clock(); 
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
     display();
     printf("The time taken to compete matrix multiplication using sequential approach is :\n %f\n\n", time_spent);

 
  

   return 0;
}