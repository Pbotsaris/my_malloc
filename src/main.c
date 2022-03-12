#include "../include/my_malloc.h"

int main(void)
{
//char *pointer = (char*)my_malloc(20);

my_malloc(30);
my_malloc(5000);
//my_malloc(50);
//my_malloc(60);
//my_malloc(10);

  printf("%lu\n", sizeof(page_t));

//  void *array_to_dump[CHUNKS_CAPACITY + 1];
//  dump(array_to_dump);
//  int i = 0;
//
//  while(array_to_dump[i])
//    {
//        printf("%p\n", array_to_dump[i]);
//      i++;
//    }
//

//heap.dump(&heap);

//dealloc();
//chunk_t *chunk = heap.alloced_chunks.move(&heap.alloced_chunks, pointer);


//printf("removed pointer: %p   | size: %lu\n", chunk->key, chunk->pair);

  return 0;
}
