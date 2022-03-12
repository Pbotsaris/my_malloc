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
  void *pointer;
  u_int32_t size;
  struct chunk *next;
}chunk_t;


void initialize_chunk(chunk_t *chunk, void *pointer, size_t size);

#endif
