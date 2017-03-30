/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                                Memory Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 09/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
#include "procs.h"

// blocks are contigious regions of memory which may be free,
// or may be occupied by a process
// memory is an iterable chain of contigious blocks with a fixed total size
typedef struct block block_t;
typedef struct memory memory_t;


/* * * * * * * * * * * MEMORY FUNCTIONS * * * * * * * * * * */

// return a new memory unit with a given total size
memory_t* memory_new(int size);

// free a memory segment and all its blocks
void memory_free(memory_t* memo);

// get the (rounded up) percentage memory usage
int memory_usage(memory_t* memo);
// get the number of blocks in a memory unit that are occupied by processes
int memory_nused(memory_t* memo);
// get the number of blocks in a memory unit that are unoccupied
int memory_nfree(memory_t* memo);


/* * * * * * * * * * * BLOCK FUNCTIONS * * * * * * * * * * */

// getters
int block_size(block_t* block);
int block_time(block_t* block);
proc_t* block_proc(block_t* block);

// add a process to a block at a certain time
void block_add(block_t* block, proc_t* proc, int timestamp);

// remove the process from a given block
// returns the cleared block (potentially merged with neighboring clear blocks)
block_t* block_clear(block_t* block);


/* * * * * * * * * * * ITERATOR FUNCTIONS * * * * * * * * * * */

// restart the iterator (required initially and after any block alterations)
void memory_rewind(memory_t* memo);

// iterate to the next block of memory
block_t* memory_next(memory_t* memo);

// iterate to the next free block of memory
block_t* memory_next_free(memory_t* memo);

// iterate to the next occupied block of memory
block_t* memory_next_used(memory_t* memo);
