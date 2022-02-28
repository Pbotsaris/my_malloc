#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define CHUNKS_CAPACITY 1024

typedef struct chunk {
  void *key;
  size_t pair;
  int location;
  struct chunk *next;
}chunk_t;

typedef struct chunkrepo
{
  chunk_t      *chunks; /* chunks will be stored at the end of the heap */
  size_t        capacity;
  int           available_locations[CHUNKS_CAPACITY];
  int           location_index;
  size_t        count;

  int          (*add_chunk)(struct chunkrepo*, void*, size_t);
}chunkrepo_t;


void initialize_chunks(chunkrepo_t *chunks, chunk_t *chunks_pointer);

#endif
