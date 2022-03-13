#include <criterion/criterion.h>
#include "../include/my_malloc.h"

/* TODO: TEST WILL MULTIPLE ALLOCATION RANGED SIZE IS CRASH IN PAGES 
*
*   stack trace
    #0 0x5616808aa044 in initialize_page src/page.c:73
    #1 0x5616808a9d5c in create_page src/page.c:16
    #2 0x5616808a975c in alloc src/heap.c:38
    #3 0x5616808a9ad7 in my_malloc src/my_malloc.c:15
*
*
*
* */

 size_t fixed_sizes[9] = {4, 8, 16, 32, 64, 128, 256, 512, 0 };

 size_t ranged_sizes[31] = {
                           1014, 1018, 1024,              // 8
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

 u_int8_t expected[30]   = {
                            8,  8,  8,        
                            9,  9,  9,         
                            10, 10, 10,       
                            11, 11, 11,       
                            12, 12, 12,       
                            13, 13, 13,       
                            14, 14, 14,       
                            15, 15, 15,       
                            16, 16, 16,       
                            17, 17, 17,       
                             };

Test(asserts, get_correct_bin_index_fixed_sizes)
{

  // need to call malloc to initialize the heap.
  my_malloc(10);


  int i = 0;
  while(fixed_sizes[i] != 0)
   {

  u_int8_t result = get_bin_index(fixed_sizes[i]);

  cr_assert(result == i, "size %lu does not return correct index. result -> %i | should be: %i", fixed_sizes[i], result, i);
      i++;
   }

  dealloc();
}


Test(asserts, get_correct_bin_index_ranged_sizes)
{

  my_malloc(10);

  int i = 0;
  while(ranged_sizes[i] != 0)
   {

  u_int8_t result = get_bin_index(ranged_sizes[i]);

  cr_assert(result == expected[i], "size %lu does not return correct index. result -> %i | should be -> %i", ranged_sizes[i], result, expected[i]);
      i++;
   }

  dealloc();
}


Test(asserts, add_fixed_sized_chunks_to_bin)
{

  int i = 0;
  while(fixed_sizes[i] != 0)
  {

   void *pointer  = my_malloc(fixed_sizes[i]);

   chunk_t *chunk = move_from_alloced_chunks(pointer);
   add_to_bin(chunk);
   chunk_t *ret = find_freed(chunk);

  cr_assert(ret == chunk, "Freed chunk does not match. result -> %p | should be: %p", (void*)ret, (void*)chunk);

   i++;
  }


  dealloc();
}

Test(asserts, add_ranged_sized_chunks_to_bin)
{

  int i = 0;
  while(ranged_sizes[i] != 0)
  {

   void *pointer  =  my_malloc(ranged_sizes[i]);

   chunk_t *chunk = move_from_alloced_chunks(pointer);
   add_to_bin(chunk);
   chunk_t *ret = find_freed(chunk);

  cr_assert(ret == chunk, "Freed chunk does not match. result -> %p | should be: %p", (void*)ret, (void*)chunk);

   i++;
  }


  dealloc();
}





