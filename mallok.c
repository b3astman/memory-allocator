/********************************************************************
Bethany Eastman
December 2015

A linked list implementation of malloc. mallok.c will create
a 'pool' of bytes which is allocated memory, and allows a client
to allocate bytes from this private heap. Further, a client will
be able to free this memory with my_free and free_pool.
*/

/********************************************************************
create_pool - Creates a block of the exact size of the parameter.
*/
void create_pool(int size) {
    void *allocated = malloc(size);
    list = malloc(sizeof(Block));
    list->size = size;          // set default size
    list->free = 1; 	        // set to free
    list->start = allocated;    // set pointer to block allocated
    list->next = NULL;

    heap = allocated;			// create heap
}

/********************************************************************
my_malloc - get first free block big enough to satisfy the size
of the parameter. Then, my_malloc will use the first part of
the block to satisfy the request then use the rest in a private 
heap. Will return NULL if parameter cannot be satisfied.
*/
void * my_malloc(int size) {
    Block *scan = list;
    void * returnValue = NULL;
    int space = 0;

    while (scan != NULL && (!scan->free || scan->size < size)) {
        scan = scan->next; // get block satisfying size
    }

    if (scan != NULL) { 	// if block is available
        scan->free = 0;		// it is no longer free
        if (size <= scan->size) {	
            space = scan->size;		
            scan->size = size; 

            if (space - size != 0) { // if block larger than size
                // create block for extra space
                Block *newBlock = malloc(sizeof(Block));
                newBlock->free = 1;  // block for extra space is free
                newBlock->size = space - size;
                newBlock->start = scan->start + size;
                newBlock->next = scan->next; 

                scan->next = newBlock; // point previous block to new
                }
            }
            returnValue = scan->start; // set return value on success
    }

    return returnValue;            // NULL if no block available
}

/********************************************************************
my_free - returns a block to the private heap making the
storage available again. Will merge neighboring blocks if free
space available.
*/
void my_free(void *block) {
    Block *scan, *back = NULL;

    if (list != NULL) {
        scan = list;
        while (scan != NULL && scan->start != block) {
            back = scan;
            scan = scan->next;
        }

        if (scan != NULL) {
            scan->free = 1;

            if (back != NULL && scan->next != NULL) {
                // if block not first or last Block (middle)
                if (back->free) { // check if previous block free
                    back->size += scan->size;
                    back->next = scan->next;
                    scan = back;
                }

                if (scan->next->free) { // check if next is free
                    scan->size += scan->next->size;
                    scan->next = scan->next->next;
                }
            } else if (scan == list) {
                // if block is last, check next is also free
                if (scan->next != NULL && scan->next->free) {
                    scan->size += scan->next->size;
                    scan->next = scan->next->next;
                }
            } else if (scan->next == NULL) {
                // if block is last block, check previous is free
                if (back->next != NULL && back->free) {
                    back->size += scan->size;
                    back->next = scan->next;
                }
            }
        }
    }
}

/*******************************************************************
printlist - print info of linked list, helpful for debugging.
*/
void printlist(Block *block) {
    if (block) {
        printf("[Block] Address: %p, Size (Bytes): %d, " 
        "Is Free: %d\n", block->start, block->size, block->free);
        printlist(block->next);
    }
}

/********************************************************************
free_pool - free every node in a linked list and then free all
space allocated in the heap.
*/
void free_pool() {
    free_list(list);
    free(heap);
}

/********************************************************************
free_list - frees a linked list.
*/
void free_list(Block *list) {
    if (list) {
        free_list(list->next);
        free(list);
    }
}
