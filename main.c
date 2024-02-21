#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "mem.h"

int main(int argc, char *argv[])
{
    (void)argv;
    (void)argc;
    // initialize the allocator
    mem_init();
    mem_show_heap();
    
    for (int i = 0; i < 7; i++)
    {
        char *p = mem_alloc1( 20 ); 
        assert( p != NULL ); // check whether the allocation was successful 
    }
    

    // printf("allocated 42 bytes at %p\n", p);
    mem_show_heap();
    return 0;
}
