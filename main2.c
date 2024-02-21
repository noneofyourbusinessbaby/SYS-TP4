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
    
    int taille=6;

    assert(taille<=6);

    for (int i = 0; i < taille; i++)
    {
        char *p = mem_alloc( 42 ); 
        assert( p != NULL ); // check whether the allocation was successful
        mem_release(p); // on alloue puis désaloue directement 
    }
    
    char *p = mem_alloc( 42 ); 
    assert( p != NULL ); // check whether the allocation was successful
    mem_release(p); // on alloue puis désaloue directement 

    p = mem_alloc( 200 ); 
    assert( p != NULL ); // check whether the allocation was successful
    mem_release(p); // on alloue puis désaloue directement 

    p = mem_alloc( 300 ); 
    assert( p != NULL ); // check whether the allocation was successful

    mem_show_heap();
    return 0;
}
