/********************************************************************
    Bethany Eastman
    December 2015

    Header file containing protypes and static variables for an
    implementation of malloc based on a linked list and private heap.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;
    int free;   	  // 'boolean' 0 if not available
    int *start;      // pointer to adress of memory
    struct Block *next; // next Block in list
} Block;

// static variables
static void *heap;    
static Block *list;  

// prototypes
void create_pool(int size);
void *my_malloc(int size);
void my_free(void *block);
void printlist(Block *block);
void free_pool();
void free_list();

#include "mallok.c"