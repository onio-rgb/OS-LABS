#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
int N = 5;
sem_t *sem;
sem_t lock;
void eat(int idx)
{
    sem_wait(&lock);
    sem_wait(&sem[idx]);
    sem_wait(&sem[(idx + 1) % N]);
    printf("Philosopher %d eating using chopsticks %d and %d\n", idx, idx, (idx + 1) % N);
    sem_post(&sem[(idx + 1) % N]);
    sem_post(&sem[idx]);
    sem_post(&lock);
}
void *philosopher(void *arg)
{
    int idx = *(int *)arg;
    while (1)
    {
        eat(idx);
        sleep(1);
    }
}
int main()
{
    sem_init(&lock, 0, 1);
    sem = malloc(sizeof(sem_t) * N);
    for (int i = 0; i < N; i++)
    {
        sem_init(&sem[i], 0, 1);
    }
    pthread_t th[N];
    for (int i = 0; i < N; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&th[i], NULL, philosopher, arg);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(th[i], NULL);
    }
}