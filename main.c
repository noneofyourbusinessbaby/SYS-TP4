#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "mem.h"

int main(int argc, char *argv[])
{
    // initialize the allocator
    mem_init();
    mem_show_heap();
    
    // char *p = mem_alloc( 42 ); 
    // assert( p != NULL ); // check whether the allocation was successful
    // printf("allocated 42 bytes at %p\n", p);
    // mem_show_heap();
}
