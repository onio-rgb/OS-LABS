#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
int *available;
int *finish;
int *work;
int *maxres;
int MAX_PROCESS = 0;
int RESOURCES_M = 3;
int deadlockHappened = 0;
pthread_mutex_t resourceRequest;
pthread_mutex_t addProcess;
struct process
{
    int pid;
    int *allocated;
    int *need;
    int *request;
};
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    struct process *array;
};

struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = -1;
    queue->size = 0;
    queue->rear = -1;
    queue->array = malloc(
        queue->capacity * sizeof(struct process));
    return queue;
}

int isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

void enqueue(struct Queue *queue, struct process item)
{
    if (isFull(queue))
        return;

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    if (queue->front == -1)
        queue->front = 0;
    queue->size = queue->size + 1;
}

struct process dequeue(struct Queue *queue)
{
    struct process item = queue->array[queue->front];
    if (queue->front == queue->rear)
    {
        queue->front = -2;
        queue->rear = -1;
    }
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
struct process front(struct Queue *queue)
{
    return queue->array[queue->front];
}

struct process rear(struct Queue *queue)
{
    return queue->array[queue->rear];
}
struct Queue *readyQueue;
int safetyAlgorithm()
{
    for (int i = 0; i < RESOURCES_M; i++)
        work[i] = available[i];

    int noAvailableProcess = 0;
    int size = readyQueue->size;
    for (int i = 0; i < size; i++)
    {
        struct process temp = dequeue(readyQueue);
        int availableForScheduling = 1;
        for (int j = 0; j < RESOURCES_M; j++)
        {
            if (temp.need[j] > work[j])
                availableForScheduling = 0;
        }
        if (availableForScheduling == 1)
        {
            for (int j = 0; j < RESOURCES_M; j++)
            {
                work[j] = work[j] + temp.allocated[j];
                finish[j] = 1;
            }
        }
        enqueue(readyQueue, temp);
    }
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (finish[i] == 0)
            noAvailableProcess = 1;
    }
    if (noAvailableProcess == 1)
    {
        printf("Deadlock will happen : \n");
        return 0;
    }
    return 1;
}
void requestResource(struct process p)
{
    pthread_mutex_lock(&resourceRequest);
    sleep(2);
    if (deadlockHappened)
    {
        pthread_mutex_unlock(&resourceRequest);
        return;
    }
    printf("Requesting resources for process %d : ", p.pid);
    for (int i = 0; i < RESOURCES_M; i++)
    {
        p.request[i] = rand() % (p.need[i]);
        printf("%d ", p.request[i]);
    }
    printf("\n");
    for (int i = 0; i < RESOURCES_M; i++)
    {
        p.allocated[i] += p.request[i];
        available[i] -= p.request[i];
        p.need[i] -= p.request[i];
    }
    int safe = safetyAlgorithm();
    if (!safe)
    {
        printf("Cant request resources for %d \n", p.pid);
        deadlockHappened = 1;
        pthread_mutex_unlock(&resourceRequest);
        return;
    }
    pthread_mutex_unlock(&resourceRequest);
}
void *addProcessToReadyQueue(void *arg)
{
    pthread_mutex_lock(&addProcess);
    int pid = *(int *)arg;
    struct process newProcess;
    newProcess.allocated = malloc(sizeof(int) * RESOURCES_M);
    newProcess.need = malloc(sizeof(int) * RESOURCES_M);
    newProcess.request = malloc(sizeof(int) * RESOURCES_M);
    newProcess.pid = pid;
    for (int i = 0; i < RESOURCES_M; i++)
    {
        newProcess.allocated[i] = rand() % (available[i] / 2);
        newProcess.need[i] = rand() % available[i];
        available[i] -= newProcess.allocated[i];
    }
    enqueue(readyQueue, newProcess);
    printf("Process %d has allocated : ", pid);
    for (int i = 0; i < RESOURCES_M; i++)
    {
        printf("%d ", newProcess.allocated[i]);
    }
    printf("\nProcess %d needs : ", pid);
    for (int i = 0; i < RESOURCES_M; i++)
    {
        printf("%d ", newProcess.need[i]);
    }
    printf("\n");
    int f = rand() % 2;
    if (f == 1)
    {
        struct process curr = rear(readyQueue);
        pthread_mutex_unlock(&addProcess);
        requestResource(curr);
        return NULL;
    }
    pthread_mutex_unlock(&addProcess);
}
int main()
{
    srand(time(NULL));
    scanf("%d", &MAX_PROCESS);
    pthread_mutex_init(&resourceRequest, NULL);
    pthread_mutex_init(&addProcess, NULL);
    pthread_t *th = malloc(sizeof(pthread_t) * MAX_PROCESS);
    maxres = malloc(sizeof(int) * RESOURCES_M);
    available = malloc(sizeof(int) * RESOURCES_M);
    work = malloc(sizeof(int) * RESOURCES_M);
    finish = malloc(sizeof(int) * MAX_PROCESS);
    available[0] = work[0] = maxres[0] = 10;
    available[1] = work[1] = maxres[1] = 7;
    available[2] = work[2] = maxres[2] = 12;
    printf("Initial Resources : ");
    for (int i = 0; i < RESOURCES_M; i++)
    {
        printf("%d \n", maxres[i]);
    }
    readyQueue = (struct Queue *)createQueue(100);
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&th[i], NULL, addProcessToReadyQueue, arg);
    }
    sleep(1);
    printf("Available resources : ");
    for (int i = 0; i < RESOURCES_M; i++)
    {
        printf("%d ", available[i]);
    }
    printf("\n");
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        pthread_join(th[i], NULL);
    }
}