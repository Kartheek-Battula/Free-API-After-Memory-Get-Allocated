/*
Name :
Date :
Description :
Sample Input :
Sample Output :
*/
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define TOTAL_BLOCKS 128   
#define BLOCK_SIZE 1024    


typedef struct {
    bool is_free;    
    size_t size;     
} Block;

Block heap[TOTAL_BLOCKS];


void init_heap() {
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        heap[i].is_free = true;  
        heap[i].size = 1;        
    }
}


void* my_malloc(size_t size) {
    size_t required_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int best_index = -1;
    size_t best_size = TOTAL_BLOCKS + 1;

    for (int i = 0; i < TOTAL_BLOCKS; ) {
        if (heap[i].is_free) {
            size_t free_blocks = 0;

            while (i + free_blocks < TOTAL_BLOCKS && heap[i + free_blocks].is_free) {
                free_blocks++;
            }

            if (free_blocks >= required_blocks && free_blocks < best_size) {
                best_size = free_blocks;
                best_index = i;
            }
            i += free_blocks;  
        } else {
            i += heap[i].size; 
        }
    }

    if (best_index == -1) {
        return NULL;
    }

    for (size_t i = best_index; i < best_index + required_blocks; i++) {
        heap[i].is_free = false;
    }

    heap[best_index].size = required_blocks;

    return &heap[best_index];
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return;   
    }

    size_t index = (Block*)ptr - heap;

    if (heap[index].is_free) {
        printf("Error: Double free detected!\n");
        return;
    }

    size_t block_size = heap[index].size;
    for (size_t i = index; i < index + block_size; i++) {
        heap[i].is_free = true;
    }

    printf("Memory freed at block index %zu\n", index);
}

int main() {
    init_heap();

    void* ptr1 = my_malloc(sizeof(int) * 128);  
    if (ptr1) {
        printf("Memory allocated at %p\n", ptr1);
    }

    my_free(ptr1);  

    return 0;
}

