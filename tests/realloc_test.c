#include <criterion/criterion.h>
#include "../include/my_malloc.h"

 Test(asserts, realloc_before_malloc_fails)
{
  int i = 10;
  void *pointer = &i; 

  pointer = my_realloc(pointer, 10);

  cr_assert(!pointer, "Realloc did not return NULL when heap was not initialized");
}

 Test(asserts, realloc_with_null_pointer_fails)
{
  void *pointer = my_realloc(NULL, 10);

  cr_assert(!pointer, "Realloc did not return NULL when heap was not initialized");
}

 Test(asserts, realloc_same_size_returns_same_pointer)
{
  void *pointer = my_malloc(100);
  void *pointer_two = my_realloc(pointer, 100);

  cr_assert(pointer == pointer_two, "realloc pointer is not the same. returned -> %p, should be -> %p", pointer_two, pointer);

  dealloc();
}

 Test(asserts, realloc_creates_new_allocation)
{
  void *pointer = my_malloc(100);
  void *pointer_two = my_realloc(pointer, 256);

  cr_assert(pointer != pointer_two, "realloc pointer does not return a new pointer address");

   chunk_t *found = find_alloc(pointer_two);
   chunk_t *free_found = find_freed_pointer(pointer);

  cr_assert(found && found->pointer == pointer_two, "realloc pointer was not found in heap->alloced_chunks. returned -> %p, should be: %p", found, (void*)pointer_two);
  cr_assert(free_found && free_found->pointer == pointer, "freed pointer was not found in the heap->bin");

  dealloc();
}

 Test(asserts, calloc_returns_a_valid_pointer)
{
  void *pointer = my_calloc(sizeof(int), 10);

  cr_assert(pointer, "calloc did not return a valid pointer");
}
