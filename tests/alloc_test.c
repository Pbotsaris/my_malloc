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


 Test(asserts, allocation_with_zero_size)
 {
   void *pointer = my_malloc(0);
 
   cr_assert(pointer == NULL, "allocation with invalid size does not return NULL. Was %p, should be NULL", pointer);

 }

 Test(asserts, allocation_with_invalid_size)
 {
   void *pointer = my_malloc(-1);
 
   cr_assert(pointer == NULL, "allocation with invalid size does not return NULL. Was %p, should be NULL", pointer);

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
 
   dump_alloced_chunks(array_to_dump);
 
   cr_assert(contains(array_to_dump, pointer), "First pointer does not match\n");
   cr_assert(contains(array_to_dump, pointer_two), "second pointer does not match\n");
   cr_assert(contains(array_to_dump, pointer_three), "third pointer does not match\n");
   cr_assert(contains(array_to_dump, pointer_four), "fourth pointer does not match\n");
 
   dealloc();
 
 }
 
 Test(asserts, allocation_has_correct_size)
 {
 
   void *pointer = my_malloc(3); // fixed size will become 4
   void *pointer_two = my_malloc(20); // fixed size will become 32
   void *pointer_three = my_malloc(140); // fixed size will become 256
   void *pointer_four = my_malloc(899); // ranged size so remain same size value
 
   chunk_t *chunk = find_alloc(pointer);
   chunk_t *chunk_two = find_alloc(pointer_two);
   chunk_t *chunk_three = find_alloc(pointer_three);
   chunk_t *chunk_four = find_alloc(pointer_four);
 
   cr_assert(chunk->size == 4, "Pointer one: Allocation size is incorrect\n");
   cr_assert(chunk_two->size ==  32, "Pointer two: Allocation size is incorrect\n");
   cr_assert(chunk_three->size == 256, "Pointer three: Allocation size is incorrect\n");
   cr_assert(chunk_four->size == 899, "Pointer three: Allocation size is incorrect\n");
 
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

 Test(asserts, allocate_new_pages_when_page_is_full)
{
  /* PAGE 01
   * allocate 1 page worth of chunks
   * 4096 (page size) - 40 (page header) = 4060
   * 4056 / 4 (four allocations) = 1014
   */
  my_malloc(966); // 966 + 48 = 1014
  my_malloc(966); 
  my_malloc(966);
  my_malloc(966);

  page_t *pages = get_heap_pages();

  cr_assert(pages && !pages->next, "collect test did not allocate right number of pages");

  // PAGE 02

  my_malloc(840); 
  my_malloc(855);

  cr_assert(pages && pages->next && !pages->next->next , "collect test did not allocate right number of pages");

  dealloc();

}


  Test(asserts, pages_holds_a_reference_to_its_allocated_chunk)
{
  void *pointer = my_malloc(8);
  void *pointer_two = my_malloc(8);
  void *pointer_three = my_malloc(8);

  page_t *pages = get_heap_pages();
  chunk_t *page_chunks = pages->chunks;

  cr_assert(pointer == page_chunks->pointer, "pointer does not match chunk pointer. Was -> %p should be -> %p", page_chunks->pointer, pointer);
  page_chunks = page_chunks->next_in_page;
  cr_assert(pointer_two == page_chunks->pointer, "pointer does not match chunk pointer. Was -> %p should be -> %p", page_chunks->pointer, pointer_two);
  page_chunks = page_chunks->next_in_page;
  cr_assert(pointer_three == page_chunks->pointer, "pointer does not match chunk pointer. Was -> %p should be -> %p", page_chunks->pointer, pointer_three);

    dealloc();

}

 Test(asserts, page_allocation_count_increments_and_decrements)
 {
   void *pointer = my_malloc(8);
   page_t *pages = get_heap_pages(); 

   cr_assert(pages->alloced_count == 1, "alloced count does not match. Was -> %d  | should be -> 1", pages->alloced_count);

    my_malloc(8);

   cr_assert(pages->alloced_count == 2, "alloced count does not match. Was -> %d  | should be -> 2", pages->alloced_count);

    my_free(pointer);

   cr_assert(pages->alloced_count == 1, "alloced count does not match. Was -> %d  | should be -> 1", pages->alloced_count);

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
 
