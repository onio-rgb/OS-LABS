#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
sem_t sem[3];
pthread_mutex_t mutex;
int finish[4];
int available[3] = {9, 4, 10};
int work[3];
int allocated[4][3] = {{1, 0, 2}, {0, 0, 1}, {1, 1, 0}, {0, 2, 0}};
int maxres[3] = {10, 7, 12};
int need[4][3] = {{3, 1, 4}, {5, 1, 2}, {1, 4, 2}, {1, 2, 1}};
int safety()
{
    for (int i = 0; i < 3; i++)
    {
        work[i] = available[i];
        finish[i] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        int f1 = 0;
        for (int j = 0; j < 4; j++)
        {
            int f2 = 0;
            for (int k = 0; k < 3; k++)
            {
                if (need[j][k] > work[k] && finish[j] == 0)
                    f2 = 1;
            }
            if (f2 == 0)
            {
                finish[j] = 1;
                for (int k = 0; k < 3; k++)
                {
                    work[k] += allocated[j][k];
                }
                f1 = 1;
                break;
            }
        }
        if (f1 == 0)
        {
            for (int x = 0; x < 4; x++)
            {
                if (finish[x] == 0)
                {
                    printf("Unsafe \n");
                    return 0;
                }
            }
            printf("safe\n");
            return 1;
        }
    }
}
void *resource_request(void *arg)
{
    pthread_mutex_lock(&mutex);
    int idx = *(int *)arg;
    int req[3];
    printf("Process %d requesting resources : ", idx);
    for (int i = 0; i < 3; i++)
    {
        req[i] = rand() % (available[i] / 2);
        printf("%d ", req[i]);
    }
    printf("\n");
    for (int i = 0; i < 3; i++)
    {
        if (req[i] > need[idx][i])
        {
            printf("Invalid req > need\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }
    printf("waiting process %d\n", idx);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < available[i]; j++)
        {
            sem_post(&sem[i]);
        }
    }
    printf("got process %d\n", idx);
    for (int i = 0; i < 3; i++)
    {
        available[i] -= req[i];
        need[idx][i] -= req[i];
        allocated[idx][i] += req[i];
    }
    if (safety())
    {
        printf("Allocated for process %d \n", idx);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < req[i]; j++)
                sem_wait(&sem[i]);
        }
    }
    else
    {
        printf("Unsafe\n");
        for (int i = 0; i < 3; i++)
        {
            available[i] += req[i];
            need[idx][i] += req[i];
            allocated[idx][i] -= req[i];
        }
    }
    pthread_mutex_unlock(&mutex);
}
int main()
{
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 3; i++)
    {
        sem_init(&sem[i], 0, available[i]);
    }
    pthread_t th[4];
    for (int i = 0; i < 4; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&th[i], NULL, resource_request, arg);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(th[i], NULL);
    }
    for (int i = 0; i < 4; i++)
    {
        sem_destroy(&sem[i]);
    }
}