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

 Test(asserts, garbage_collection_removes_first_pages_from_heap)
{

  /* page 1 */
 void *p1 = my_malloc(4060); 

  /* page 2 */
  void *p2 = my_malloc(840); 
  my_malloc(855);

  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && !pages->next->next,  "Number of pages after allocation does not match.");

  my_free(p1);
  my_free(p2);

  pages = get_heap_pages();

  /* removes only first page. next page has no allocation but will be collected only after next free if it is still empty */
 cr_assert(pages && !pages->next,  "Number of pages after deallocation does not match.");

 dealloc();

}


 Test(asserts, garbage_collection_removes_end_pages_from_heap)
{

 /* page 1 */
 void *p1 = my_malloc(4060); 

  /* page 2 */
  void *p2 = my_malloc(840); 

  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && !pages->next->next,  "Number of pages after allocation does not match.");

  /* change the order to free last page */
  my_free(p2);
  my_free(p1);

  pages = get_heap_pages();


  cr_assert(pages && !pages->next,  "Number of pages after deallocation does not match.");

 dealloc();

}


 Test(asserts, garbage_collection_removes_pages_in_middle_from_heap)
{

 /* page 1 */
   my_malloc(4060); 

  /* page 2 */
  void *p2 = my_malloc(4060); 

  /* page 3 */
  void *p3 = my_malloc(840); 


  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && pages->next->next && !pages->next->next->next,  "Number of pages after allocation does not match.");

  /* free page 2 then the next free will trigger collector */
  my_free(p2);
  my_free(p3);

  pages = get_heap_pages();

 cr_assert(pages && pages->next && !pages->next->next,  "Number of pages after deallocation does not match.");

 dealloc();

}

 Test(asserts, garbage_collection_removes_chunks_from_bin)
{

  /* PAGE 01
   * allocate 1 page worth of chunks
   * 4096 (page size) - 40 (page header) = 4060
   * 4056 / 4 (four allocations) = 1014
   */

  void *page1_chunk1 = my_malloc(966); // 966 + 48 = 1014
  void *page1_chunk2 = my_malloc(966); 
  void *page1_chunk3 = my_malloc(966);
  void *page1_chunk4 = my_malloc(966);

  my_malloc(840); 
  void *page2_chunk2 = my_malloc(840); 

  my_free(page1_chunk1);
  my_free(page1_chunk2);
  my_free(page1_chunk3);
  my_free(page1_chunk4);

  chunk_t *chunk1 = find_freed_pointer(page1_chunk1);
  chunk_t *chunk2 = find_freed_pointer(page1_chunk2);

  cr_assert(chunk1 && chunk2 ,  "Chunks not in the bin after free");

  my_free(page2_chunk2);

  chunk1 = find_freed_pointer(page1_chunk1);
  chunk2 = find_freed_pointer(page1_chunk2);
  chunk_t *page2_chunk = find_freed_pointer(page2_chunk2);

  cr_assert(page2_chunk && !chunk2 && !chunk1 ,  "chunks were not removed from bin after collection");
}





