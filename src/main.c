#include "../include/heap.h"

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

int main(void)
{

heap.initialized = false;

char *pointer = (char*)my_malloc(20);
my_malloc(30);

heap.dump(&heap);

  return 0;
}
