#include "../include/my_malloc.h"

int main(void)
{
 void *p1 = my_malloc(966); // 966 + 48 = 1014
 void *p2 = my_malloc(966); 
 void *p3 = my_malloc(966);
 void *p4 = my_malloc(966);

//  page_t *pages = get_heap_pages();

  // PAGE 02

  void *p5 = my_malloc(840); 
  void *p6 = my_malloc(855);

   my_free(p1);
   my_free(p2);
   my_free(p3);
   my_free(p4);
   my_free(p5);
   my_free(p6);
  


   print_freed();


   dealloc();

  return 0;
}


//0x7ffff7fa94be
