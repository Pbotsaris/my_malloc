#include <criterion/criterion.h>
#include "../include/my_malloc.h"


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

// TODO: write a test for garbage collection. You need to add two pages where chunks fall in the same
//  bin size class. then trigger garbage collection and check if bin structure is intact.
//  also need to check if page was removed for the heap->pages
 Test(asserts, allocate_pages_correctly)
{
  /* PAGE 01
   * allocate 1 page worth of chunks
   * 4096 (page size) - 40 (page header) = 4060
   * 4056 / 4 (four allocations) = 1014
   */

 void *p1 = my_malloc(966); // 966 + 48 = 1014
 void *p2 = my_malloc(966); 
 void *p3 = my_malloc(966);
 void *p4 = my_malloc(966);

//  page_t *pages = get_heap_pages();

  // PAGE 02

  void *p5 = my_malloc(840); 
  my_malloc(855);

  my_free(p1);
  my_free(p2);
  my_free(p3);
  my_free(p4);
  my_free(p5);


   print_freed();


 // void *pointer_one = my_malloc(840); 
 // void *pointer_two = my_malloc(855);


   dealloc();

}
