#include "../include/heap.h"

static void *alloc(heap_t *heap, size_t size);
static void dealloc(heap_t *heap);
static void print_dump(heap_t *heap);
static void dump(heap_t *heap, chunk_t *array_to_dump[]);

void initialize_heap(heap_t *heap, size_t size)
{
  heap->alloc               = alloc;
  heap->dealloc             = dealloc; 
  heap->dump                = dump;
  heap->print_dump          = print_dump;
  heap->os_page_size        = sysconf(_SC_PAGESIZE);
  heap->next_allocation     = 0x00; // first allocation starts at addr 0

  /* create first page */
  heap->pages = create_page(heap->next_allocation, size, heap->os_page_size);
  heap->next_allocation = (void*)((int8_t*)heap->pages->buffer + 1);
  
  /* init hash table for alloced chunks */
  init_map(&heap->alloced_chunks);
}

static void *alloc(heap_t *heap, size_t size)
{
   if(size == 0)
     return NULL;

  page_t *page = find_page(heap->pages, size);

  // if page not aviable create a new one
  if(!page)
  {
    page = create_page(heap->next_allocation, size, heap->os_page_size);
    heap->pages = add_page(heap->pages, page);
    heap->next_allocation = (void*)((int8_t*)page->buffer + 1);
  }
  
  chunk_t *chunk                       =  (chunk_t*)((int8_t*)page->buffer + page->size);
  void *pointer                        =  ((int8_t*)page->buffer) + (sizeof(chunk_t) + page->size);
  page->size                          += (u_int32_t)(size + sizeof(chunk_t));
 
  initialize_chunk(chunk, pointer, size);
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

  page_t *page = heap->pages;

  while(page)
    {
      munmap(page->buffer, page->capacity);
      page = page->next;
    }
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
