#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void* array;
    int size;
    int capacity;
    size_t elementSize;
} ArrayList;

ArrayList* createArray(size_t elementSize) {
    ArrayList* arr = (ArrayList*)malloc(sizeof(ArrayList));
    arr->capacity = 2;
    arr->size = 0;
    arr->elementSize = elementSize;
    arr->array = malloc(arr->capacity * elementSize);
    return arr;
}

int addToList(ArrayList* arr, void* element) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->array = realloc(arr->array, arr->capacity * arr->elementSize);
    }

    int newIndex = arr->size;
    void* target = (char*)arr->array + (arr->size * arr->elementSize);

    memcpy(target, element, arr->elementSize);
    arr->size++;

    return newIndex;
}

void* get(ArrayList* arr, int index) {
    if (index < 0 || index >= arr->size) {
        printf("Index out of bounds\n");
        exit(1);
    }
    return (char*)arr->array + (index * arr->elementSize);
}

void freeArray(ArrayList* arr) {
    free(arr->array);
    free(arr);
}
