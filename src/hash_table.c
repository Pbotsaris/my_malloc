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
void map_insert(map_t *map, chunk_t *chunk);
chunk_t *map_get(map_t *map, void *pointer);
chunk_t *map_move(map_t *map, void *pointer);
void map_print(map_t*map);
void map_dump(map_t *map, chunk_t *array_to_dump[]);

/* PRIVATE */
static unsigned int hash(void *pointer);


/* PUBLIC FUNCTIONS */

void init_map(map_t *map)
{
  for(int i = 0; i < T_SIZE; i++)
    map->chunk_table[i] = NULL;
 
}

void map_insert(map_t *map, chunk_t *chunk)
{
  unsigned int slot    = hash(chunk->pointer);
  chunk_t *entry       = map->chunk_table[slot];

  /* if nothing at head add to head */
  if(!entry)
  {
  //  printf("--? slot! %u, chunk: %p\n", slot, chunk->key);
    map->chunk_table[slot] = chunk;
    return;
  }

  while(entry != NULL)
  {
    /* match keys then update size */
    if(entry->pointer == chunk->pointer)
    {
      entry->size = chunk->size;
      break;
    }

    /* no match, add to tail */
    if(!entry->next)
    {
     entry->next = chunk;
     break;
    }

    entry = entry->next;
  }
}

 chunk_t *map_get(map_t *map, void *pointer)
{
  unsigned int slot     = hash(pointer);
  chunk_t *chunk        = map->chunk_table[slot];

  if(!chunk)
      return NULL;

  if(chunk->pointer == pointer)
     return chunk;

  while(chunk)
  {
    if(chunk->pointer == pointer)
        return chunk;

    chunk = chunk->next;
  }
 return NULL;
}


/**/

chunk_t *map_move(map_t *map, void *pointer)
{
  unsigned int slot     = hash(pointer);
  chunk_t *prev         = NULL;
  chunk_t *chunk        = map->chunk_table[slot];

  while(chunk)
  {
    if(chunk->pointer == pointer)
    {
      if(prev)
        prev->next = chunk->next;
      /* if deleting first item of linked list */
      else
        map->chunk_table[slot] = chunk->next;

     // chunk returned in the first second argument 
       return chunk;
    }
    prev = chunk;
    chunk = chunk->next;
  }
 //  map->chunks[slot] = NULL;
 return NULL;
}


/**/

void map_print(map_t *map)
{
  for(int i = 0; i < T_SIZE; i++)
  {
    chunk_t *entry = map->chunk_table[i];

    if(!entry || !entry->pointer)
      continue;

    while(entry)
    {
        printf("addr: %p |  size: %i\n", entry->pointer, entry->size);
        entry = entry->next;
    }
  }
}


void map_dump(map_t *map, chunk_t *array_to_dump[])
{
  int index = 0;

for(int i = 0; i < T_SIZE; i++)
  {
    chunk_t *entry = map->chunk_table[i];

    if(!entry || !entry->pointer)
      continue;

    while(entry)
    {
        array_to_dump[index] = entry;
        entry = entry->next;
        index++;
    }
  }
  array_to_dump[index] = NULL;
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
 

