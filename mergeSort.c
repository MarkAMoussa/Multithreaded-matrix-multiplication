#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <time.h>

int list[100];

void merge(int l, int m, int r)
{
    int arr1 = l;
    int arr2 = m+1;
    int temp[r-l+1];
    int index = 0;

    while(arr1 <= m && arr2 <= r)
    {
        if(list[arr1] < list[arr2])
        {
            temp[index] = list[arr1];
            arr1++;
            index++;
        }
        else
        {
            temp[index] = list[arr2];
            arr2++;
            index++;
        }
    }

    while(arr1 <= m)
    {
        temp[index] = list[arr1];
        arr1++;
        index++;
    }
    while(arr2 <= r)
    {
        temp[index] = list[arr2];
        arr2++;
        index++;
    }
    int i;
    for (i = 0; i<(r-l+1); i++)
        list[l+i] = temp[i];

}

void * mergesort(void *arg)
{
    int i;
    int *temp = (int *)arg;
    int mid = (temp[0]+temp[1])/2;

    int left[2] = {temp[0], mid};
    int right[2] = {mid+1, temp[1]};


    if(temp[0] >= temp[1])
        return;

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, mergesort, &left);
    pthread_create(&thread2, NULL, mergesort, &right);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    merge(temp[0], mid, temp[1]);

}

int main()
{
    int length;
    int i;

    FILE *fptr;
    fptr = fopen("input.txt", "r");
    fscanf(fptr, "%d", &length);

    int limits[2] = {0, length-1};

    list[length];

    for(i=0;i<length;i++)
    {
        fscanf(fptr, "%d ", &list[i]);
    }
    fclose(fptr);

    pthread_t thread;
    pthread_create(&thread, NULL, mergesort, &limits);

    pthread_join(thread, NULL);

    for(i=0;i<length;i++)
    {
        printf("%d ", list[i]);
    }

return 0;
}
