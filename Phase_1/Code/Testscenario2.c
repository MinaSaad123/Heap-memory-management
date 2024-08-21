#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dynamic_Allocation_APIs.h"

#define NUM_ALLOCS 10000
#define MAX_SIZE 10240
#define MAX_ITERATIONS 100000
extern char MyHeap[];
extern FreeBlockNode_t* HeadNode;

void random_alloc_free_test() 
{
    srand((unsigned int)time(NULL));
    
    void* pointers[NUM_ALLOCS] = {NULL};
    
    for (int i = 0; i < MAX_ITERATIONS; ++i) 
    {
        int index = rand() % NUM_ALLOCS;
        if (pointers[index] == NULL)
        {
            // Allocate memory
            size_t size = (size_t)(rand() % MAX_SIZE) + 1;
            printf("%u\n", size);
            pointers[index] = Hmm_Alloc(size);
            if (pointers[index] != NULL)
            {
                printf("Allocated memory of size %zu at address %p\n", size, pointers[index]);
            } 
        } else 
        {
            // Free memory
            printf("Freeing memory at address %p\n", pointers[index]);
            Hmm_free(pointers[index]);
            pointers[index] = NULL;
        }
    }
    
    // Free remaining allocated memory
    for (int i = 0; i < NUM_ALLOCS; ++i) 
    {
        if (pointers[i] != NULL) 
        {
            printf("Freeing remaining memory at address %p\n", pointers[i]);
            Hmm_free(pointers[i]);
            pointers[i] = NULL;
        }
    }
}

int main() 
{
    FreeBlockNode_t* Temp ;
    printf("%p   %p\n", MyHeap, ( (char*)MyHeap + MyHeapSize) );
    printf("Starting random allocation and deallocation test...\n");
    random_alloc_free_test();
    printf("Test complete.\n");
}
