#include "../include/malloc_test_api.h"

heap_t heap_test;

void *malloc_test(size_t size)
{
  static bool initialized = false;

  if(!initialized)
    {
      initialize_heap(&heap_test, size);
      initialized = true;
    }

   void *pointer = heap_test.alloc(&heap_test, size);

  return pointer;
}

void free_test(void *pointer)
{
  heap_test.free(&heap_test, pointer);
}

void dealloc(void)
{
  heap_test.dealloc(&heap_test);
}


void print_alloced_chunks(void)
{
 printf("Alloced chunks:\n");
 map_print(&heap_test.alloced_chunks);
}

void dump_alloced_chunks(chunk_t *array_to_dump[])
{
  map_dump(&heap_test.alloced_chunks, array_to_dump);
}

page_t *get_heap_pages(void)
{
  return heap_test.pages;
}

size_t get_heap_os_page_size()
{
  return heap_test.os_page_size;
}

void print_heap_pages(void)
{
  print_pages(heap_test.pages);
}

u_int8_t get_bin_index(size_t size)
{
  return heap_test.bin.get_index(size);
}

chunk_t **get_heap_bin(void)
{
  return heap_test.bin.table;
}

chunk_t *find_alloc(void *pointer)
{
 return map_get(&heap_test.alloced_chunks, pointer);
}

chunk_t *add_to_bin(chunk_t *chunk)
{
  return heap_test.bin.add(&heap_test.bin, chunk);
}

chunk_t *find_freed_chunk(chunk_t *chunk)
{
 return heap_test.bin.find_by_chunk(&heap_test.bin, chunk);
}

chunk_t *find_freed_pointer(void *pointer)
{
return heap_test.bin.find_by_pointer(&heap_test.bin, pointer);
}

void print_freed(void)
{
  heap_test.bin.print(&heap_test.bin);
}

chunk_t *move_from_alloced_chunks(void *pointer)
{
 return map_move(&heap_test.alloced_chunks, pointer);
}

