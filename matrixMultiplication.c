#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <time.h>

clock_t start1;
clock_t end1;
double timer1;
clock_t start2;
clock_t end2;
double timer2;

int row1;
int row2;
int col1;
int col2;

int matA[50][50];
int matB[50][50];
int result[50][50] = {0};
int resultEle[50][50] = {0};
int steps = 0;
int stepEle = 0;
/*here i use cell limited by stepEle which is equalt to
  total number of elements in the array to determine which
  element to write into using the global variables*/
void *method1()
{
    int cell = stepEle;
    stepEle++;
    int temp1 = cell/col2;
    int temp2 = cell%col2;
    int i;

    for(i=0;i<col1;i++)
    {
            resultEle[temp1][temp2] += matA[temp1][i]*matB[i][temp2];
    }

}
/*here i used the global variables and set my
  row by row impelementation i have 'row' variable
  which incerements each time the method is called
  (limited to be row1) because i only call row 1 times*/
void *method2()
{
    int row = steps;
    int i;
    int j;

    for(i=0;i<col2;i++)
    {
        for(j=0;j<col1;j++)
        {
            result[row][i] += matA[row][j] * matB[j][i];
        }
    }
    steps++;
}

int main()
{

    int i;
    int j;
    int k;

    FILE *fptr;
    fptr = fopen("input.txt", "r");
    /*reading rows and cols of first matrix*/
    fscanf(fptr, "%d", &row1);
    fscanf(fptr, "%d", &col1);
    matA[row1][col1];
    /*reading each element of the first matrix in file*/
    for(i=0;i<row1;i++)
    {
        for(j=0;j<col1;j++)
        {
            fscanf(fptr, "%d", &matA[i][j]);
        }
    }

    /*reading rows and cols of second matrix*/
    fscanf(fptr, "%d", &row2);
    fscanf(fptr, "%d", &col2);
    matB[row2][col2];
    /*reading each element of the second matrix in file*/
    for(i=0;i<row2;i++)
    {
        for(j=0;j<col2;j++)
        {
            fscanf(fptr, "%d", &matB[i][j]);
        }
    }
    fclose(fptr);
    /*total number of threads in element by element*/
    int no_of_threads = row1*col2;
    pthread_t thread[no_of_threads];
    start1 = clock();
    for(i=0;i<row1;i++)
    {
        for(j=0;j<col2;j++)
        {
            pthread_create(&thread[i*col2 +j], NULL, method1, NULL);
        }
    }

    fptr = fopen("output.txt", "w");
    fprintf(fptr, "Result matrix1\n\n");

    for (i = 0; i < no_of_threads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for(i=0;i<row1;i++)
    {
        for(j=0;j<col2;j++)
        {
            fprintf(fptr, "%d ", resultEle[i][j]);
        }

        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\nEND1:\t");
    end1 = clock();

    timer1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
    fprintf(fptr, "%lf\n",timer1);
    /*number of thread for row by row method*/
    pthread_t byRows[row1];
    start2 = clock();
    /*creating row1 threads with argument NULL
      since im using global variables*/
    for (i=0;i<row1;i++)
    {
        pthread_create(&byRows[i], NULL, method2, NULL);
    }

    for (i=0;i<row1;i++)
    {
        pthread_join(byRows[i], NULL);
    }

    end2 = clock();
    timer2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
    fprintf(fptr, "Result matrix2\n\n");

    for(i=0;i<row1;i++)
    {
        for(j=0;j<col2;j++)
        {
            fprintf(fptr, "%d ", result[i][j]);
        }

        fprintf(fptr, "\n");
    }

    fprintf(fptr, "\nEND2:\t");
    fprintf(fptr, "%lf\n",timer2);
    fclose(fptr);

    return 0;
}
