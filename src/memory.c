/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                                Memory Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 09/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "memory.h"

// memory is an iterable chain of contigious blocks with a fixed total size
struct memory{
	block_t* top;
	block_t* next;
	int size, used;
	int nused, nfree;
};

// blocks are contigious regions of memory which may be free,
// or may be occupied by a process
// they form part of a memory block chain (a doubly linked list of blocks)
struct block{
	proc_t* proc; // NULL for free blocks
	int timestamp;
	int size;
	block_t* prev;
	block_t* next;
	memory_t* memo;
};

/* * * * * * * * * * * HELPER FUNCTION PROTOTYPES * * * * * * * * * * */

// allocate a new block
block_t* block_new(memory_t* memo, proc_t* proc, int timestamp, int size);
//　free an existing block
void block_free(block_t* block);
// merge a block (a) with the block under it (b) and return the resulting block
block_t* block_merge(block_t* a, block_t* b);


/* * * * * * * * * * * MEMORY FUNCTIONS * * * * * * * * * * */

// return a new memory unit with a given total size
memory_t* memory_new(int size){
	memory_t* memo = malloc(sizeof(memory_t));

	memo->size = size;

	// start with one big empty block
	memo->top = block_new(memo, NULL, -1, size);
	memo->nfree = 1;

	memo->used = 0;
	memo->nused = 0;

	// start iterator off
	memo->next = NULL;

	return memo;
}

// free a memory segment and all its blocks
void memory_free(memory_t* memo){
	block_t* next;
	block_t* now = memo->top;

	while(now){
		next = now->next;
		block_free(now);
		now = next;
	}
	free(memo);
}

// get the (rounded up) percentage memory usage
int memory_usage(memory_t* memo){

	// use floor division, and then add 1 unless we didn't round down
	return 100 * memo->used / memo->size + !! ((100 * memo->used) % memo->size);
}

// get the number of blocks in a memory unit that are occupied by processes
int memory_nused(memory_t* memo){
	return memo->nused;
}

// get the number of blocks in a memory unit that are unoccupied
int memory_nfree(memory_t* memo){
	return memo->nfree;
}



/* * * * * * * * * * * BLOCK FUNCTIONS * * * * * * * * * * */

// getters
int block_size(block_t* block){
	return block->size;
}
int block_time(block_t* block){
	return block->timestamp;
}
proc_t* block_proc(block_t* block){
	return block->proc;
}


// add a process to a block at a certain time
void block_add(block_t* block, proc_t* proc, int timestamp){

	// first of all, check that the block fits
	if(!block || block->proc || proc_memory_size(proc) > block->size){
		fprintf(stderr,
			"Trying to fit proc into invalid block (null, used, or small)\n");
		exit(EXIT_FAILURE);
	}

	// make the new block to add
	block_t* new_block = block_new(block->memo, proc, timestamp,
		proc_memory_size(proc));
	block->memo->nused += 1;
	block->memo->used += proc_memory_size(proc);


	// attach it at the top
	new_block->prev = block->prev;
	if(block->prev){
		block->prev->next = new_block;
	} else {
		// omg it took me about 6 hours to figure out that
		// forgetting this was the cause of my segfaults
		block->memo->top = new_block;
	}

	if(new_block->size < block->size){
		// there's space for the blocks to share

		// reduce the size of the original block
		block->size -= new_block->size;

		// and attach it to the bottom of the new block
		new_block->next = block;
		block->prev = new_block;
	} else {
		// they must be equal sizes! hijak this block's previous and delete it
		new_block->next = block->next;
		if(block->next){
			block->next->prev = new_block;
		}

		block->memo->nfree -= 1;
		block_free(block);
	}

	// done!
}


// remove the process from a given block
// returns the cleared block (potentially merged with neighboring clear blocks)
block_t* block_clear(block_t* block){

	if(!block || !block->proc){
		// maybe block is already free
		return block;
	}

	// otherwise, make block free

	proc_swap_out(block->proc);

	block->memo->used -= proc_memory_size(block->proc);
	block->proc = NULL;
	block->timestamp = -1;

	block->memo->nused -= 1;
	block->memo->nfree += 1;

	// now if the block above is also free, merge with it
	if(block->prev && !block->prev->proc){
		block = block_merge(block->prev, block);
	}

	if(block->next && !block->next->proc){
		block = block_merge(block, block->next);
	}

	return block;
}


/* * * * * * * * * * * ITERATOR FUNCTIONS * * * * * * * * * * */

// restart the iterator (required initially and after any block alterations)
void memory_rewind(memory_t* memo){
	memo->next = memo->top;
}

// iterate to the next block of memory
block_t* memory_next(memory_t* memo){
	// memo->next points to the one we want to return,
	// but we also want to increment it as well as returning!
	block_t* block = memo->next;
	if(memo->next){
		memo->next = memo->next->next;
	}
	return block;
}

// iterate to the next free block of memory
block_t* memory_next_free(memory_t* memo){
	
	// skip all blocks with procs
	while(memo->next && memo->next->proc){
		memo->next = memo->next->next;
	}

	// now memo->next points to the one we want to return
	// but we also want to increment it as well as returning!
	block_t* block = memo->next;
	if(memo->next){
		memo->next = memo->next->next;
	}
	return block;
}

// iterate to the next occupied block of memory
block_t* memory_next_used(memory_t* memo){

	// skip all blocks WITHOUT procs
	while(memo->next && ! memo->next->proc){
		memo->next = memo->next->next;
	}

	// now memo->next points to the one we want to return
	// but we also want to increment it as well as returning!
	block_t* block = memo->next;
	if(memo->next){
		memo->next = memo->next->next;
	}

	return block;
}


/* * * * * * * * * * * BLOCK HELPER FUNCTIONS * * * * * * * * * * */

// allocate a new block
block_t* block_new(memory_t* memo, proc_t* proc, int timestamp, int size){
	block_t* block = malloc(sizeof(block_t));

	block->memo = memo;
	block->proc = proc;
	block->size = size;
	block->timestamp = timestamp;

	block->prev = NULL;
	block->next = NULL;

	return block;
}

//　free an existing block  (きぞんのブロックをかいほうして) (haha I know Japanese)
void block_free(block_t* block){
	
	// clear iterator if it points here
	if(block && block->memo->next==block){
		block->memo->next = NULL;
	}

	// either way, free this block
	free(block);
}

// merges the second free block below the first free block
// and returns the merged block
// assumes that b == a->next and a and b are not NULL
block_t* block_merge(block_t* a, block_t* b){

	a->next = b->next;
	if(b->next){
		b->next->prev = a;
	}

	a->size += b->size;

	block_free(b);
	a->memo->nfree -= 1;

	return a;
}

/* * * * * * * * * * * PRINTING FUNCTIONS * * * * * * * * * * */

void memory_print(memory_t* memo){
	block_t* block = memo->top;
	while(block){
		printf("[%p:%p(%d)]-->", block, block->proc, block->size);
		block = block->next;
	}
	printf("\n");
}