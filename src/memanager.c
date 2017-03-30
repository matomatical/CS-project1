/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           Memory Management Module                        *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 10/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "memanager.h"
#include "memory.h"

// a memory manager is just a wrapper for a memory storage unit
// that provides first fit and removal of oldest largest block logic
struct memanager{
	memory_t* memo;
};

/* * * * * * * * * * * HELPER FUNCTION PROTOTYPES * * * * * * * * * * */

// store a process in a memory block at a certain time, if it will fit
bool stock(block_t* block, proc_t* proc, int clock);

// find the first free block with a given amount of space
block_t* first_fit(memory_t* memo, int size);

// clear the oldest of the largest blocks in memory and
// return the resulting free block
block_t* clear_big_old(memory_t* memo);


/* * * * * * * * * * * FUNCTION CODE * * * * * * * * * * */

// returns a pointer to a new, fresh memory manager with a memory unit of a
// specified size
memanager_t* memanager_new(int size){
	memanager_t* meman = malloc(sizeof(memanager_t));

	meman->memo = memory_new(size);

	return meman;
}

// free a memory manager
void memanager_free(memanager_t* meman){
	memory_free(meman->memo);
	free(meman);
}

// ask a memory manager to load a particular process at a certain time
void memanager_load(memanager_t* meman, proc_t* proc, int clock){

	block_t* block = first_fit(meman->memo, proc_memory_size(proc));
	
	if(block){
		// found a fit on the first pass!
		stock(block, proc, clock);
	} else {
		// no fits at the moment, we'll have to try removing something
		// and placing it in the space we make
		while((block = clear_big_old(meman->memo))){
			// we successfully cleared a block, let's try it
			if(stock(block, proc, clock)){
				// we found a spot and successfully put it in!
				return;
			}
		}
		// if we get here, there are NO BLOCKS BIG ENOUGH
		fprintf(stderr,
			"Fatal error: process %d too big for memory: %dMB\n",proc_pid(proc),
			proc_memory_size(proc));
		exit(EXIT_FAILURE);
	}
}

// ask a memory manager to unload one instance of a particular process
void memanager_unload(memanager_t* meman, proc_t* proc){
	
	memory_rewind(meman->memo);

	block_t* block = memory_next_used(meman->memo);

	while(block){
		if(block_proc(block) == proc){
			// found our process!
			block_clear(block);
			return;
		}
		block = memory_next_used(meman->memo);
	}

	// if we make it here then the process wasn't in memory so no need to free
}

// ask a memory manager to prints its current status
void memanager_print_status(memanager_t* meman, FILE* out){
	fprintf(out, "numprocesses=%d, numholes=%d, memusage=%d%%",
		memory_nused(meman->memo),
		memory_nfree(meman->memo),
		memory_usage(meman->memo));
}

/* * * * * * * * * * * FUNCTION CODE * * * * * * * * * * */

// store a process in a memory block at a certain time, if it will fit
bool stock(block_t* block, proc_t* proc, int clock){
	if(block && proc && block_size(block) >= proc_memory_size(proc)){
		block_add(block, proc, clock);
		proc_swap_in(proc);
		return true;
	} else {
		return false;
	}
}

// find the first free block with a given amount of space
block_t* first_fit(memory_t* memo, int size){
	memory_rewind(memo);

	block_t* block = memory_next_free(memo);
	
	while(block){
		if(block_size(block) >= size){
			// we found a block that will fit this proc!
			return block;
		}
		// check next block
		block = memory_next_free(memo);
	}
	// we didn't find a fit
	return NULL;
}

// clear the oldest of the largest blocks in memory and
// return the resulting free block
block_t* clear_big_old(memory_t* memo){

	memory_rewind(memo);

	block_t* block = memory_next_used(memo);
	block_t* best = NULL;

	while(block){
		if(!best || block_size(best) < block_size(block)){
			best = block; // found a bigger one!

		} else if(block_size(best) == block_size(block)
					&& block_time(best) > block_time(block)){
			best = block; // found an older one!
		}
		// try the next one
		block = memory_next_used(memo);
	}
	
	// clear this block and return it
	if(best){
		best = block_clear(best);
	}
	return best;
}