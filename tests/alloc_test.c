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
 
 Test(asserts, allocation_has_correct_size)
 {
 
   u_int32_t size = 10;
   u_int32_t size_two = 20;
   u_int32_t size_three = 30;
 
   void *pointer = my_malloc(size);
   void *pointer_two = my_malloc(size_two);
   void *pointer_three = my_malloc(size_three);
 
   chunk_t *chunk = find_alloc(pointer);
   chunk_t *chunk_two = find_alloc(pointer_two);
   chunk_t *chunk_three = find_alloc(pointer_three);
 
   cr_assert(chunk->size == size, "Pointer one: Allocation size is incorrect\n");
   cr_assert(chunk_two->size == size_two, "Pointer two: Allocation size is incorrect\n");
   cr_assert(chunk_three->size == size_three, "Pointer three: Allocation size is incorrect\n");
 
   dealloc();
 }
 
 Test(asserts, allocation_accross_multiple_sized_pages)
 {
   /* they should live in different pages */
   size_t size_small = 40, size_medium = 5000, size_large = 50000;
 
   void *small = my_malloc(size_small);
   void *medium = my_malloc(size_medium);
   void *large = my_malloc(size_large);
 
   size_t os_page_size = get_heap_os_page_size();
   page_t *pages = get_heap_pages();
 
 //  list_pages(pages);
 
   void *small_page_buffer = NULL , *medium_page_buffer = NULL, *large_page_buffer = NULL;
 
   while(pages)
     {
     // one allocation per page we gotta account for both page and chunk metadata;
     if(pages->capacity == PAGE_SMALL * os_page_size)
        small_page_buffer = (int8_t*)pages->buffer + (sizeof(page_t) + sizeof(chunk_t));
 
     if(pages->capacity == PAGE_MEDIUM * os_page_size)
         medium_page_buffer = (int8_t*)pages->buffer + (sizeof(page_t) + sizeof(chunk_t));
 
     if(pages->capacity == PAGE_LARGE * os_page_size)
         large_page_buffer = (int8_t*)pages->buffer + (sizeof(page_t) + sizeof(chunk_t));
 
     pages = pages->next;
     }
 
   cr_assert(small_page_buffer != NULL, "Did not allocate a small page");
   cr_assert(medium_page_buffer != NULL, "Did not allocate a medium page");
   cr_assert(large_page_buffer != NULL, "Did not allocate a large page");
 
   cr_assert(small_page_buffer == small, "Pointer does not exist in respective SMALL page");
   cr_assert(medium_page_buffer == medium, "Pointer does not exist in respective MEDIUM page");
   cr_assert(large_page_buffer == large, "Pointer does not exist in respective LARGE page");
 
   dealloc();
 
 }
 
  
  
  Test(asserts, move_pointer_from_allocated_chunks)
  {
    void *pointer = my_malloc(10);
    my_malloc(10);
    my_malloc(10);
  
    chunk_t *moved_chunk = move_from_alloced_chunks(pointer);
  
    chunk_t *chunk = find_alloc(pointer);
  
    cr_assert(chunk == NULL, "Chunk in alloced_chunks still exist\n");
    cr_assert(moved_chunk->pointer == pointer , "Chunk returned from move is incorrect\n");
  
    dealloc();
  }
 
