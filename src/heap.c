#include "../include/heap.h"

static void *alloc(heap_t *heap, chunkrepo_t *chunks, size_t size);
static void dealloc(heap_t *heap);
static void print_dump(heap_t *heap);
static void dump(heap_t *heap, chunk_t *array_to_dump[]);

void initialize_heap(heap_t *heap, chunkrepo_t *chunks)
{
  init_map(&heap->alloced_chunks);
  heap->alloc               = alloc;
  heap->dealloc             = dealloc; 
  heap->dump                = dump;
  heap->print_dump          = print_dump;

  heap->size                = 0;
  heap->initialized         = true;
  heap->capacity            = (size_t) sysconf(_SC_PAGESIZE) * NUM_OF_PAGES;
  size_t total_size         = heap->capacity + (CHUNKS_CAPACITY * sizeof(chunk_t));
  heap->heap                = (char*) mmap(0, total_size , PROT_READ | PROT_WRITE,
                                           MAP_ANON | MAP_PRIVATE, -1, 0); 

  initialize_chunks(chunks, (chunk_t*)heap->heap + heap->capacity);
}

static void *alloc(heap_t *heap, chunkrepo_t *chunks, size_t size)
{
   if(size == 0)
    return NULL;

  assert(heap->size + size < heap->capacity);

  void *pointer                         =  heap->heap + heap->size;
  heap->size                           += size;
  int location                          =  chunks->add_chunk(chunks, pointer, size);
  
  map_insert(&heap->alloced_chunks, &chunks->chunks[location]);
  
  return pointer;
}

static void print_dump(heap_t *heap)
{
    printf("Alloced chunks:\n");
    map_print(&heap->alloced_chunks);
}

static void dump(heap_t *heap, chunk_t *array_to_dump[])
{
  map_dump(&heap->alloced_chunks, array_to_dump);
}

static void dealloc(heap_t *heap)
{
  munmap(heap->heap, heap->capacity + (CHUNKS_CAPACITY * sizeof(chunk_t)));
}

//void hfree(void *pointer)
//{
//  assert(false && "TODO: hfree not implemented");
//}
//
//void hcollect()
//{
//  assert(false && "collect not implemented");
//
//}
//
