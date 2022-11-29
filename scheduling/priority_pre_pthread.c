#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
int counter = 0;
int arrival[5] = {0, 1, 2, 5, 8};
int burst[5] = {4, 1, 1, 2, 3};
int priority[5] = {10, 2, 6, 9, 4}; // priotity
struct process *List;
pthread_cond_t cond;
pthread_mutex_t sch;
int finish = 0;
int size = 0;
// pthread_mutex_t add;
struct process
{
    int id;
    int arrival_time;
    int burst;
    int start_time;
    int exit_time;
    int priority; // priotrity
    int execOnce;
};
struct process *current_process = NULL;
void *runner(void *arg)
{
    while (1)
    {
        if (current_process == NULL)
        {
            printf("Nothing running currently\n");
        }
        else if (current_process->burst == 0)
        {
            printf("process %d finished \n", current_process->id);
            current_process->exit_time = counter;
            current_process = NULL;
            pthread_cond_signal(&cond);
        }
        else
        {
            printf("process %d executing \n", current_process->id);
            current_process->burst--;
            if (current_process->execOnce == 0)
                current_process->start_time = counter;
            current_process->execOnce = 1;
        }
        sleep(1);
        counter++;
    }
}
void *addProcess(void *arg)
{
    for (int i = 0; i < 5;)
    {
        if (counter == arrival[i])
        {
            struct process newProcess;
            newProcess.id = i;
            newProcess.arrival_time = arrival[i];
            newProcess.burst = burst[i];
            newProcess.execOnce = 0;
            newProcess.priority = priority[i];
            List[i] = newProcess;
            size++;
            pthread_cond_signal(&cond);
            i++;
        }
    }
}
void *scheduler(void *arg)
{
    while (1)
    {
        int f = 0;
        int c = 0;
        pthread_mutex_lock(&sch);
        pthread_cond_wait(&cond, &sch);
        for (int i = 0; i < size; i++)
        {
            if (current_process == NULL || current_process->priority > List[i].priority) // priority
            {
                if (List[i].burst > 0)
                {
                    c = 1;
                    current_process = &List[i];
                }
            }
        }
        if (c == 1)
            printf("Context Switch \n");
        for (int i = 0; i < size; i++)
        {
            if (List[i].burst > 0)
                f = 1;
        }
        if (f == 0)
        {
            return NULL;
        }
        pthread_mutex_unlock(&sch);
    }
}
int main()
{
    List = malloc(sizeof(struct process) * 5);
    pthread_mutex_init(&sch, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t schedulerThread, addProcessThread, runnerThread;
    pthread_create(&schedulerThread, NULL, scheduler, NULL);
    pthread_create(&addProcessThread, NULL, addProcess, NULL);
    pthread_create(&runnerThread, NULL, runner, NULL);
    pthread_join(addProcessThread, NULL);
    pthread_join(schedulerThread, NULL);
    pthread_mutex_destroy(&sch);
    pthread_cond_destroy(&cond);
    for (int i = 0; i < size; i++)
    {
        printf("Process %d : \n", List[i].id);
        printf("waiting time : %d\n", List[i].exit_time - List[i].arrival_time - burst[i]);
        printf("turnaround : %d\n", List[i].exit_time - List[i].arrival_time);
    }
}