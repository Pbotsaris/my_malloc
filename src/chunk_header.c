#include "../include/chunk_header.h"



void initialize_chunk(chunk_t *chunk, void *pointer, size_t size)
{
  chunk->pointer                      = pointer;
  chunk->size                         = (u_int32_t)size; 
  chunk->next                         = NULL;
}
