#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
int bt[5];
int timeQuantum = 3;
pthread_mutex_t mutex;
int findNext(int turn)
{
    int cur = (turn + 1) % 5;
    while (bt[cur] <= 0)
    {
        if (cur == turn && bt[turn] == 0)
            return -1;
        cur = (cur + 1) % 5;
    }
    return cur;
}
void *schedule(void *args)
{
    int tid = *(int *)args;
    int turn = tid;
    pthread_mutex_lock(&mutex);
    while (bt[tid] > 0)
    {
        if (bt[turn] >= timeQuantum)
        {
            printf("Executing thread %d for %d seconds \n", turn, timeQuantum);
            sleep(timeQuantum);
            bt[turn] -= timeQuantum;
        }
        else
        {
            printf("Executing thread %d for %d seconds \n", turn, bt[turn]);
            sleep(bt[turn]);
            bt[turn] = 0;
        }
        turn = findNext(turn);
        if (turn == -1)
            break;
    }
    pthread_mutex_unlock(&mutex);
}
int main(int argc, char *argv[])
{
    pthread_t th[5];
    if (argc < 6)
    {
        printf("Usage : ./rr.c <burst time 1> <burst time 2>\n");
        return 1;
    }
    pthread_mutex_init(&mutex, NULL);
    for (int i = 1; i < argc; i++)
    {
        bt[i - 1] = atoi(argv[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&th[i], NULL, schedule, idx);
        free(idx);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
}