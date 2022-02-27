
#include <stdlib.h>
#include <stdio.h>

int main(void)
{


   void *p = malloc(10);

   char str[16] = {0};

   sprintf(str, "%p", p);


  printf("%s", str);

  free(p);

  return 0;



}
