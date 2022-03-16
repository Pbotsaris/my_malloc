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

/* functions bellow are used for testing only */

void dealloc(void)
{
  heap.dealloc(&heap);
}

void print_dump(void)
{
  heap.print_dump(&heap);
}

void dump(chunk_t *array_to_dump[])
{
  heap.dump(&heap, array_to_dump);
}

page_t *get_heap_pages(void)
{
  return heap.pages;
}

size_t get_heap_os_page_size()
{
  return heap.os_page_size;
}

void list_heap_pages(void)
{
  list_pages(heap.pages);
}

u_int8_t get_bin_index(size_t size)
{
  return heap.bin.get_index(size);
}

chunk_t *find_alloc(void *pointer)
{
 return map_get(&heap.alloced_chunks, pointer);
}

chunk_t *add_to_bin(chunk_t *chunk)
{
  return heap.bin.add(&heap.bin, chunk);
}

chunk_t *find_freed(chunk_t *chunk)
{
 return heap.bin.find(&heap.bin, chunk);
}

void print_freed(void)
{
  heap.bin.print(&heap.bin);
}


chunk_t *move_from_alloced_chunks(void *pointer)
{
 return map_move(&heap.alloced_chunks, pointer);
}

