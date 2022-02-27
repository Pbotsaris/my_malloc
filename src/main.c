#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "../include/hash_table.h"

#define NUM_OF_PAGES 10
#define HCHUNKS_CAPACITY 1024


typedef struct {
   void *start;
   size_t size;
}hchunk_t;

typedef struct
{
  char *heap;
  map_t alloced_chunks;
//  hchunk_t alloced_chunks[HCHUNKS_CAPACITY];
  size_t alloced_chucks_count;
  size_t size;
  size_t capacity;
}heap_t;


void initialize_heap(heap_t *heap)
{
  init_map(&heap->alloced_chunks);
  heap->capacity = (size_t) sysconf(_SC_PAGESIZE) * NUM_OF_PAGES;
  heap->size = 0;
  heap->heap = mmap(0, heap->capacity, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0); 
}

void *halloc(heap_t *heap, size_t size)
{
   if(size == 0)
    return NULL;

  assert(heap->size + size < heap->capacity);
  void *pointer =  heap->heap + heap->size;
  heap->size += size;

  assert(heap->alloced_chucks_count < CHUNKS_CAPACITY - 1);
  heap->alloced_chunks.insert(&heap->alloced_chunks, pointer, size);
  
  return pointer;
}


void heap_dump_chunks(heap_t *heap)
{
    printf("Alloced chunks:\n");
    heap->alloced_chunks.print(&heap->alloced_chunks);
}

void hfree(void *pointer)
{

  assert(false && "TODO: hfree not implemented");

}

void hcollect()
{
  assert(false && "collect not implemented");

}


int main(void)
{

  heap_t heap;
  initialize_heap(&heap);


  char *root =halloc(&heap, 10); 

   for(int i = 0; i < 10; i++)
    {
      root[i] = 'P';
    }
  root[9] = '\0';


  char *root_duo = halloc(&heap, 20);

   for(int i = 0; i < 20; i++)
    {
      root_duo[i] = 'J';
    }
  root_duo[19] = '\0';


printf("%s ---- %s\n", root, root_duo);

//  hfree(root);
int i = 25;

 while(i < 27)
{
halloc(&heap, 25);
    i++;
}

heap_dump_chunks(&heap);

// chunk_t chunk;
// heap.alloced_chunks.move(&heap.alloced_chunks, &chunk, root);
//
//  printf("moved: %p ---- \n", chunk.key);
//
//  printf("---------\n");
//
//heap_dump_chunks(&heap);
//
  return 0;
}
