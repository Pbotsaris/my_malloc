#include <criterion/criterion.h>
#include "../include/malloc_test_api.h"


 Test(asserts, garbage_collection_removes_first_pages_from_heap)
{

  /* page 1 */
 void *p1 = malloc_test(4060); 

  /* page 2 */
  void *p2 = malloc_test(840); 
  malloc_test(855);

  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && !pages->next->next,  "Number of pages after allocation does not match.");

  free_test(p1);
  free_test(p2);

  pages = get_heap_pages();

  /* removes only first page. next page has no allocation but will be collected only after next free if it is still empty */
 cr_assert(pages && !pages->next,  "Number of pages after deallocation does not match.");

 dealloc();

}


 Test(asserts, garbage_collection_removes_end_pages_from_heap)
{

 /* page 1 */
 void *p1 = malloc_test(4060); 

  /* page 2 */
  void *p2 = malloc_test(840); 

  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && !pages->next->next,  "Number of pages after allocation does not match.");

  /* change the order to free last page */
  free_test(p2);
  free_test(p1);

  pages = get_heap_pages();


  cr_assert(pages && !pages->next,  "Number of pages after deallocation does not match.");

 dealloc();

}


 Test(asserts, garbage_collection_removes_pages_in_middle_from_heap)
{

 /* page 1 */
   malloc_test(4060); 

  /* page 2 */
  void *p2 = malloc_test(4060); 

  /* page 3 */
  void *p3 = malloc_test(840); 


  page_t *pages = get_heap_pages();

 cr_assert(pages  && pages->next && pages->next->next && !pages->next->next->next,  "Number of pages after allocation does not match.");

  /* free page 2 then the next free will trigger collector */
  free_test(p2);
  free_test(p3);

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

  void *page1_chunk1 = malloc_test(966); // 966 + 48 = 1014
  void *page1_chunk2 = malloc_test(966); 
  void *page1_chunk3 = malloc_test(966);
  void *page1_chunk4 = malloc_test(966);

  malloc_test(840); 
  void *page2_chunk2 = malloc_test(840); 

  free_test(page1_chunk1);
  free_test(page1_chunk2);
  free_test(page1_chunk3);
  free_test(page1_chunk4);

  chunk_t *chunk1 = find_freed_pointer(page1_chunk1);
  chunk_t *chunk2 = find_freed_pointer(page1_chunk2);

  cr_assert(chunk1 && chunk2 ,  "Chunks not in the bin after free");

  free_test(page2_chunk2);

  chunk1 = find_freed_pointer(page1_chunk1);
  chunk2 = find_freed_pointer(page1_chunk2);
  chunk_t *page2_chunk = find_freed_pointer(page2_chunk2);

  cr_assert(page2_chunk && !chunk2 && !chunk1 ,  "chunks were not removed from bin after collection");
}





