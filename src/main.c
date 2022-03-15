#include "../include/my_malloc.h"

int main(void)
{

  my_malloc(966); // 966 + 48 = 1014
  my_malloc(966); 
  my_malloc(966);
  void* pointer = my_malloc(966);

  page_t *pages_two = get_heap_pages();

 // list_pages(pages_two);

  printf("---------------------------------------------------------------\n\n\n");
  void *pointer_one = my_malloc(840); 
  void *pointer_two = my_malloc(855);

  printf("---------------------------------------------------------------------------\n\n\n");
//  my_free(pointer_one);
 // my_free(pointer);

  page_t *pages = get_heap_pages();

  //list_pages(pages_two);
  list_pages(pages);

  dealloc();
  return 0;
}
