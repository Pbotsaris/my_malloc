/*
 * =====================================================================================
 *
 *       Filename:  hash_table.h
 *
 *    Description:  Hash map implemented using separate chaining. The size of the table
 *                  can be adjusted via the T_SIZE constant.
 *
 *        Version:  1.0
 *        Created:  11/15/21 07:44:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>

#define T_SIZE 64
#define CHUNKS_CAPACITY 1024
#define HASH 5381

/*
 *
 *      This hash table is allocated in the stack using the chunk_table and chunks arrays. 
 *      the hash_table[T_SIZE] array stores the top of linked lists while chunks[CHUNKS_CAPACITY] all linked list items.
 *       
 *      The keys of the hash table stores the pointer to a chunk of memory and the pair the size of the chunk.
 *      The chunk structs is an entry in the hash map.

 */

typedef struct chunk {
  void *key;
  size_t pair;
  struct chunk *next;
}chunk_t;

typedef struct map {
 chunk_t           chunk_table[T_SIZE];
 chunk_t           chunks[CHUNKS_CAPACITY];
 int               available_locations[CHUNKS_CAPACITY];
 int               location_index;
 size_t            chunks_count;

 bool            (*insert) (struct map*, void*, size_t);                     /* Inserts a chunk to hash map . Ovewrites if same key */
 bool            (*move) (struct map*, chunk_t*, void*);               /* Removes a chunk and copy values to chunk_t*  argument. returns true if successful */
 void            (*print) (struct map*);                               /* Prints the value of every chunk in the hash map. */ 

}map_t;

void init_map(map_t *map);

#endif
