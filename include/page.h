#ifndef PAGE_H
#define PAGE_H


#include <sys/mman.h>
#include "../include/hash_table.h"

#define PAGE_SMALL 1                 /* 10 x OS page size (4K) */
#define PAGE_MEDIUM 10              /* 100 x OS page size (40K) */
#define PAGE_LARGE 100              /* 1000 x OS page size (400K) */

/*  Allocations larger than 400K takes a page the size of the allocation */

typedef struct page {
 u_int32_t      size;           /*  4 bytes */
 u_int32_t      capacity;       /*  4 bytes */
 void           *buffer;        /*  8 bytes */

 struct page    *next;      /*  8 bytes */
}page_t;


page_t *create_page(void *next_allocation, size_t alloc_size, size_t os_page_size);
page_t *find_page(page_t *page, size_t size);
page_t *add_page(page_t *root_page, page_t *page);
void list_pages(page_t *root_page);

#endif
