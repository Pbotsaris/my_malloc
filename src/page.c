#include "../include/page.h"


static size_t check_size(size_t size);

void init_page(page_t *page, size_t size)
{
  size_t page_size = check_size(size);

 if(page_size > 0 )
     page->capacity            = (int) sysconf(_SC_PAGESIZE) * page_size;
  else
     page->capacity            = size;
    
  page->buffer                 = (void*) mmap(0, page->capacity , PROT_READ | PROT_WRITE,
                                              MAP_ANON | MAP_PRIVATE, -1, 0);
  page->size                   = 0;
  page->next                   = NULL;
}



static size_t check_size(size_t size)
{

  if(size <= SMALL_ALLOCATION )
    return PAGE_SMALL;
  else if(size <= MEDIUM_ALLOCATION)
     return PAGE_MEDIUM;
  else if(size <= LARGE_ALLOCATION)
     return PAGE_LARGE;
  else
     return -1;
}
