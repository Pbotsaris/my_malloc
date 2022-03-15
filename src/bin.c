#include "../include/bin.h"

static u_int8_t get_index(size_t size);
static chunk_t *add(bin_t *bin, chunk_t *chunk);
chunk_t *find(bin_t *bin, chunk_t *chunk);

void initialize_bin(bin_t *bin)
{
   bin->get_index        = get_index;
   bin->add              = add;
   bin->find             = find;
}

static chunk_t *add(bin_t *bin, chunk_t *chunk)
{
  u_int8_t index         = get_index(chunk->size);
  chunk_t *current       = bin->table[index] ;
  chunk_t *next          = NULL;

  if(!current)
    {
       bin->table[index] = chunk;
       return bin->table[index];
    }

  while(current)
  {

    if(!current->next)
    {
        current->next  = chunk;
        chunk->prev    = current;
        chunk->next    = NULL;
        break;
    }
     /* inserting to list sorted by size */
    if(chunk->size >= current->size && chunk->size <= current->next->size)
    {
       next           = current->next;
       next->prev     = chunk;
       current->next  = chunk;
       chunk->prev    = current;
       chunk->next    = next;
       break;
    }

    current          = current->next;

  }
  return bin->table[index];
}

/* this function scans for the whole bin for a pointer 
*  it is used for testing only and does not affect performance
*/
chunk_t *find(bin_t *bin, chunk_t *chunk)
{
  for(int i = 0; i < BIN_SIZE; i++)
  {
     chunk_t *current = bin->table[i];

    while(current)
      {

      if(current->pointer == chunk->pointer)
        return current;

      current = current->next;
      }
  }
  return NULL;
}

static u_int8_t get_index(size_t size)
{
 u_int8_t result = ((u_int8_t)ceil(log((double)size) / log(2))) - OFFSET;

  /* larger always on last index */
  if(result >= BIN_SIZE - 1)
     result = BIN_SIZE - 1;

  return result;
}

