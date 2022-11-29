#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
int counter = 0;
int size = 0;
int N;
int R;
int page_fault_count = 0;
pthread_mutex_t mutex;
struct page
{
    int page_no;
    int time_of_last_use;
};
struct page *pageSet;
int find(int ref)
{
    for (int i = 0; i < size; i++)
    {
        if (pageSet[i].page_no == ref)
            return 1;
    }
    return 0;
}
int leastRecentlyUsed()
{
    int leastRecentlyUsed = __INT_MAX__;
    int leastRecentlyUsed_index = -1;
    for (int i = 0; i < size; i++)
    {
        if (leastRecentlyUsed > pageSet[i].time_of_last_use)
        {
            leastRecentlyUsed = pageSet[i].time_of_last_use;
            leastRecentlyUsed_index = i;
        }
    }
    return leastRecentlyUsed_index;
}
void showPageSet()
{
    for (int i = 0; i < size; i++)
        printf("%d ", pageSet[i].page_no);
    printf("\n");
}
void *lru_algorithm(void *arg)
{
    int ref = rand() % 100;
    pthread_mutex_lock(&mutex);
    counter++;
    if (find(ref) == 1)
    {
        printf("Found Page %d \n", ref);
        return NULL;
    }
    printf("Page Fault\n");
    page_fault_count++;
    if (size == N)
    {
        printf("Page set full \n");
        int idx = leastRecentlyUsed();
        struct page p;
        p.page_no = ref;
        p.time_of_last_use = counter;
        printf("Replaced Page no %d with Page no %d \n", pageSet[idx].page_no, ref);
        pageSet[idx] = p;
    }
    else
    {
        printf("Page set not full \n");
        struct page p;
        p.page_no = ref;
        p.time_of_last_use = counter;
        printf("Inserting Page no %d \n", ref);
        pageSet[size++] = p;
    }
    printf("Current page set : ");
    showPageSet();
    printf("\n");
    pthread_mutex_unlock(&mutex);
}
int main()
{
    srand(time(NULL));
    scanf("%d", &N);
    scanf("%d", &R);
    pthread_mutex_init(&mutex, NULL);
    pthread_t th[R];
    pageSet = (struct page *)malloc(sizeof(struct page) * N);
    for (int i = 0; i < R; i++)
    {
        pthread_create(&th[i], NULL, lru_algorithm, NULL);
    }
    for (int i = 0; i < R; i++)
    {
        pthread_join(th[i], NULL);
    }
    printf("Page fault count : %d\n", page_fault_count);
}