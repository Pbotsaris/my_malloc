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

#include "../include/chunks.h"

#define T_SIZE 24
#define HASH 5381

/*
 *
 *      This hash table is allocated in the stack using the chunk_table and chunks arrays. 
 *      the hash_table[T_SIZE] array stores the top of linked lists while chunks[CHUNKS_CAPACITY] all linked list items.
 *       
 *      The keys of the hash table stores the pointer to a chunk of memory and the pair the size of the chunk.
 *      The chunk structs is an entry in the hash map.

 */


typedef struct map {
 chunk_t           *chunk_table[T_SIZE];
}map_t;

void init_map(map_t *map);
void map_insert(map_t *map, chunk_t *chunk);
chunk_t *map_get(map_t *map, void *pointer);
chunk_t *map_move(map_t *map, void *pointer);
void map_print(map_t *map);
void map_dump(map_t *map, chunk_t *array_to_dump[]);
#endif
