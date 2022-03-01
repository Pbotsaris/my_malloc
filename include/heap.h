#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "../include/hash_table.h"

#define NUM_OF_PAGES 10

typedef struct heap
{
  char          *heap;
  size_t        size;
  size_t        capacity;
  map_t         alloced_chunks;
  bool          initialized;


  void         *(*alloc)(struct heap*, chunkrepo_t*, size_t);
  void         (*print_dump) (struct heap*);
  void         (*dump) (struct heap*, chunk_t*[]);
  void         (*dealloc)(struct heap*);
 }heap_t;

void initialize_heap(heap_t *heap, chunkrepo_t *chunks);

#endif
