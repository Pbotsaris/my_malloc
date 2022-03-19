#include "../include/malloc_test_api.h"

int main(void)
{

   void *pointer = malloc_test(4);

   chunk_t *alloced_chunk = find_alloc(pointer);
   chunk_t *freed_chunk = find_freed_pointer(pointer);


  freed_chunk = find_freed_pointer(pointer);
  alloced_chunk = find_alloc(pointer);


   pointer = malloc_test(4);
   freed_chunk = find_freed_pointer(pointer);
   alloced_chunk = find_alloc(pointer);


  return 0;
}

