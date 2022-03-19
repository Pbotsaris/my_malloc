#include "../include/heap.h"

/* API */
static void *alloc(heap_t *heap, size_t size);
static void hfree(heap_t *heap, void *pointer);
static void *ralloc(heap_t *heap, void *pointer, size_t size);
static void dealloc(heap_t *heap);

/* helpers */
static void collect(heap_t *heap);
static void *alloc_from_bin(heap_t *heap, size_t size);
static void *alloc_from_page(heap_t *heap, page_t *page, size_t size);
static size_t verify_size(heap_t *heap, size_t size);

static page_t *select_heap_page(heap_t *heap, page_t *page, size_t size);
static void remove_page_chunks_from_bin(heap_t *heap, page_t *page);
static bool has_same_size(chunk_t *chunk, size_t size);
static bool is_invalid_free(chunk_t *chunk);

void initialize_heap(heap_t *heap, size_t size)
{
  heap->alloc                  = alloc;
  heap->dealloc                = dealloc; 
  heap->realloc                = ralloc;
  heap->free                   = hfree;
  heap->os_page_size           = sysconf(_SC_PAGESIZE);   
  heap->next_allocation        = 0x00;                   

  heap->pages = create_page(heap->next_allocation, size, heap->os_page_size);
  heap->next_allocation = (void*)((int8_t*)heap->pages->buffer + (heap->pages->capacity + 1));
  
  initialize_map(&heap->alloced_chunks);
  initialize_bin(&heap->bin);
}

static void *alloc(heap_t *heap, size_t size)
{
  page_t *page             = find_page(heap->pages, (size + sizeof(chunk_t)));
  void *pointer            = NULL;
  size                     = verify_size(heap, size);

   if(!page)
      pointer             = alloc_from_bin(heap, size);

    if(!pointer)
      pointer             = alloc_from_page(heap, page, size);

   return pointer;
  }

static void *ralloc(heap_t *heap, void *pointer, size_t size)
{
   chunk_t * chunk = map_move(&heap->alloced_chunks, pointer);
   size            = verify_size(heap, size);

    if(!chunk)
      return NULL;

   if(has_same_size(chunk, size))
   {
     map_insert(&heap->alloced_chunks ,chunk);
     return pointer;
   }

   heap->bin.add(&heap->bin, chunk);
   chunk->page->alloced_count   -= 1;

   return alloc(heap, size);
  }

static void hfree(heap_t *heap, void *pointer)
{
  chunk_t *freed_chunk                = map_move(&heap->alloced_chunks, pointer);

  if(is_invalid_free(freed_chunk)) return;

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
   chunk_t *chunk = heap->bin.move(&heap->bin, size);
  
   if(!chunk)
     return NULL;

   map_insert(&heap->alloced_chunks, chunk);

   return chunk->pointer;
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

/* HELPERS */

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
  if(!page)
  {
    page = create_page(heap->next_allocation, size, heap->os_page_size);
    heap->pages = add_page(heap->pages, page);
    heap->next_allocation = (void*)((int8_t*)page->buffer + (page->capacity + 1));
  }

  return page;
}

static bool is_invalid_free(chunk_t *chunk)
{
  if(!chunk)
     fprintf(stderr, "Error: attemping to free invalid pointer\n");

    return chunk == NULL;
}

static bool has_same_size(chunk_t *chunk, size_t size) { return size  == (u_int32_t)chunk->size; }

