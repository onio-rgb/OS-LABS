#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
int N;
int R;
int *arr;
int *pages;

int find_longest_used_after(int i)
{
    int mx_index_ref = -1;
    int mx_index_pages = -1;
    int temp[N];
    for (int x = 0; x < N; x++)
        temp[x] = 999;
    for (int j = 0; j < N; j++)
    {
        for (int k = i; k < R; k++)
        {
            if (arr[k] == pages[j])
            {
                temp[j] = k;
                break;
            }
        }
    }
    int mx = -1;
    int mx_i = 0;
    for (int x = 0; x < N; x++)
    {
        if (mx < temp[x])
        {
            mx = temp[x];
            mx_i = x;
        }
    }
    return mx_i;
}
int find(int ref)
{
    for (int i = 0; i < N; i++)
    {
        if (pages[i] == ref)
            return 1;
    }
    return 0;
}
int main()
{
    scanf("%d", &N);
    scanf("%d", &R);
    arr = (int *)malloc(sizeof(int) * R);
    pages = (int *)malloc(sizeof(int) * N);
    printf("Enter the references\n");
    for (int i = 0; i < R; i++)
    {
        scanf("%d", &arr[i]);
    }
    int size = 0;
    for (int i = 0; i < R; i++)
    {
        printf("Finding Page %d\n", arr[i]);
        if (find(arr[i]) == 1)
        {
            printf("Found Page %d\n", arr[i]);
            printf("Current set of pages : ");
            for (int j = 0; j < size; j++)
                printf("%d ", pages[j]);
            printf("\n--------------\n");
            continue;
        }
        printf("Page Fault : Can't find Page %d \n", arr[i]);
        if (size == N)
        {
            int idx = find_longest_used_after(i);
            printf("Page set Full. Replacing Page %d with Page %d \n", pages[idx], arr[i]);
            pages[idx] = arr[i];
        }
        else
        {
            printf("Page set not Full. Inserting Page %d \n", arr[i]);
            pages[size] = arr[i];
            size++;
        }
        printf("Current set of pages : ");
        for (int j = 0; j < size; j++)
            printf("%d ", pages[j]);
        printf("\n--------------\n");
    }
}