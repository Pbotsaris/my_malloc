#include <criterion/criterion.h>
#include "../include/my_malloc.h"


bool contains(chunk_t *pointer_array[], void *pointer)
{
  int index = 0;
  while(pointer_array[index])
  {
      if(pointer_array[index]->pointer == pointer)
        return true;
    index++;
 }
  return false;
}


Test(asserts, allocation_returns_pointer)
{
  void *pointer = my_malloc(30);

  cr_assert(pointer != NULL, "Allocation does not return a pointer\n");
  dealloc();
}


 Test(asserts, multiple_allocations)
 {
   chunk_t *array_to_dump[CHUNKS_CAPACITY + 1];
   void *pointer = my_malloc(30);
   void *pointer_two = my_malloc(10);
   void *pointer_three = my_malloc(70);
   void *pointer_four = my_malloc(50);

   dump(array_to_dump);
 
     cr_assert(contains(array_to_dump, pointer), "First pointer does not match\n");
     cr_assert(contains(array_to_dump, pointer_two), "second pointer does not match\n");
     cr_assert(contains(array_to_dump, pointer_three), "third pointer does not match\n");
     cr_assert(contains(array_to_dump, pointer_four), "fourth pointer does not match\n");
 
   dealloc();
 
 }
 
// Test(asserts, allocation_has_correct_size)
// {
// 
//   int size = 10;
//   int size_two = 20;
//   int size_three = 30;
// 
//   void *pointer = my_malloc(size);
//   void *pointer_two = my_malloc(size_two);
//   void *pointer_three = my_malloc(size_three);
// 
//   chunk_t *chunk = find_alloc(pointer);
//   chunk_t *chunk_two = find_alloc(pointer_two);
//   chunk_t *chunk_three = find_alloc(pointer_three);
// 
//   cr_assert(chunk->pair == size, "Pointer one: Allocation size is incorrect\n");
//   cr_assert(chunk_two->pair == size_two, "Pointer two: Allocation size is incorrect\n");
//   cr_assert(chunk_three->pair == size_three, "Pointer three: Allocation size is incorrect\n");
// 
//   dealloc();
// }
// 
// 
// Test(asserts, move_pointer_from_allocated_chunks)
// {
// 
//   void *pointer = my_malloc(10);
//   my_malloc(10);
//   my_malloc(10);
// 
//   chunk_t *moved_chunk = move_from_alloced_chunks(pointer);
// 
//   chunk_t *chunk = find_alloc(pointer);
// 
//   cr_assert(chunk == NULL, "Chunk in alloced_chunks still exist\n");
//   cr_assert(moved_chunk->key == pointer , "Chunk returned from move is incorrect\n");
// 
//   dealloc();
// }
 
