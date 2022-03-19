#include <criterion/criterion.h>
#include "../include/malloc_test_api.h"

/* sizes needs to take most of the mage so it doesn't try to allocate from remaining page memory */
  size_t sizes[] = {2000, 20000, 200000, 500000, 0 }
// size_t sizes[] = {4, 8, 16, 32, 64, 128, 256, 512, 566, 1230, 2300, 0};
;

Test(asserts, free_uses_bin_when_chunk_is_avail)
{
  int i = 0;

  while(sizes[i] != 0)

    {
      void *pointer = malloc_test(sizes[i]);

      chunk_t *alloced_chunk = find_alloc(pointer);
      chunk_t *freed_chunk = find_freed_pointer(pointer);

      cr_assert(alloced_chunk && alloced_chunk->pointer , "chunk does not exist in heap->alloced_chunks after allocation");
      cr_assert(!freed_chunk , "chunk exists in the bin before calling free");

      free_test(pointer);

      freed_chunk = find_freed_pointer(pointer);
      alloced_chunk = find_alloc(pointer);

      cr_assert(!alloced_chunk, "chunk exists in heap->alloced_chunks after free");
      cr_assert(freed_chunk && freed_chunk->pointer == pointer , "free does not use chunks from the bin");

      pointer = malloc_test(sizes[i]);
      freed_chunk = find_freed_pointer(pointer);
      alloced_chunk = find_alloc(pointer);

      cr_assert(alloced_chunk && alloced_chunk->pointer , "reallocation using bin does not work");

      cr_assert(!freed_chunk , "using bin chunks for reallocation does not remove chunk from the bin");

      i++;
    }

}
