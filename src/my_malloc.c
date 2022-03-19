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

void *my_calloc(size_t n, size_t size)
{
  return my_malloc(size * n);
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

/* test function */

void dealloc(void)
{
  heap.dealloc(&heap);
}

void print_alloced_chunks(void)
{
 printf("Alloced chunks:\n");
 map_print(&heap.alloced_chunks);
}

void dump_alloced_chunks(chunk_t *array_to_dump[])
{
  map_dump(&heap.alloced_chunks, array_to_dump);
}

page_t *get_heap_pages(void)
{
  return heap.pages;
}

size_t get_heap_os_page_size()
{
  return heap.os_page_size;
}

void print_heap_pages(void)
{
  print_pages(heap.pages);
}

u_int8_t get_bin_index(size_t size)
{
  return heap.bin.get_index(size);
}

chunk_t **get_heap_bin(void)
{
  return heap.bin.table;
}

chunk_t *find_alloc(void *pointer)
{
 return map_get(&heap.alloced_chunks, pointer);
}

chunk_t *add_to_bin(chunk_t *chunk)
{
  return heap.bin.add(&heap.bin, chunk);
}

chunk_t *find_freed_chunk(chunk_t *chunk)
{
 return heap.bin.find_by_chunk(&heap.bin, chunk);
}

chunk_t *find_freed_pointer(void *pointer)
{
return heap.bin.find_by_pointer(&heap.bin, pointer);
}

void print_freed(void)
{
  heap.bin.print(&heap.bin);
}

chunk_t *move_from_alloced_chunks(void *pointer)
{
 return map_move(&heap.alloced_chunks, pointer);
}


