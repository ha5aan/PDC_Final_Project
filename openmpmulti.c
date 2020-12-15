#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>


#define N 1000

int matrix_1[N][N];
int matrix_2[N][N];
int Result[N][N];

int main() 
{   
    srand(time(0));
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
    //#pragma omp_set_num_threads(10)
    #pragma omp parallel for private(i,j,k) shared(matrix_1,matrix_2,Result) num_threads(10)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                Result[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }

#pragma omp barrier
    clock_t end = clock(); 
      double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
     
    // for (i= 0; i< N; i++)
    // {
    //     for (j= 0; j< N; j++)
    //     {
    //         printf(" %d ",Result[i][j]);
    //     }
    //     printf("\n");
    // }
     printf("The time taken to compete matrix multiplication using Shared memory is :\n %f\n\n", time_spent);

}