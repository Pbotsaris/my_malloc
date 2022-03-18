#include "../include/my_malloc.h"

 size_t ranged_sizes[31] = {
                           514, 1018, 1024,              // 8
                           1025, 2000, 2048,              // 9
                           2049, 3000, 4096,              // 10
                           4097, 6000, 8192,              // 11
                           8193, 12000, 16384,            // 12
                           16385, 25000, 32768,           // 13
                           32769, 50000, 65536,           // 14
                           65537, 100000, 131072,         // 15
                           131073, 200000, 262144,        // 16
                           262145, 500000, 409600,        // 17
                           0
                           };


int main(void)
{

  int i = 0;
  while(ranged_sizes[i] != 0)
  {

   void *pointer  =  my_malloc(ranged_sizes[i]);

  chunk_t *chunk = move_from_alloced_chunks(pointer);

   //       printf("\nfointer -> %d\n", chunk->size);
 
        add_to_bin(chunk);
        chunk_t *ret = find_freed_chunk(chunk);

   i++;
  }


   dealloc();

  return 0;
}


//0x7ffff7fa94be
