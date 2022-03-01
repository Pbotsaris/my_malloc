#include "../include/my_malloc.h"

heap_t heap;
chunkrepo_t chunks;

void *my_malloc(size_t size)
{
  static bool initialized = false;

  if(!initialized)
      initialize_heap(&heap, &chunks);

  initialized = true;
  return heap.alloc(&heap, &chunks, size);
}

void dealloc(void)
{
  heap.dealloc(&heap);
}

/* functions bellow are used for united testing */

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
 return heap.alloced_chunks.get(&heap.alloced_chunks, pointer);
}


chunk_t *move_from_alloced_chunks(void *pointer)
{
 return heap.alloced_chunks.move(&heap.alloced_chunks, pointer);
}



