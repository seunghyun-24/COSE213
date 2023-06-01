#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index){
    
    int parent_index = (index-1)/2;
    if(heap->compare(heap->heapArr[index], heap->heapArr[parent_index])>0){
        void *temp;
        temp = heap->heapArr[parent_index];
        heap->heapArr[parent_index] = heap->heapArr[index];
        (heap->heapArr[index]) = temp;
        _reheapUp(heap, parent_index);
    }
    else return;

};


/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index){
    
    if(heap_Empty(heap)==1) return;


    int ind = 0;
    if(heap->last >= index*2 +2){
        if( heap->compare(heap->heapArr[2*index+1], heap->heapArr[2*index+2]) >= 0){
           ind = 2*index+1;
        }
        else ind = 2*index+2;
    }
    else ind = 2*index+1;

    if(heap->last < ind ) return;

    if( heap->compare( heap->heapArr[ind], heap->heapArr[index] ) >= 0){
        
        void *temp =  (heap->heapArr[index]);
        (heap->heapArr[index]) = (heap->heapArr[ind]);
        (heap->heapArr[ind]) = temp;
        _reheapDown(heap, ind);
    }

};


HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2)){
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));

    heap->heapArr = malloc(capacity * sizeof(int *));
    heap->last = -1;
    heap->capacity = capacity;
    heap->compare = compare;
   
    return heap;

};

void heap_Destroy( HEAP *heap){
    
    if(heap_Empty(heap) != 1){
        void *dataPtr;
        while (!heap_Empty( heap)){
            heap_Delete( heap, (void **)&dataPtr);
            free(dataPtr);
        }
    }

    free(heap->heapArr);
    free(heap);

};

int heap_Insert( HEAP *heap, void *dataPtr){
    
    if( heap->capacity-1 == heap->last ){
        heap->capacity = heap->capacity*2;
        heap->heapArr = realloc(heap->heapArr, (heap->capacity) * sizeof(int *));

        if(heap->heapArr==NULL){ printf("ERROR"); return 0; }
    }

    heap->last++;
    (heap->heapArr[heap->last]) = dataPtr;
    _reheapUp(heap, heap->last);
    return 1;

};

int heap_Delete( HEAP *heap, void **dataOutPtr){
    if(heap_Empty(heap) == 1) return 0;

    *(dataOutPtr) = heap->heapArr[0];
    heap->heapArr[0] = heap->heapArr[heap->last];
    heap->last--;
    _reheapDown(heap, 0);
    
    return 1;
};

int heap_Empty(  HEAP *heap){
    if(heap->last == -1) return 1;
    else return 0;
};

void heap_Print( HEAP *heap, void (*print_func) (void *data)){
    
    for(int i=0; i <= heap->last; i++){
        print_func(heap->heapArr[i]);
    }
    printf("\n");

};
