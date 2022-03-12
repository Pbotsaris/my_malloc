#include "../include/page.h"

/* PRIVATE HELPERS */
static page_t *initialize_page(void *buffer, size_t size);
static void *alloc_page_buffer(void *next_allocation, size_t alloc_size, size_t *return_size, size_t os_page_size);
static size_t check_size(size_t size, size_t os_page_size);


/* PUBLIC  */

page_t *create_page(void *next_allocation, size_t alloc_size, size_t os_page_size)
{
  size_t return_size;
  /* create buffer for the first page */
  void *buffer = alloc_page_buffer(next_allocation, alloc_size, &return_size, os_page_size);
  return initialize_page(buffer, return_size);
}


page_t *find_page(page_t *page, size_t size)
{
  page_t *temp = page;


  while(temp)
  {
     if(size <= temp->capacity)
        break;

    temp = temp->next;
  }

  return temp;
}

page_t *add_page(page_t *root_page, page_t *page)
{
  page_t *temp = root_page;

  while(temp)
  {
      if(!temp->next)
      {
        temp->next = page;
        break;
      }

      temp = temp->next;
  }

  return root_page;
}

void list_pages(page_t *root_page)
{
  page_t *temp = root_page;

  while(temp)
  {
      printf("page addr: %p, page->capacity: %d, page->size: %d\n", (void*)temp, temp->capacity, temp->size);
      temp = temp->next;
  }

}


/* PUBLIC  */

static page_t *initialize_page(void *buffer, size_t size)
{

  page_t *page                 = (page_t*)buffer;
  page->capacity               = size; 
  page->buffer                 = (void*)((int32_t*)buffer + sizeof(page_t));
  /* adding page metadate to the top of the buffer */
  page->size                   = sizeof(page_t);
  page->next                   = NULL;

  return page;
}

static void *alloc_page_buffer(void *next_allocation, size_t alloc_size, size_t *return_size, size_t os_page_size)
{

  
  size_t page_multiplier     = check_size(alloc_size, os_page_size);

  /* for huge allocations the page will grow to fit the exact size of the allocation */
 if(page_multiplier > 0 )
    alloc_size                     = (u_int32_t) os_page_size * page_multiplier;


  void *buffer               = (void*) mmap(next_allocation, alloc_size , PROT_READ | PROT_WRITE,
                                           MAP_ANON | MAP_PRIVATE, -1, 0);

  /* to return the actual size of the alloc'ed buffer */
  *return_size               = alloc_size;

  return buffer;
}


static size_t check_size(size_t size, size_t os_page_size)
{

  if(size <=  os_page_size * PAGE_SMALL)
    return PAGE_SMALL;
  else if(size <= os_page_size * PAGE_MEDIUM)
     return PAGE_MEDIUM;
  else if(size <= os_page_size * PAGE_LARGE)
     return PAGE_LARGE;
  else
     return -1;
}




