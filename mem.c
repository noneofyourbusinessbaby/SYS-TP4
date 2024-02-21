#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>

#include "mem.h"

void * heap_base = NULL;// first address of the heap
void * heap_end  = NULL;// first address beyond the heap

int mem_initialized = 0;

// initialize the memory manager

// — la fonction d’initialisation mem_init() est déjà écrite. Elle commence par un appel mmap() pour

// demander de la mémoire au noyau. Dans ce TP, le tas fait 800 octets en tout. Ensuite (seulement
// pour des raisons pédagogiques) elle crée des blocs libres dans ce tas : cinq blocs de 80 octets,
// suivis d’un bloc de 400 octets. Le résultat est illustré ci-dessous, figure 1(a).


// — la fonction mem_release() est vide : pour l’instant, notre gestionnaire mémoire ne sait pas

// désallouer. On règlera ce problème dans la partie 2 du TP, page suivante. En attendant, remarquez que «ne rien faire» est une implémentation valide, qui est certes insatisfaisante, mais qui
// respecte le contrat de notre interface.


// — la fonction mem_alloc() est déjà implémentée, dans une version simple : elle parcourt le tas de

// bloc en bloc jusqu’à en trouver un qui soit à la fois libre et assez grand (autrement dit : allocation
// en first-fit sans découpage). Lisez le code, exécutez-le en rajoutant des printf pour visualiser
// ce qui se passe, et posez des questions sur ce que vous ne comprenez pas. Remarque : le
// format des blocs en mémoire est illustré ci-dessous, figure 1(b).

// — la fonction mem_show_heap() est déjà implémentée également. Elle est très similaire à la précédente : on parcourt le tas de bloc en bloc, en affichant les méta-données de chaque bloc.
// Assurez-vous de comprendre comment sont manipulées ces méta-données :
// — x & y se lit «ET bit-à-bit entre x et y», par ex : 0b0110 & 0b1010 == 0b0010 == 2
// — x | y se lit «OU bit-à-bit entre x et y», par ex : 0b0110 | 0b1010 == 0b1110 == 14
// — ~x se lit «NON bit-à-bit de x», par exemple sur 4 bits : ~0b0110 == 0b1001 == 9


void mem_init(void)
{
    // request memory from the kernel
    heap_base = mmap(NULL, 800, PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    assert(heap_base != MAP_FAILED);

    heap_end = heap_base + 800 ;

    // create some free blocks: five of 80 bytes and one of 400 bytes
    void *block_ptr;
    for(int i=0; i<5;i++)
    {
        block_ptr = heap_base + 80*i;
        *( (int64_t*)block_ptr ) = 80;
    }
    block_ptr = heap_base + 400;
    *( (int64_t*)block_ptr ) = 400;
    
    mem_initialized = 1;
}

void * mem_alloc0(int64_t length)
{
    assert(mem_initialized == 1);
    
    // compute actual size of block
    length = (length+7)/8*8 ; // round up to nearest multiple of 8
    length += 8;              // add space for the header

    // heap traversal
    void *  block_ptr ;
    int64_t header ;
    int64_t size;   
    char    flags;
    
    block_ptr = heap_base;
    while(block_ptr < heap_end)
    {
        header = *( (int64_t*)block_ptr );
        flags  = header & 0b111;  // keep only three least significant bits
        size = header & ~0b111;   // discard the three least significant bits

        if( (flags == 0 ) &&      // if current block is free, and
            (size >= length))     // is also large enough, then we have a winner
            break;

        // otherwise we move on to the next block
        block_ptr += size;
    }

    // if the heap  traversal reached this far, then it  means we have
    // found no suitable block, so we should return NULL
    if(block_ptr >= heap_end)
    {
        return NULL;
    }

    flags = 0b001; // mark block as taken
    header = size | flags;
    *( (int64_t*)block_ptr ) = header ; // write header back into the block

    return block_ptr + 8 ; // skip header
}

void mem_release(void *ptr)
{
    assert( mem_initialized == 1);
    assert( ((int64_t)ptr % 8) == 0 ); // sanity check

    // TODO: your deallocation algorithm goes here
    //     Exercice Implémentez la fonction mem_release().

    // — À ce stade, on ne s’intéresse pas encore au découpage ou à la fusion de blocs. Votre tâche
    // consiste donc uniquement à marquer le bloc comme libre.
    // — Attention, le paramètre ptr que vous passe l’application pointe sur l’espace utile, c’est à dire la
    // zone data dans la figure 1(b), et non pas sur le début du bloc. Il faut soustraire 8 pour retomber
    // sur l’en-tête.

    ptr=ptr-8;// on tombe sur l'entête
    int64_t header = *( (int64_t*)ptr ); // on récupère le header
    int flags  = header & 0b111;
    if (flags) // si alloué
    {
       *( (int64_t*)ptr )=header-1;
    }
    header = *( (int64_t*)ptr ); // on récupère le header
    assert((header & 0b111)==0b000);
    
    return;
}

void mem_show_heap(void)
{
    assert( mem_initialized == 1);
    
    void * block_ptr = heap_base;

    printf("heap_base = %p\n",heap_base);
    while(block_ptr < heap_end)
    {
        int64_t header = *( (int64_t*)block_ptr );
        char    flags  = header & 0b111;  //   keep only three least significant bits
        int64_t size   = header & ~0b111; // discard the three least significant bits
        if( (size < 8) ||
            (size%8 != 0) )
        {
            printf("error: block at %p has incorrect size %ld\n",block_ptr,size);
            exit(1);
        }

        printf("  block at %p: header=0x%08lx size=%ld flags=%d (%s)\n",
               block_ptr,header,size,flags,
               (flags ? "taken" : "free")
               );

        block_ptr += size; // move on to next block
    }

    printf("heap_end = %p\n",heap_end);

    //sanity check: a full heap traversal should reach *exactly* the end
    assert( block_ptr == heap_end); 
}


void * mem_alloc1(int64_t length)
{
    assert(mem_initialized == 1);
    
    // compute actual size of block
    length = (length+7)/8*8 ; // round up to nearest multiple of 8
    length += 8;              // add space for the header

    // heap traversal
    void *  block_ptr ;
    int64_t header ;
    int64_t size;
    int64_t size_init;
    char    flags;
    
    block_ptr = heap_base;
    while(block_ptr < heap_end)
    {
        header = *( (int64_t*)block_ptr );
        flags  = header & 0b111;  // keep only three least significant bits
        size = header & ~0b111;   // discard the three least significant bits
        size_init=size;
        if( (flags == 0 ) &&      // if current block is free, and
            (size >= length))
            {  
                if (size-length-8>=0)
                {
                    size=length;
                }
                
                break;
            }

        // otherwise we move on to the next block
        block_ptr += size;
    }

    // if the heap  traversal reached this far, then it  means we have
    // found no suitable block, so we should return NULL
    if(block_ptr >= heap_end)
    {
        return NULL;
    }

    flags = 0b001; // mark block as taken
    header = size | flags;
    *( (int64_t*)block_ptr ) = header ; // write header back into the block

    if (size_init!=size)
    {
        flags = 0b000;
        block_ptr=block_ptr+size;
        size=size_init-size;
        header = size| flags;
        *( (int64_t*)block_ptr ) = header ; // write header back into the block
        block_ptr=block_ptr-size;
    }

    


    return block_ptr + 8 ; // skip header
}