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
static void insert(map_t *map, chunk_t *chunk);
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

  for(int i = 0; i < T_SIZE; i++)
    map->chunk_table[i] = NULL;
 
}



/* PUBLIC METHODS */
static void insert(map_t *map, chunk_t *chunk)
{
  unsigned int slot    = hash(chunk->key);
  chunk_t *entry       = map->chunk_table[slot];

  /* if nothing at head add to head */
  if(!entry)
  {
    map->chunk_table[slot] = chunk;
    return;
  }

  while(entry != NULL)
  {
    /* match keys then update size */
    if(entry->key == chunk->key)
    {
      entry->pair = chunk->pair;
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

/**/

static bool move(map_t *map, chunk_t *chunk, void *pointer)
{
  chunk->key            = pointer;
  unsigned int slot     = hash(chunk);
  chunk_t *prev         = NULL;
  chunk_t *entry        = map->chunk_table[slot];

  if(entry == NULL)
    return false;

  while(entry != NULL)
  {
    if(entry->key == pointer)
    {
      if(prev)
        prev->next = entry->next;
      else
        /* if deleting first item of linked list */
        map->chunk_table[slot] = entry->next;

     // chunk returned in the first second argument 
      chunk->key = entry->key;
      chunk->pair = entry->pair;
    }
    prev = entry;
    entry = entry->next;
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
    chunk_t *entry = map->chunk_table[i];

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
 

