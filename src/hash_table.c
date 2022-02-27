/*
 * =====================================================================================
 *
 *       Filename:  hash.c
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  11/15/21 07:42:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/hash_table.h"

/* PUBLIC */
static bool insert(map_t *map, void *pointer, size_t size);
static bool move(map_t *map, chunk_t *chunk, void *ptr);
static void print(map_t *map);

/* PRIVATE */
static unsigned int hash(void *pointer);

/* PUBLIC INITIALIZER */

void init_map(map_t *map)
{

  map->insert           = insert;
  map->move             = move;
  map->print            = print;
  map->location_index   = CHUNKS_CAPACITY;
  map->chunks_count     = 0;

  for(int i = 0; i < T_SIZE; i++)
    {
    chunk_t chunk = { .key = NULL, .pair = 0, .next = NULL };
    map->chunk_table[i] = chunk;
   }

  int index = CHUNKS_CAPACITY;

  for(int i = 0; i < CHUNKS_CAPACITY  ; i++)
  {
      index--;
      map->available_locations[i] = index;
  }

}


/* PUBLIC METHODS */


static bool insert(map_t *map, void *pointer, size_t size)
{
  unsigned int slot    = hash(pointer);
  chunk_t *entry       = &map->chunk_table[slot];

  if(!entry->key)
  {
    map->chunk_table[slot].key = pointer;
    map->chunk_table[slot].pair = size;
    return true;
  }

  if(map->chunks_count >= CHUNKS_CAPACITY - 1)
    {
    printf("alloced chunks capacity exceeded\n");
    return false;
    }

//  printf("location: %i\n", map->available_locations[map->location_index]);
  /* only add to chunks array if not the head of the linked list. */
  chunk_t chunk = {.key = pointer, .pair = size, .next = NULL };
  map->chunks[map->available_locations[map->location_index]] = chunk;


  while(entry != NULL)
  {
    /* match keys then update size */
    if(entry->key == chunk.key)
    {
      entry->pair = chunk.pair;
      break;
    }

    /* no match, add to tail */
    if(!entry->next)
    {
     entry->next = &map->chunks[map->location_index];
     break;
    }
    entry = entry->next;
  }

  map->chunks_count++;
  map->location_index--;

  return true;
}

/**/

static bool move(map_t *map, chunk_t *chunk, void *ptr)
{
  chunk->key            = ptr;
  unsigned int slot     = hash(chunk);
  chunk_t *prev         = NULL;
  chunk_t *entry        = &map->chunks[slot];

  if(entry == NULL)
    return false;

  while(entry != NULL)
  {
    if(entry->key == ptr)
    {
      if(prev)
        /* if deleting first item of linked list */
        prev->next = entry->next;
      else
        map->chunks[slot] = *(entry->next);

      // chunk returned in the first second argument 
      chunk->key = entry->key;
      chunk->pair = entry->pair;
    }
    prev = entry;
    entry = entry->next;
    map->chunks_count--;
    return true;
  }
 //  map->chunks[slot] = NULL;
 return false;
}


/**/

static void print(map_t *map)
{
  for(int i = 0; i < T_SIZE; i++)
  {
    chunk_t *entry = &map->chunk_table[i];

    if(!entry || !entry->key)
      continue;

    while(entry)
    {
        printf("addr: %p |  size: %lu \n", entry->key, entry->pair);
        entry = entry->next;
    }
  }
}

/**/

/* http://www.cse.yorku.ca/~oz/hash.html  */
static unsigned int hash(void *pointer)
{
  char pointer_str[16]      = {0};
  unsigned long hash        = HASH;
  int i                    = 0;

  // convert pointer address for the hash function
  sprintf(pointer_str, "%p", pointer);
 
  while(pointer_str[i] != '\0')
  {
    hash = ((hash << 5) + hash) + (int)pointer_str[i];
    i++;
  }

  return hash % T_SIZE;
}
 

