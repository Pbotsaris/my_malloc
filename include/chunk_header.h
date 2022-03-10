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
  int size;
  struct chunk *next;
}chunk_t;

#endif
