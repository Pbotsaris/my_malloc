#ifndef BIN_H
#define BIN_H

#include <math.h>
#include "../include/chunk_header.h"

/* 
*   MAX SIZE with linux page @ 4096 is 409600 bytes
*   The sizes in the table has quadratic distribution so to calculate the number of items in the table we can:
*   (LOG2 (MAX_SIZE) - 1) -> (LOG2 (409600) - 1) ~= 18 ->  ceil(18.644 - 1)
*    we are going to start sizes at 4 byts or  2 ^ 2.
*/

/*
*
*  EXACT SIZES: allocation below 512 will have fixed sizes depending on the class. This will cause some fragmentation
*   but querying for free chunks will be fast.
*   E.g: if user requests for a  17 bytes chunk she will receive a 32 bytes instead,
*
*            2^2  2^3  2^4   2^5   2^6  2^7   2^8   2^9    
*  size:      4    8   16    32    64   128   256   512 
*  index:     0    1    2    3     4     5     6     7
*
*  RANGED SIZES: For larger allocations each class will store a range of sizes. The algorithm will search the list for the first fit
*              formular for the range is  -> 2^(X-1)-1 <-> 2^X 
*
*                    2^10            2^11            2^12            2^13            2^14              2^15              
*  range:     | 513 <-> 1024 | 1025 <-> 2048 | 2049 <-> 4096 | 4097 <-> 8192 | 8193 <-> 16384 | 16385 <-> 32768 
*  index:             8               9               10              11              12                13
*
*                     2^16              2^17                2^18                     MAX 
*  range:     | 32769 <-> 65536 | 65537 <-> 131072 | 131073 <-> 262144 | 262145 <-> 409600
*  index:              14                15                  16                  17
*
*
*      FORMULA TO FIND INDEX: 
*      for x = size
*
*      ceil(log2 (X)) - OFFSET
*
*      BIN_SIZE  = 16;
*      OFFSSET   = 2;
*
*/

#define BIN_SIZE 18
#define OFFSET 2

typedef struct bin
{
  chunk_t         *table[BIN_SIZE];

  u_int8_t       (*get_index)(size_t);
  chunk_t        *(*add)(struct bin*, chunk_t*);
  chunk_t        *(*find)(struct bin*, chunk_t*);
  }bin_t;


void initialize_bin(bin_t *bin);

#endif
