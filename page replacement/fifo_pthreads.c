#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int N; // no of pages
int R; // no of references
int page_fault = 0;
pthread_mutex_t mutex;
struct Queue
{
    int ref;
    int capacity;
    int size;
    int front;
    int rear;
    int *array;
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
        queue->capacity * sizeof(int));
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

void enqueue(struct Queue *queue, int ref)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    if (queue->front == -1)
        queue->front = 0;
    queue->array[queue->rear] = ref;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue *queue)
{

    if (isEmpty(queue))
        return -1;
    int ref = queue->array[queue->front];
    if (queue->front == queue->rear)
    {
        queue->front = -1;
        queue->rear = -1;
    }
    else
        queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return ref;
}
int front(struct Queue *queue)
{
    if (isEmpty(queue))
        return -1;
    return queue->array[queue->front];
}

int rear(struct Queue *queue)
{
    if (isEmpty(queue))
        return -1;
    return queue->array[queue->rear];
}
struct Queue *pages;
int find_in_pages(int ref)
{
    int f = 0;
    if (isEmpty(pages))
        return 0;
    int size = pages->size;
    for (int i = 0; i < size; i++)
    {
        int x = dequeue(pages);
        if (x == ref)
            f == 1;
        enqueue(pages, x);
    }
    return f;
}
void print_current_page_status()
{
    int size = pages->size;
    for (int i = 0; i < size; i++)
    {
        int x = dequeue(pages);
        printf("%d ", x);
        enqueue(pages, x);
    }
    printf("\n");
}
void *create_references(void *args)
{
    int ref = rand() % 100;
    pthread_mutex_lock(&mutex);
    int f = find_in_pages(ref);
    if (f == 1)
    {
        printf("found %d in the pages \n", ref);
    }
    else
    {
        printf("Page Fault: Could not find %d in the pages\n", ref);
        page_fault++;
        int f1 = isFull(pages);
        if (f1 == 1)
        {
            int rem = dequeue(pages);
            sleep(1);
            printf("Removed %d from the pages\n", rem);
        }
        enqueue(pages, ref);
        sleep(1);

        printf("Inserted %d into the pages\n", ref);
        printf("Current set of pages : ");
        print_current_page_status();
        printf("-------------------\n");
    }
    pthread_mutex_unlock(&mutex);
}
int main()
{
    srand(time(NULL));
    scanf("%d", &N);
    scanf("%d", &R);
    pages = (struct Queue *)createQueue(N);
    pthread_t th[R];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < R; i++)
    {
        pthread_create(&th[i], NULL, create_references, NULL);
        sleep(1);
    }
    for (int i = 0; i < R; i++)
    {
        pthread_join(th[i], NULL);
    }
    printf("Total Page faults : %d\n", page_fault);
}