#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

// initialize the memory manager
//
// this function must be called just once, before any calls to mem_alloc or mem_release
void mem_init(void);

// allocate a region of memory and return its address (or NULL in case of error)
void * mem_alloc0(int64_t length);

// allocate a region of memory returns its adress while cropping the corresponding bloc
void * mem_alloc1(int64_t length);

// deallocate a region of memory
void mem_release(void* ptr);


// pretty-print the heap (useful for debug)
void mem_show_heap();


#endif /* _MEM_H_ */


//— mem_init() permet d’initialiser le gestionnaire mémoire. L’application doit appeler cette fonction
// une seule fois, au début de l’exécution.

// — mem_alloc(int64_t length) permet de demander l’allocation d’une zone de mémoire. Cette
// fonction renvoie l’adresse de début de la région, ou NULL en cas d’erreur.

// — mem_release(void *ptr) permet de libérer une zone de mémoire. Le pointeur doit correspondre à une zone préalablement allouée avec mem_alloc().

// la zone mémoire désignée dans les deux est elle un bloc 

// — mem_show_heap() est là juste pour vous aider à débugger. Elle affiche le contenu du tas, en
// détaillant les informations disponibles sur chaque bloc.


// fragementation externe => on a plein de blocs de tailles différentes 
// fragmentation interne => on ne remplit pas tout le bloc
