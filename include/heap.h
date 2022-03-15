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

typedef struct heap
{
  void          *next_allocation;
  page_t        *pages;
  size_t        os_page_size;
  map_t         alloced_chunks;
  bin_t         bin;

  void         (*create_page)(struct heap*, page_t, size_t);
  void         *(*alloc)(struct heap*, size_t);
  void         (*free)(struct heap*, void*);
  void         (*print_dump) (struct heap*);
  void         (*dump) (struct heap*, chunk_t*[]);
  void         (*dealloc)(struct heap*);
 }heap_t;

void initialize_heap(heap_t *heap, size_t size);

#endif


/* 
*  NOTE: you have a problem with garbage collection. You need to be able to identify all the freed chunks in the bin for a page
*        in order to return it to the os.
*/
