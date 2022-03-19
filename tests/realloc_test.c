#include <criterion/criterion.h>
#include "../include/malloc_test_api.h"

 Test(asserts, realloc_before_malloc_fails)
{
  int i = 10;
  void *pointer = &i; 

  pointer = realloc_test(pointer, 10);

  cr_assert(!pointer, "Realloc did not return NULL when heap was not initialized");
}

 Test(asserts, realloc_with_null_pointer_fails)
{
  void *pointer = realloc_test(NULL, 10);

  cr_assert(!pointer, "Realloc did not return NULL when heap was not initialized");
}

 Test(asserts, realloc_same_size_returns_same_pointer)
{
  void *pointer = malloc_test(100);
  void *pointer_two = realloc_test(pointer, 100);

  cr_assert(pointer == pointer_two, "realloc pointer is not the same. returned -> %p, should be -> %p", pointer_two, pointer);

  dealloc();
}

 Test(asserts, realloc_creates_new_allocation)
{
  void *pointer = malloc_test(100);
  void *pointer_two = realloc_test(pointer, 256);

  cr_assert(pointer != pointer_two, "realloc pointer does not return a new pointer address");

   chunk_t *found = find_alloc(pointer_two);
   chunk_t *free_found = find_freed_pointer(pointer);

  cr_assert(found && found->pointer == pointer_two, "realloc pointer was not found in heap->alloced_chunks. returned -> %p, should be: %p", found, pointer_two);
  cr_assert(free_found && free_found->pointer == pointer, "freed pointer was not found in the heap->bin");

  dealloc();
}

 Test(asserts, calloc_returns_a_valid_pointer)
{
  void *pointer = calloc_test(sizeof(int), 10);

  cr_assert(pointer, "calloc did not return a valid pointer");
}
