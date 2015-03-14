
#define LEFT(i)		((i << 1) | 1)
#define RIGHT(i)	((i << 1) + 2)

void build_max_heap(int *A, unsigned int n)
{
    unsigned int i;
    void max_heapify(int *A, unsigned int i, unsigned int n);
    for (i = (n >> 1) - 1; i < n; i--)
        max_heapify(A, i, n);
}

void heapsort(int *A, unsigned int n)
{
    unsigned int heap_size, i;
    void max_heapify(int *A, unsigned int i, unsigned int n);

    build_max_heap(A, n);
    heap_size = n;
    for (i = n - 1; i > 0; i--) {
        A[i] ^= A[0];
        A[0] ^= A[i];
        A[i] ^= A[0];
        max_heapify(A, 0, --heap_size);
    }
}

int is_max_heap(int *A, unsigned int n)
{
    unsigned int i, l, r;
    const unsigned int m = n >> 1;

    for (i = 0; i < m; i++) {
        l = LEFT(i);
        if (A[i] < A[l])
            return 0;
        r = RIGHT(i);
        if (r < n && A[i] < A[r])
            return 0;
    }
    return 1;
}

void max_heapify(int *A, unsigned int i, unsigned int n)
{
    unsigned int j, l, r, largest_id;
    int key, largest;
    int stop;

    j = i;
    key = A[i];
    do {
        stop = 1;

        l = LEFT(j);
        if (l < n && A[l] > key) {
            largest = A[l];
            largest_id = l;
        }
        else {
            largest = key;
            largest_id = j;
        }

        r = RIGHT(j);
        if (r < n && A[r] > largest) {
            largest = A[r];
            largest_id = r;
        }

        if (largest_id != j) {
            A[j] = largest;
            j = largest_id;
            stop = 0;
        }
    } while(!stop);

    A[j] = key;
}

