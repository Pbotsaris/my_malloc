#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define CHUNKS_CAPACITY 1024
#include "../include/page.h"

typedef struct chunk {
  void              *pointer;          /*  8 bytes  */
  u_int32_t         size;              /*  4 bytes  */

  struct chunk      *next;             /*  8 bytes  */
  struct chunk      *prev;             /*  8 bytes  */ /* used only in bin  */
  struct chunk      *next_in_page;     /*  8 bytes  */ /* connects secondary chunk list on every page for garbage collection  */
  page_t            *page;             /*  8 bytes  */ 
}chunk_t;
                                       /*  -------- */
                               /* TOTAL:   48 bytes */

void initialize_chunk(chunk_t *chunk, void *pointer, size_t size);
void add_chunk_to_page(chunk_t *chunk, page_t *page);
void remove_page_chunks_from_bin(page_t *page);


#endif
