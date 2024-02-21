#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "mem.h"

// Partie 1 :
// Exercice Écrivez dans votre main.c différents cas de test permettant d’explorer les cas «limite» de
// l’algorithme d’allocation. Par exemple :


// — Vérifiez que vous pouvez allouer successivement six blocs de petite taille, par exemple 42 octets.
void alloue6blocs(int taille)
{
    for (int i = 0; i < 6; i++)
    {
        char *p = mem_alloc( taille ); 
        assert( p != NULL ); // check whether the allocation was successful 
    }
    

}
// — Vérifiez que vous ne pouvez pas allouer un septième bloc : il n’y a plus de blocs libres et
// l’allocateur doit renvoyer NULL.
void alloue7emebloc(int taille)
{
        char *p = mem_alloc( taille );
        assert( p == NULL ); // check whether the allocation was successful 

}
// — Vérifiez que les tailles sont traitées correctement : une allocation de 200 doit aller dans le bloc
// libre de 400. Et une seconde allocation de 200 doit échouer.
void traitementCorrect(int taille)
{
        

        char *p = mem_alloc( taille );
        int64_t header = *( (int64_t*)p );
        int64_t size   = header & ~0b111; // discard the three least significant bits
        printf("%ld\n",size);
        assert( p != NULL ); // check whether the allocation was successful
        p = mem_alloc( taille );
        assert( p == NULL ); // check whether the allocation was successful
}

int main(int argc, char *argv[])
{
    (void)argv;
    (void)argc;
    // initialize the allocator
    mem_init();
    mem_show_heap();
    
    // char *p = mem_alloc( 42 ); 
    // assert( p != NULL ); // check whether the allocation was successful 
    // printf("allocated 42 bytes at %p\n", p);
    // mem_show_heap();
    if (argc>1)
    {
        alloue6blocs(42);
        alloue7emebloc(42);
    }
    else
    {
        traitementCorrect(200);
    }
    mem_show_heap();
    return 0;
}
