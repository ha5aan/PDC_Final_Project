#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#define N               3   /* number of rows and columns 
                               in matrix ,for number of processes 
                              one greater than matrix size */

MPI_Status status;

double matrix_1[N][N],matrix_2[N][N],matrix_res[N][N];

int main(int argc, char **argv)
{
  int numtasks,taskid,numworkers,source,dest,rows,offset,i,j,k,temp;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  numworkers = numtasks-1;

  /*---------------------------- master ----------------------------*/
  if (taskid == 0) {
    printf("Enter values for matrix one: \n");
    for (i=0; i<N; i++) {
      for (j=0,temp=0; j<N; j++) {
        printf("enter value for[%d][%d] ",i+1,j+1);
        scanf("%d", &temp );
        matrix_1[i][j]= temp;
        printf("\n");
      }
    }

    printf("Enter values for matrix two: \n");
    for (i=0; i<N; i++) {
      for (j=0,temp=0; j<N; j++) {
        printf("enter value for[%d][%d] ",i+1,j+1);
        scanf("%d", &temp );
        matrix_2[i][j]= temp;
        printf("\n");
      }
    }
 clock_t begin = clock(); 
    /* send matrix data to the worker tasks */
    rows = N/numworkers;
    offset = 0;

    for (dest=1; dest<=numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_1[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      MPI_Send(&matrix_2, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* wait for results from all worker tasks */
    for (i=1; i<=numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_res[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

    printf("Here is the result matrix:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++)
        printf("%6.2f   ", matrix_res[i][j]);
      printf ("\n");
    }

    	clock_t end = clock(); 
      double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      printf("The time taken to compete matrix multiplication using Message Passing is :\n %f\n\n", time_spent);

  }

  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_1, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_2, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k=0; k<N; k++)
      for (i=0; i<rows; i++) {
        matrix_res[i][k] = 0.0;
        for (j=0; j<N; j++)
          matrix_res[i][k] = matrix_res[i][k] + matrix_1[i][j] * matrix_2[j][k];
      }


    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_res, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();

 return 0;  
}
