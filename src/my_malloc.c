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

 // heap.size = size + sizeof(chunk_t);
//  printf("outside:::: heap->size: %lu\n", heap.size);
  return pointer;
}

void dealloc(void)
{
  heap.dealloc(&heap);
}

/* functions bellow are used for testing only */

void print_dump(void)
{
  heap.print_dump(&heap);
}

void dump(chunk_t *array_to_dump[])
{
  heap.dump(&heap, array_to_dump);
}

chunk_t *find_alloc(void *pointer)
{
 return map_get(&heap.alloced_chunks, pointer);
}


chunk_t *move_from_alloced_chunks(void *pointer)
{
 return map_move(&heap.alloced_chunks, pointer);
}



