#include "../include/my_malloc.h"

heap_t heap;

void *my_malloc(size_t size)
{
  static bool initialized = false;

  if(!initialized)
    {
      initialize_heap(&heap, size);
      initialized = true;
    }

   void *pointer = heap.alloc(&heap, size);

  return pointer;
}

void my_free(void *pointer)
{
  heap.free(&heap, pointer);
}


