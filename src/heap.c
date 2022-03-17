#include "../include/heap.h"

/* API */
static void *alloc(heap_t *heap, size_t size);
static void hfree(heap_t *heap, void *pointer);

/* TEST API */
static void dealloc(heap_t *heap);
static void print_dump(heap_t *heap);
static void dump(heap_t *heap, chunk_t *array_to_dump[]);

/* helpers */
static void collect(heap_t *heap);
static void *alloc_from_bin(heap_t *heap, size_t size);
static void *alloc_from_page(heap_t *heap, page_t *page, size_t size);
static size_t verify_size(heap_t *heap, size_t size);

static page_t *select_heap_page(heap_t *heap, page_t *page, size_t size);
static void remove_page_chunks_from_bin(heap_t *heap, page_t *page);

void initialize_heap(heap_t *heap, size_t size)
{
  heap->alloc               = alloc;
  heap->dealloc             = dealloc; 
  heap->dump                = dump;
  heap->free                = hfree;
  heap->print_dump          = print_dump;
  heap->os_page_size        = sysconf(_SC_PAGESIZE);   
  heap->next_allocation     = 0x00;                   

  /* create first page */
  heap->pages = create_page(heap->next_allocation, size, heap->os_page_size);
  heap->next_allocation = (void*)((int8_t*)heap->pages->buffer + (heap->pages->capacity + 1));
  
  /* init hash table for alloced chunks */
  initialize_map(&heap->alloced_chunks);

  /* init bin for freed chunks */
  initialize_bin(&heap->bin);
}

static void *alloc(heap_t *heap, size_t size)
{
   if(size == 0)
     return NULL;

  page_t *page             = find_page(heap->pages, (size + sizeof(chunk_t)));
  void *pointer            = NULL;
  size                     = verify_size(heap, size);

   if(!page)
      pointer = alloc_from_bin(heap, size);

    if(!pointer)
      pointer = alloc_from_page(heap, page, size);

   return pointer;
  }

static void hfree(heap_t *heap, void *pointer)
{
  chunk_t *freed_chunk                = map_move(&heap->alloced_chunks, pointer);
  heap->bin.add(&heap->bin, freed_chunk);

  collect(heap);
  freed_chunk->page->alloced_count   -= 1;
}


static void *alloc_from_page(heap_t *heap, page_t *page, size_t size)
{

  page = select_heap_page(heap, page, size);
   
  chunk_t *chunk                       =  (chunk_t*)((int8_t*)page->buffer + page->size);
  void *pointer                        =  ((int8_t*)page->buffer) + (sizeof(chunk_t) + page->size);
  page->size                          += (u_int32_t)(size + sizeof(chunk_t));
  page->alloced_count                 += 1;  
  chunk->page                          = page;
 
  initialize_chunk(chunk, pointer, size);
  map_insert(&heap->alloced_chunks, chunk);
  add_chunk_to_page(chunk, page);

  return pointer;
}


static void *alloc_from_bin(heap_t *heap, size_t size)
{
  (void)heap;
  (void)size;
  return NULL;
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

/* HEPERS */

static void collect(heap_t *heap)
{
  page_t *page = heap->pages;

  while(page)
    {
      if(page->alloced_count == 0)
      {
        remove_page_chunks_from_bin(heap, page);
        heap->pages = remove_page(heap->pages, page);
        munmap(page->buffer, page->capacity);
      }

   page = page->next;
   }
}


static size_t verify_size(heap_t *heap, size_t size)
{
  if(size > FIXED_SIZE)
     return size;

 int fixed_sizes[] = {4, 8, 16, 32, 64, 128, 256, 512};

  u_int8_t index = heap->bin.get_index(size);

  return fixed_sizes[index];
}


static void remove_page_chunks_from_bin(heap_t *heap, page_t *page)

{
  chunk_t *chunk = page->chunks;

  while(chunk)
  {
    if(chunk->prev)
      chunk->prev->next = chunk->next;
    else
      heap->bin.remove_from_head(&heap->bin, chunk);
    
    chunk = chunk->next_in_page;
  }

}

static page_t *select_heap_page(heap_t *heap, page_t *page, size_t size)
{
  /* allocated new page if no suitable is avail */
  if(!page)
  {
    page = create_page(heap->next_allocation, size, heap->os_page_size);
    heap->pages = add_page(heap->pages, page);
    heap->next_allocation = (void*)((int8_t*)page->buffer + (page->capacity + 1));
  }

  return page;
}
