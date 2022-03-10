#include "../include/heap.h"

static void *alloc(heap_t *heap, size_t size);
static void dealloc(heap_t *heap);
static void print_dump(heap_t *heap);
static void dump(heap_t *heap, chunk_t *array_to_dump[]);

void initialize_heap(heap_t *heap, size_t size)
{
  init_map(&heap->alloced_chunks);
  heap->alloc               = alloc;
  heap->dealloc             = dealloc; 
  heap->dump                = dump;
  heap->print_dump          = print_dump;
  heap->size                = 0;
  heap->capacity            = (size_t) sysconf(_SC_PAGESIZE) * NUM_OF_PAGES;
  heap->heap                = (void*) mmap(0, heap->capacity , PROT_READ | PROT_WRITE,
                                           MAP_ANON | MAP_PRIVATE, -1, 0); 
}

static void *alloc(heap_t *heap, size_t size)
{
   if(size == 0)
    return NULL;
  
  chunk_t *chunk                       =  (chunk_t*)(heap->heap + heap->size);
  void *pointer                         =  heap->heap + (sizeof(chunk_t) + heap->size + 10);
  heap->size                           += (size + sizeof(chunk_t) + 10);

  chunk->pointer                      = pointer;
  chunk->size                         = (int)size; 
  chunk->next                         = NULL;

  map_insert(&heap->alloced_chunks, chunk);

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
