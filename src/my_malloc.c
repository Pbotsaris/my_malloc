#include "../include/my_malloc.h"

heap_t heap;

static bool has_invalid_size(size_t size);
static bool was_not_initialized(heap_t *heap);
static bool is_null(heap_t *pointer);

/* public API */
  
void *my_malloc(size_t size)
{
  if(has_invalid_size(size))
    return NULL;
   
  if(!heap.alloc)
      initialize_heap(&heap, size);

  return heap.alloc(&heap, size);
}

void *my_realloc(void *pointer, size_t size)
{
  if(is_null(pointer) || was_not_initialized(&heap) || has_invalid_size(size))
    return NULL;

  return heap.realloc(&heap, pointer ,size);
}

void my_free(void *pointer)
{
  if(is_null(pointer) || was_not_initialized(&heap))
     return;

  heap.free(&heap, pointer);
}

/* private */
static bool has_invalid_size(size_t size)  {return size == 0 || size >= MAX_SIZE; } 
static bool was_not_initialized(heap_t *heap)  {return !heap->alloc; }
static bool is_null(heap_t *pointer)  {return !pointer; }

