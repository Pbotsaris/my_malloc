#include "../include/chunks.h"

static int add_chunk(chunkrepo_t *chunks, void *pointer, size_t size);

void initialize_chunks(chunkrepo_t *chunks, chunk_t *chunks_pointer)
{
  chunks->chunks                      = chunks_pointer;   
  chunks->add_chunk                   = add_chunk;
  chunks->count                       = 0;
  chunks->location_index              = CHUNKS_CAPACITY - 1;
  chunks->capacity                    = CHUNKS_CAPACITY;
  int index                           = chunks->capacity;
  
  for(size_t i = 0; i < chunks->capacity  ; i++)
  {
      index--;
      chunks->available_locations[i]  = index;
  }
}

static int add_chunk(chunkrepo_t *chunks, void *pointer, size_t size)
{
  int location                       = chunks->available_locations[chunks->location_index];
  chunks->chunks[location].key       = pointer;
  chunks->chunks[location].pair      = size;
  chunks->chunks[location].location  = location;
  chunks->chunks[location].next      = NULL;
  chunks->location_index--;
  chunks->count++;

  return location;

}

