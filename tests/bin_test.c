#include <criterion/criterion.h>
#include "../include/malloc_test_api.h"

 size_t fixed_sizes[9] = {4, 8, 16, 32, 64, 128, 256, 512, 0 };

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

  // need to call malloc to initialize malloc_test
  malloc_test(10);


  int i = 0;
  while(fixed_sizes[i] != 0)
   {

  u_int8_t result = get_bin_index(fixed_sizes[i]);

  cr_assert(result == i, "size %lu does not return correct index. result -> %i | should be: %i", fixed_sizes[i], result, i);
      i++;
   }

  dealloc();
}

Test(asserts, bin_doubly_linked_list_is_sorted)
{

 size_t size = 1026;
 chunk_t *head;
 chunk_t *prev = NULL;

  while(size < 1035)
  {
   void *pointer  = malloc_test(size);
   chunk_t *chunk = move_from_alloced_chunks(pointer);
    
   add_to_bin(chunk);

    if(size == 1026)
      head = find_freed_chunk(chunk);

  size++;
  }

  while(head)
  {
      if(prev)
         cr_assert(prev->size < head->size, "List is not sorted correctly.  %d  <-- prev | current --> %d", prev->size, head->size);
      
      prev = head;
      head = head->next;
  }
}

Test(asserts, get_correct_bin_index_ranged_sizes)
{

  malloc_test(10);

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

   void *pointer  = malloc_test(fixed_sizes[i]);

   chunk_t *chunk = move_from_alloced_chunks(pointer);
   add_to_bin(chunk);
   chunk_t *ret = find_freed_chunk(chunk);

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

   void *pointer  =  malloc_test(ranged_sizes[i]);

  chunk_t *chunk = move_from_alloced_chunks(pointer);

   //       printf("\nfointer -> %d\n", chunk->size);
 
        add_to_bin(chunk);
        chunk_t *ret = find_freed_chunk(chunk);

    cr_assert(ret == chunk, "Freed chunk does not match. result -> %p | should be: %p", (void*)ret, (void*)chunk);

   i++;
  }


  dealloc();
}

Test(asserts, bin_doubly_linked_list_is_correct)
{
  
int i = 0;
chunk_t *head;
  while(i < 5)
  {
   void *pointer  = malloc_test(8);

   chunk_t *chunk = move_from_alloced_chunks(pointer);

    
   add_to_bin(chunk);



    if(i == 0)
      head = find_freed_chunk(chunk);

   i++;
  }

  int j = 0;
  chunk_t *prev;

  while(head)
  {
    if(j == 0)
       cr_assert(head->prev == NULL, "Head->prev should be NULL, was -> %p", (void*)head->prev);
    else
       cr_assert(head->prev == prev, "%p prev should be -> %p and was -> %p",(void*)head, (void*)prev, (void*)head->prev);

    prev = head;
    head = head->next;
   j++;
  }
}



