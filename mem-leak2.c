#include <stdio.h>
#include <stdlib.h>

void memory_leak_example() {
    int *arr = (int *)malloc(5 * sizeof(int)); // Allocate memory
    for (int i = 0; i < 5; i++) {
        arr[i] = i + 1; // Fill the array
    }
    // Memory is not freed, causing a leak
}

void invalid_memory_access() {
    int *ptr = (int *)malloc(sizeof(int));
    free(ptr); // Free the memory
    *ptr = 42; // Accessing freed memory (use-after-free)
}

void uninitialized_memory() {
    int *arr = (int *)malloc(3 * sizeof(int));
    printf("Value: %d\n", arr[1]); // Using uninitialized memory
    free(arr);
}

int main() {
    memory_leak_example();
    invalid_memory_access();
    uninitialized_memory();
    return 0;
}

