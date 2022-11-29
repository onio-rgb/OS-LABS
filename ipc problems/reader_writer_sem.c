#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
sem_t wrt;
sem_t lock;
int readcount = 0;
void *reader(void *arg)
{
    int idx = *(int *)arg;
    sem_wait(&lock);
    readcount++;
    if (readcount == 1)
    {
        sem_wait(&wrt);
    }
    sem_post(&lock);
    printf("Reader %d reading \n", idx);
    sem_wait(&lock);
    readcount--;
    if (readcount == 0)
    {
        sem_post(&wrt);
    }
    sem_post(&lock);
}
void *writer(void *arg)
{
    int idx = *(int *)arg;

    sem_wait(&wrt);
    printf("Writer %d writing \n", idx);
    sem_post(&wrt);
}
int main()
{
    sem_init(&wrt, 0, 1);
    sem_init(&lock, 0, 1);
    pthread_t readert[10];
    pthread_t writert[5];
    for (int i = 0; i < 10; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&readert[i], NULL, reader, arg);
        if (i < 5)
        {
            pthread_create(&writert[i], NULL, writer, arg);
        }
    }

    for (int i = 0; i < 10; i++)
        pthread_join(readert[i], NULL);
    for (int i = 0; i < 5; i++)
        pthread_join(writert[i], NULL);
}