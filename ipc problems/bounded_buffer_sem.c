#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
int buffsize = 10;
int *buff;
sem_t empty;
sem_t full;
sem_t lock;
int in = 0;
int out = 0;
void *consumer(void *arg)
{
    int indx = *(int *)arg;
    while (1)
    {
        sem_wait(&full);
        sem_wait(&lock);
        int item = buff[out];
        out = (out + 1) % buffsize;
        printf("consumed %d by consumer %d\n", item, indx);
        sem_post(&lock);
        sem_post(&empty);
        sleep(1);
    }
}
void *producer(void *arg)
{
    int item = rand() % 100;
    int indx = *(int *)arg;
    while (1)
    {
        sem_wait(&empty);
        sem_wait(&lock);
        buff[in] = item;
        in = (in + 1) % buffsize;
        printf("Produced %d by Consumer %d\n", item, indx);
        sem_post(&lock);
        sem_post(&full);
        sleep(2);
    }
}
int size = 0;
int main()
{
    srand(time(NULL));
    buff = malloc(sizeof(int) * buffsize);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, buffsize);
    sem_init(&lock, 0, 1);
    pthread_t con[10];
    pthread_t prod[10];
    for (int i = 0; i < 10; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&prod[i], NULL, producer, (void *)arg);
    }
    for (int i = 0; i < 10; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&con[i], NULL, consumer, (void *)arg);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(con[i], NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(prod[i], NULL);
    }
}