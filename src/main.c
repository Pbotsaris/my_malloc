#include "../lib/my_malloc.h"

#define S 15

int sizes[S] = {10, 11, 12, 13, 20, 21, 22, 100, 500, 1000, 5000, 15000, 10000, 100000, 0};
int realloc_sizes[S] = {200, 400 , 505, 2000, 2400, 4034, 5069, 20000, 40000, 20390, 16, 10, 2039403, 32, 0};
void *pointers[S];

int main(void)
{
  int index = 0;

  while(sizes[index] > 0)
  {
    pointers[index] = my_malloc(sizes[index]) ;   
    index++;
  }

  /* vizualize the heap */

  printf("====== heap before free ======\n");
  print_alloced_chunks();
  print_freed();
  printf("+++++++++++++++++++++++++++++\n");

  /* free half of the pointer */

  index = 0;
  while(pointers[index])
  {
      if(index % 2 == 0)
        my_free(pointers[index]);

      index++;
  }

  printf("====== heap after free =====\n");
  print_alloced_chunks();
  print_freed();
  printf("+++++++++++++++++++++++++++++\n");


  /* you can also visualize the pages */

  printf("====== allocated pages =====\n");
  print_heap_pages();


  /* realloc the other half */

  index = 0;
  while(pointers[index])
  {

  /* can only reallocate unfreed pointers */

      if(index % 2 != 0)
        my_realloc(pointers[index], realloc_sizes[index]);

      index++;

  }

  printf("====== heap after realloc =====\n");
  print_alloced_chunks();
  print_freed();
  printf("+++++++++++++++++++++++++++++\n");


  return 0;
}

