#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include "../include/heap.h"

void *my_malloc(size_t size);
void print_dump(void);
void dealloc(void);

/* test helpers */
void dump(chunk_t *array_to_dump[]);
page_t *get_heap_pages(void);
size_t get_heap_os_page_size();
void list_heap_pages(void);

chunk_t *find_alloc(void *pointer);
chunk_t *move_from_alloced_chunks(void *pointer);

#endif
