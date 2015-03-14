#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define K 7
#define N 5
#define M 5

typedef struct {
    int key;
    unsigned int value;
}heap_elem_t;

typedef struct {
    heap_elem_t *elem;
    unsigned int size;
    unsigned int capacity;
}heap_t;

typedef struct {
    int *elem;
    unsigned int size;
}list_t;

void build_min_heap(heap_elem_t *arr, const unsigned int n)
{
    int i;
    void min_heapify(
            heap_elem_t *const arr,
            const unsigned int i,
            const unsigned int n);
            
    for (i = (int) ((n >> 1) - 1); i >= 0; i--)
        min_heapify(arr, (unsigned int) i, n);
}

void heap_decrease_key(heap_t *const heap, const unsigned int i, const int key)
{

#define PARENT(i) (((i + 1) >> 1) - 1)

    unsigned int j, k;
    heap_elem_t tmp;

    if (key > heap->elem[i].key) {
        fprintf(stderr, "New key is larger than current key in heap_decrease_key().\n");
        exit(1);
    }
    
    heap->elem[i].key = key;
    
    j = i;
    k = PARENT(j);
    while (k < j && heap->elem[k].key > heap->elem[j].key) {
        tmp = heap->elem[k];
        heap->elem[k] = heap->elem[j];
        heap->elem[j] = tmp;
        j = k;
        k = PARENT(j);
    }
}

heap_elem_t heap_extract_min(heap_t *heap)
{
    heap_elem_t min;
    void min_heapify(
            heap_elem_t *const arr, 
            const unsigned int i, 
            const unsigned int n);

    if (heap->size < 1) {
        fprintf(stderr, "Heap overflow in heap_extract_min()\n");
        exit(1);
    }
    
    min = heap->elem[0];
    heap->elem[0] = heap->elem[--heap->size];
    min_heapify(heap->elem, 0, heap->size);
    
    return min;
}

void merge(const list_t *const sorted_lists, unsigned int k, int *const sorted)
{
    heap_t heap;
    unsigned int *min_elem_index;
    
    unsigned int i, j;
    heap_elem_t new_elem, min_elem;
    
    void build_min_heap(heap_elem_t *arr, const unsigned int n);
    heap_elem_t heap_extract_min(heap_t *heap);
    void min_heap_insert(heap_t *heap, heap_elem_t elem);
    
    heap.elem = (heap_elem_t *) malloc(k * sizeof(heap_elem_t));
    heap.size = heap.capacity = k;
    
    min_elem_index = (unsigned int *) malloc(k * sizeof(unsigned int));
    
    for (i = 0; i < k; i++) {
        min_elem_index[i] = 0;
        heap.elem[i].key = sorted_lists[i].elem[0];
        heap.elem[i].value = i;
    }
    
    build_min_heap(heap.elem, k);
    
    i = 0;
    while (k > 0) {
        min_elem = heap_extract_min(&heap);
        sorted[i++] = min_elem.key;
        j = min_elem.value;
        if (++min_elem_index[j] < sorted_lists[j].size) {
            new_elem.key = sorted_lists[j].elem[min_elem_index[j]];
            new_elem.value = j;
            min_heap_insert(&heap, new_elem);
        }
        else
            --k;
    }
    
    free(min_elem_index);
    free(heap.elem);
}

void min_heap_insert(heap_t *const heap, heap_elem_t elem)
{
    void heap_decrease_key(heap_t *const heap, const unsigned int i, const int key);
    if (++heap->size > heap->capacity) {
        fprintf(stderr, "Out of heap capacity in min_heap_insert().\n");
        exit(1);
    }
    heap->elem[heap->size - 1].key = INT_MAX;
    heap->elem[heap->size - 1].value = elem.value;
    heap_decrease_key(heap, heap->size - 1, elem.key);
}

void min_heapify(
        heap_elem_t *const arr,
        const unsigned int i,
        const unsigned int n)
{

#define LEFT(i)  ((i << 1) | 1)
#define RIGHT(i) ((i << 1) + 2)

    unsigned int l, r;
    int stop;
    heap_elem_t smallest;
    unsigned int smallest_id;
    
    const heap_elem_t elem = arr[i];
    unsigned int j = i;
    
    do {
        stop = 1;
        l = LEFT(j);
        r = RIGHT(j);
        
        if (l < n && arr[l].key < elem.key) {
            smallest = arr[l];
            smallest_id = l;
        }
        else {
            smallest = elem;
            smallest_id = j;
        }
        
        if (r < n && arr[r].key < smallest.key) {
            smallest = arr[r];
            smallest_id = r;
        }
        
        if (smallest_id != j) {
            arr[j] = smallest;
            j = smallest_id;
            stop = 0;
        }
    } while(!stop);
    
    arr[j] = elem;
}

/*
 * Test driver
 */
int main()
{
    unsigned int i, j;
    unsigned int k, n;
    list_t *sorted_lists;
    int *all_sorted;
    
    /* Construct k sorted lists. */
    
    srand(time(NULL));
    k = (unsigned int) rand() % K + 1;
    printf("We have %u sorted list%s at the beginning:\n", k, k == 1 ? "" : "s");
    
    sorted_lists = (list_t *) malloc(k * sizeof(list_t));
    for (i = 0, n = 0; i < k; i++) {
        printf("A[%u] = {", i);
        sorted_lists[i].size = (unsigned int) (rand() % N) + (unsigned int) (rand() % M) + 1;
        sorted_lists[i].elem = (int *) malloc(sorted_lists[i].size * sizeof(int));
        sorted_lists[i].elem[0] = rand() % M;
        for (j = 1; j < sorted_lists[i].size; j++)
            sorted_lists[i].elem[j] = sorted_lists[i].elem[j-1] + rand() % M;
        for (j = 0; j < sorted_lists[i].size - 1; j++)
            printf("%d, ", sorted_lists[i].elem[j]);
        printf("%d}\n", sorted_lists[i].elem[sorted_lists[i].size - 1]);
        n += sorted_lists[i].size;
    }
    putchar('\n');
    
    all_sorted = (int *) malloc(n * sizeof(int));
    
    merge(sorted_lists, k, all_sorted);
    
    printf("The whole sorted list is below:\nA = {");
    for (i = 0; i < n - 1; i++)
        printf("%d, ", all_sorted[i]);
    printf("%d}\n", all_sorted[n - 1]);
    
    free(all_sorted);
    for (i = 0; i < k; i++)
        free(sorted_lists[i].elem);
    free(sorted_lists);
    
    return 0;
}

