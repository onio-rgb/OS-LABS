#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
int n = 10;
int m = 3;
int *available;
int *finish;
int *work;
int *maxres;
pthread_cond_t callBanker;
pthread_mutex_t mutex;

struct process
{
    int pid;
    int *allocated;
    int *need;
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
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
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
    queue->size = queue->size + 1;
}

struct process dequeue(struct Queue *queue)
{
    struct process item = queue->array[queue->front];
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
void *banker()
{
    int count = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (isEmpty(readyQueue) == 1)
        {
            pthread_cond_wait(&callBanker, &mutex);
        }
        int noAvailableProcess = 1;
        int size = readyQueue->size;
        for (int i = 0; i < size; i++)
        {
            struct process temp = dequeue(readyQueue);
            int availableForScheduling = 1;
            for (int j = 0; j < m; j++)
            {
                if (temp.need[j] > work[j])
                    availableForScheduling = 0;
            }
            if (availableForScheduling == 1)
            {
                noAvailableProcess = 0;
                ++count;
                printf("Process %d scheduled\n", temp.pid);
                for (int j = 0; j < m; j++)
                {
                    work[j] = work[j] + temp.allocated[j];
                }
                for (int i = 0; i < m; i++)
                {
                    printf("%d ", work[i]);
                }
                printf("\n");
            }
            else
            {
                enqueue(readyQueue, temp);
            }
        }
        if (noAvailableProcess == 1)
        {
            printf("Deadlock happended : \n");
            printf("Available : ");
            for (int i = 0; i < m; i++)
            {
                printf("%d ", work[i]);
            }
            printf("\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        if (count == n && isEmpty(readyQueue) == 1)
        {
            printf("Bankers Exiting \n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        pthread_mutex_unlock(&mutex);
    }
}
void *simulateProcess(void *arg)
{
    int pid = *(int *)arg;
    struct process newProcess;
    newProcess.allocated = malloc(sizeof(int) * m);
    newProcess.need = malloc(sizeof(int) * m);
    newProcess.pid = pid;
    for (int i = 0; i < m; i++)
    {
        newProcess.allocated[i] = rand() % (maxres[i] / 3);
        newProcess.need[i] = rand() % maxres[i];
    }
    enqueue(readyQueue, newProcess);
    printf("Process %d has allocated : ", pid);
    for (int i = 0; i < m; i++)
    {
        printf("%d ", newProcess.allocated[i]);
    }
    printf("\nProcess %d needs : ", pid);
    for (int i = 0; i < m; i++)
    {
        printf("%d ", newProcess.need[i]);
    }
    printf("\n");
    pthread_cond_signal(&callBanker);
}
int main()
{
    srand(time(NULL));
    maxres = malloc(sizeof(int) * m);
    available = malloc(sizeof(int) * m);
    work = malloc(sizeof(int) * m);
    finish = malloc(sizeof(int) * n);

    available[0] = work[0] = maxres[0] = 10;
    available[1] = work[1] = maxres[1] = 7;
    available[2] = work[2] = maxres[2] = 12;
    printf("Initial Resources : ");
    for (int i = 0; i < m; i++)
    {
        // available[i] = 0;
        // work[i] = 0;
        printf("%d ", maxres[i]);
    }
    printf("\n");
    readyQueue = (struct Queue *)createQueue(100);
    pthread_cond_init(&callBanker, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t *bankerThread = malloc(sizeof(pthread_t));
    pthread_create(&bankerThread, NULL, banker, NULL);

    pthread_t *th = malloc(sizeof(pthread_t) * n);
    for (int i = 0; i < n; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&th[i], NULL, simulateProcess, arg);
        sleep(1);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(th[i], NULL);
    }
    pthread_join(bankerThread, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&callBanker);
}