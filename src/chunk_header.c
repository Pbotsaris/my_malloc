#include "../include/chunk_header.h"



void initialize_chunk(chunk_t *chunk, void *pointer, size_t size)
{
  chunk->pointer                      = pointer;
  chunk->size                         = (u_int32_t)size; 
  chunk->next                         = NULL;
  chunk->prev                         = NULL;
  chunk->next_in_page                 = NULL;
}


void add_chunk_to_page(chunk_t *chunk, page_t *page)
{
  chunk_t *temp = page->chunks;

  if(!page->chunks)
  {
    page->chunks = chunk;
    return;
  }

  while(temp)
    {
      if(!temp->next_in_page) 
      {
        temp->next_in_page = chunk;
        break;
      }

      temp = temp->next_in_page;
    }
}

void remove_page_chunks_from_bin(page_t *page)
{
  chunk_t *chunk = page->chunks;

  while(chunk)
  {

    if(chunk->prev)
      chunk->prev->next = chunk->next;
    
    chunk = chunk->next_in_page;
  }
}

