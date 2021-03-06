#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "../include/page.h"
#include "../include/hash_table.h"
#include "../include/bin.h"

#define NUM_OF_PAGES 1
#define MAX_SIZE 4294967295
#define PAGE_SIZE 4096

typedef struct heap
{
  void          *next_allocation;
  page_t        *pages;
  size_t        os_page_size;
  map_t         alloced_chunks;
  bin_t         bin;

  void         (*create_page)(struct heap*, page_t, size_t);
  void         *(*alloc)(struct heap*, size_t);
  void         *(*realloc)(struct heap*, void*, size_t);
  void         (*free)(struct heap*, void*);
  void         (*dealloc)(struct heap*);
 }heap_t;

void initialize_heap(heap_t *heap, size_t size);

#endif

