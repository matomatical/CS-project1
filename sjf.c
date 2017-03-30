/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                   Shortest Job First Scheduler Module                     *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "sjf.h"
#include "heap.h"

// first come first serve scheduling algorithm just needs a single queue
typedef struct {
	heap_t* h;
} sjf_state_t;


// set up state
void* sjf_init(){
	sjf_state_t* s = malloc(sizeof(sjf_state_t));

	s->h = heap_new();

	return s;
}

// clean up state
void sjf_free(void* state){
	sjf_state_t* s = state;

	heap_free(s->h);
	free(s);
}

// is this scheduler empty?
bool sjf_empty(void* state){
	sjf_state_t* s = state;
	
	return heap_is_empty(s->h);
}

// add a new process to the schedule
void sjf_add(void* state, proc_t* proc){
	sjf_state_t* s = state;

	heap_insert(s->h, proc, proc_remaining_time(proc));
}

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* sjf_next(void* state, proc_t* proc, int* alarm){
	sjf_state_t* s = state;
	
	if(proc){
		return proc;
	} else {
		return heap_remove_min(s->h);
	}
}