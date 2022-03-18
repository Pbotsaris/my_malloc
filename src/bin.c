#include "../include/bin.h"

static u_int8_t get_index(size_t size);
static chunk_t *add(bin_t *bin, chunk_t *chunk);
static void remove_from_head(bin_t *bin,  chunk_t *chunk);
static chunk_t *move(bin_t *bin, size_t size);

/* used for testing and bin visualization only */
static chunk_t *find_by_chunk(bin_t *bin, chunk_t *chunk);
static chunk_t *find_by_pointer(bin_t *bin, void *pointer);
static void print(bin_t *bin);

/* helpers */
static bool found_suitable(chunk_t *current_chunk, size_t size);
static chunk_t *handle_ranged_size(chunk_t *current, chunk_t *root);
static chunk_t *handle_fixed_size(chunk_t *current, chunk_t *root);

const char *BIN_SIZE_LABELS[18] =
{
  /* fixed */
  "4", "8", "16", "32",
  "128", "256", "512",
   /* ranged */
  "513 - 1024",
  "1025 - 2048" ,
  "2049 - 4096" ,
  "4097 - 8192" ,
  "8193 - 16384" ,
  "16385 - 32768",
  "32769 - 65536" ,
  "65537 - 131072" ,
  "131073 - 262144" ,
  "262145 - 409600",
  "MAX"
};
  
void initialize_bin(bin_t *bin)
{
   bin->get_index           = get_index;
   bin->add                 = add;
   bin->move                = move;
   bin->find_by_chunk       = find_by_chunk;
   bin->find_by_pointer     = find_by_pointer;
   bin->print               = print;
   bin->remove_from_head    = remove_from_head;
}

static chunk_t *add(bin_t *bin, chunk_t *chunk)
{
  u_int8_t index           = get_index(chunk->size);
  chunk_t *current         = bin->table[index] ;
  chunk_t *next            = NULL;

  if(!current)
    {
       bin->table[index] = chunk;
       chunk->next       = NULL;
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

static chunk_t *move(bin_t *bin, size_t size)
{
  u_int8_t index           = get_index(size);
  chunk_t *current         = bin->table[index] ;
  
  /* fixed sizes returns first on the list */
  if(size <= FIXED_SIZE)
     return handle_fixed_size(current, bin->table[index]);

  while(current)
  {
     if(found_suitable(current, size))
        return handle_ranged_size(current, bin->table[index]);

     current = current->next;
   }

  return NULL;
}


static void remove_from_head(bin_t *bin,  chunk_t *chunk)
{
   u_int8_t index           = get_index(chunk->size);
   
  bin->table[index]        = bin->table[index]  ?  bin->table[index]->next : NULL;

   if(bin->table[index])
      bin->table[index]->prev  = NULL;
}

/* FUNCTIONS FOR TESTING */

/* this function scans for the whole bin for a pointer 
*  it is used for testing only and does not affect performance
*/

static chunk_t *find_by_chunk(bin_t *bin, chunk_t *chunk)
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


static chunk_t *find_by_pointer(bin_t *bin, void *pointer)
{
  for(int i = 0; i < BIN_SIZE; i++)
  {
    chunk_t *current = bin->table[i];

    while(current)
      {

      if(current->pointer == pointer)
        return current;

      current = current->next;
      }
  }
  return NULL;
}


static void print(bin_t *bin)
{
  for(int i = 0; i < BIN_SIZE; i++)
  {
    chunk_t *chunks = bin->table[i];

      printf("%s: ", BIN_SIZE_LABELS[i]);

      while(chunks)
      {
      printf("[addr: %p, s: %d ]", chunks->pointer, chunks->size);
      if(chunks->next)
       printf(" -> ");
      chunks = chunks->next;
      }
      printf(" \n");
  }
}


static u_int8_t get_index(size_t size)
{
 u_int8_t result = ((u_int8_t)ceil(log((double)size) / log(2))) - OFFSET;

  /* larger always on last index */
  if(result >= BIN_SIZE - 1)
     result = BIN_SIZE - 1;

  return result;
}

/* HELPERS */

static bool found_suitable(chunk_t *current_chunk, size_t size)
{
  return current_chunk->size <= size && (!current_chunk->next || current_chunk->size > size);
}


static chunk_t *handle_ranged_size(chunk_t *current, chunk_t *root)
{
  if(current->prev)
      current->prev->next = current->next;

  if(!current->prev)
  {
    root = current->next;

    if(root)
      root->prev = NULL;
  }

  if(current->next)
     current->next->prev = current->prev;

  current->prev = NULL;

  return current;
}


static chunk_t *handle_fixed_size(chunk_t *current, chunk_t *root)
{
     root       = current->next;
     root->prev = NULL;
     return current;
}



