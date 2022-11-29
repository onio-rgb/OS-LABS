#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
struct process
{
    int b;
    int a;
    int exit;
    int pid;
};
int start = 0;
int n = 5;
int counter = 0;
int arrival[5] = {2, 3, 6, 8, 10};
int burst[5] = {4, 2, 5, 3, 1};
int size = 0;
struct process *arr;
struct process *curr;
pthread_cond_t cond;
pthread_mutex_t mutex;
void *runner(void *arg)
{
    while (1)
    {
        if (curr == NULL)
        {
            printf("%d Notin run curr\n", counter);
        }
        else if (curr->b == 0)
        {
            printf("%d Process %d exiting \n", counter, curr->pid);
            curr->exit = counter;
            curr = NULL;
            pthread_cond_signal(&cond);
        }
        else
        {
            printf("%d Process %d running \n", counter, curr->pid);
            curr->b--;
        }
        sleep(1);
        counter++;
        for (int i = 0; i < n; i++)
        {
            if (counter == arrival[i])
            {
                struct process p;
                p.pid = i;
                p.a = counter;
                p.b = burst[i];
                p.exit = 0;
                arr[size++] = p;
                printf("%d Process %d arrived \n", counter, p.pid);
                if (!start)
                {
                    start = 1;
                    pthread_cond_signal(&cond);
                }
            }
        }
    }
}
void *scheduler(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("%d scheduler called \n", counter);
        for (int i = 0; i < n; i++)
        {
            if ((!curr || curr->b > arr[i].b) && arr[i].b > 0)
            {
                curr = &arr[i];
            }
        }
        int f = 0;
        for (int i = 0; i < n; i++)
        {
            if (arr[i].b > 0)
                f = 1;
        }
        if (f == 0)
            return NULL;
        pthread_mutex_unlock(&mutex);
    }
}
int main()
{

    arr = malloc(sizeof(struct process) * n);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t sch, run;
    pthread_create(&sch, NULL, scheduler, NULL);
    pthread_create(&run, NULL, runner, NULL);
    pthread_join(sch, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}