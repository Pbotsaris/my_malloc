#include "../include/bin.h"

static u_int8_t get_index(size_t size);


void initialize_bin(bin_t *bin)
{
   bin->get_index = get_index;
}

static u_int8_t get_index(size_t size)
{
 u_int8_t result = ((u_int8_t)ceil(log((double)size) / log(2))) - OFFSET;

  /* larger always on last index */
  if(result >= BIN_SIZE - 1)
     result = BIN_SIZE - 1;

  return result;
}

