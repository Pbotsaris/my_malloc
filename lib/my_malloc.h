#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include "../include/heap.h"
void *my_malloc(size_t size);
void my_free(void *pointer);
void *my_realloc(void *pointer, size_t size);
void *my_calloc(size_t n, size_t size);

/* TESTING */
void dealloc(void);
void print_alloced_chunks(void);
void dump_alloced_chunks(chunk_t *array_to_dump[]);
page_t *get_heap_pages(void);
size_t get_heap_os_page_size();
void print_heap_pages(void);
u_int8_t get_bin_index(size_t size);
chunk_t *add_to_bin(chunk_t *chunk);
chunk_t **get_heap_bin(void);
chunk_t *find_freed_chunk(chunk_t *chunk);
chunk_t *find_freed_pointer(void *pointer);
void print_freed(void);
chunk_t *find_alloc(void *pointer);
chunk_t *move_from_alloced_chunks(void *pointer);
#endif
