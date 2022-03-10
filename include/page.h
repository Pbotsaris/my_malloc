#ifndef PAGE_H
#define PAGE_H


#include <sys/mman.h>
#include "../include/hash_table.h"

#define PAGE_SMALL 1                 /* 10 x OS page size (4K) */
#define PAGE_MEDIUM 10              /* 100 x OS page size (40K) */
#define PAGE_LARGE 100              /* 1000 x OS page size (400K) */

#define SMALL_ALLOCATION 4000
#define MEDIUM_ALLOCATION 40000
#define HUGE_ALLOCATION 400000

/*  Allocations larger than 400K takes a page the size of the allocation */

typedef struct page {
 int             size;   /*  8 bytes */
 int             capacity;
 void           *buffer;  /*  8 bytes */

 struct page    *next_page; /*  8 bytes */
}page_t;


#endif
