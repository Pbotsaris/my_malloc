# my_malloc
`my_malloc` is a simple custom allocator. While it mostly prioritize speed over memory, it also implements a simple garbage collector that returns memory to the kernel whenever an empty page remains
unused for more than one malloc/free lifecycle.

## Usage

Compile 

    make

In main function there is some basic usage of `my_malloc`, `my_calloc`, `my_free` and `my_realloc`. There are other available functions such as
`print_freed` and `print_alloced_chunks` that let you visualize the heap.

To run the code in `main.c`
 
    make run

Clean

    make clean

## Terminology

 * **heap**:  The "virtual" heap of `my_malloc`. 
 * **pages**: A list of pages of allocated memory.
 * **chunk**: A chunk of user requested memory. 
 * **bin**: the data structure for freed memory chunks.
 * **alloced_chunks**: a hash table of allocated memory chunks.

## Implementation

### Pages

`my_malloc` organizes allocations in pages of memory chunks. Pages can have 3 fixed sizes `PAGE_SMALL = 1 * PAGESIZE`, `PAGE_MEDIUM = 10 * PAGESIZE` and `PAGE_LARGE = 100 * PAGESIZE`. 

You can check your page size on your machine with `getconf PAGESIZE`(linux). If the requested allocation is greater than `PAGE_LARGE`, a page with the exact size of the allocation will be requested.


                      page 01                     page 02                      page 03
             |----------------------|     |----------------------|     |----------------------|
             |   |-----|  |-----|   |     |   |-----|  |-----|   |     | -------------------  |
             |   |chunk|  |chunk|   |     |   |chunk|  |chunk|   |     | |                  | |
             |   |-----|  |-----|   |     |   |-----|  |-----|   |     | |                  | |
             |                      | --> |                      | --> | |    huge chunk    | |    
             |   |-----|  |-----|   |     |   |-----|  |-----|   |     | |                  | |
             |   |chunk|  |chunk|   |     |   |chunk|  |chunk|   |     | |                  | |
             |   |-----|  |-----|   |     |   |-----|  |-----|   |     | |------------------| |
             |----------------------|     |----------------------|     |----------------------|


When a new page is allocated, `my_malloc` stores metadata about the page on the top of the buffer. The rest of the memory is dedicated to user requested allocation.
Below an example of a page with two allocations.


                         metadata     chunk         chunk           user memory
                       --------------------------------------------------------------------
          PAGE_SMALL   | 40 bytes |  16 bytes  | 32 bytes  |          ~4.8K               |   
                       --------------------------------------------------------------------
                                                           ^                              ^
                                             page.size =  88           page.capacity = 4960

### Chunks
Chunks are created upon a memory request from the user.  Every chunk contain its own metadata such as the fields `size` and `pointer`. The allocated chunks live in the `heap.allocated_chunks` hash table.

Memory addresses are unique so their are used as the key of the hash table while value contains the size of the chunk.

This hash table resolves collision with separate channing.

                           metadata                   user memory
                    ------------------------------------------------------------------
        CHUNK       |      48 bytes  |                512 bytes                      |   
                    ------------------------------------------------------------------
                                    

                 index         

                   0  | key: 0x7f8...,  pair: 2000 | -> | key: 0x7f8...,  size: 16 |  
       ALLOCED     1  | key: 0x7f8...,  size: 2039 | 
       CHUNKS      2  | key: 0x7f8...,  size: 4034 | -> | key: 0x7f8...,  size: 32 | 
                   3  | key: 0x7f8...,  size: 512  |  


### The bin

The bin is array of sorted doubly linked lists with freed chunks to be reused as users requests for more memory. Note that the `bin` is only used when all the memory blocks
in the `page.buffer` has been depleted.

The `bin` array contains 19 indexes where each corresponds of a size class.
`my_malloc` will look for the first chunk that fits the requested size. The linked list is sorted though so the first fit will also be the best available fit.

To optimize for speed (while consequently creating more fragmentation), the allocator uses fixed sizes for small allocation while larger allocations can be of any range. 

That said, The `bin` array is then divided between following size classes.


     FIXED SIZES

               2^2  2^3  2^4   2^5   2^6  2^7   2^8   2^9    
     size:      4    8   16    32    64   128   256   512 
     index:     0    1    2    3     4     5     6     7
   
     RANGED SIZES
   
                       2^10            2^11            2^12            2^13            2^14            2^15              
     range:     | 513 <-> 1024 | 1025 <-> 2048 | 2049 <-> 4096 | 4097 <-> 8192 | 8193 <-> 16384 | 16385 <-> 32768 
     index:             8               9               10              11              12                13
   
                        2^16              2^17                2^18                2^19        |       2^20
     range:     | 32769 <-> 65536 | 65537 <-> 131072 | 131073 <-> 262144 | 262145 <-> 409600  |  greater than 409600
     index:              14                15                  16                  17   


Because the size classes grow exponentially, we can calculate the correct index for a given size with the following formula.  


        ceil(log2 (chunk.size)) - OFFSET   where   OFFSET == 2 
        

An `OFFSET` of two compensates that the size classes start at `2^2` instead of `2^0`.


### Garbage collection

Everytime a user calls `my_free` the allocator will check if any pages have zero allocation for more than one malloc/free lifecycle and return that block of memory to the kernel.
Every page holds references to its chunks via the linked list `page.chunks` and as a doubly liked list, `bin` nodes hold a reference to both the next and previous nodes. 
This gives the allocator the ability to remove chunks from the `bin` by simply iterating through the relevant chunks in`page.chunks` instead of scanning the whole `bin` data structure.

## tests

tests using [criterion](https://github.com/Snaipe/Criterion). Compile and run:

    make test

clean tests

    make test_clean

