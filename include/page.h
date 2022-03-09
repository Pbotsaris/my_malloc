#ifndef PAGE_H
#define PAGE_H

#include "../include/hash_table.h"

#define PAGE_TINY 1                  /* 1 x OS page size (4K) */
#define PAGE_SMALL 10                /* 10 x OS page size (40K) */
#define PAGE_MEDIUM 100              /* 100 x OS page size (400K) */
#define PAGE_LARGE 1000              /* 1000 x OS page size (4M) */
#define PAGE_HUGE 10000              /* 10000 x OS page size (40M) */
#define PAGE_MASSIVE 100000          /* 100000 x OS page size (400M) */

typedef struct page {
 int             size;   /*  8 bytes */
 char           *memory;  /*  8 bytes */
 map_t          alloced_chunks;

 struct page    *next_page; /*  8 bytes */
}page_t;


#endif
