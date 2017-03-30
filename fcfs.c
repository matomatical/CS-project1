/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                 First-Come, First-Serve Queue Scheduler                   *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "fcfs.h"
#include "queue.h"

// first come first serve scheduling algorithm just needs a single queue
typedef struct {
	queue_t* q;
} fcfs_state_t;


// set up state
void* fcfs_init(){
	fcfs_state_t* s = malloc(sizeof(fcfs_state_t));

	s->q = queue_new();

	return s;
}

// clean up state
void fcfs_free(void* state){
	fcfs_state_t* s = state;

	queue_free(s->q);

	free(s);
}

// is this scheduler empty?
bool fcfs_empty(void* state){
	fcfs_state_t* s = state;
	
	return ! (queue_length(s->q) > 0) ;
}

// add a new process to this scheduler
void fcfs_add(void* state, proc_t* proc){
	fcfs_state_t* s = state;

	queue_enqueue(s->q, proc);
}

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* fcfs_next(void* state, proc_t* proc, int* alarm){
	fcfs_state_t* s = state;
	
	if(proc){
		return proc;
	} else {
		return queue_dequeue(s->q);
	}
}