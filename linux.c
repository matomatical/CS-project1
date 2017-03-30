/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                      Linux's O(1) Scheduler Module                        *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "linux.h"
#include "queue.h"

// a O(1) scheduler needs two priority arrays of queues
typedef struct {
	queue_t **active_queues;
	queue_t **expire_queues;
	int* quanta;
	int prev; // the previous priority
	int n_procs;
	int n_queues;
} linux_state_t;

// set up scheduler state
void* linux_init(int n_queues){
	linux_state_t* s = malloc(sizeof(linux_state_t));

	s->n_queues = n_queues;

	s->active_queues = malloc(sizeof(queue_t*)*n_queues);
	s->expire_queues = malloc(sizeof(queue_t*)*n_queues);
	s->quanta = malloc(sizeof(int)*n_queues);

	for(int i = 0; i < n_queues; i++){
		s->active_queues[i] = queue_new();
		s->expire_queues[i] = queue_new();
		s->quanta[i] = 2*(n_queues-i) + 10; // this is the important part!
	}

	s->prev = 0;
	s->n_procs = 0;

	return s;
}

// clean up scheduler state
void linux_free(void* state){
	linux_state_t* s = state;

	for(int i = 0; i < s->n_queues; i++){
		queue_free(s->active_queues[i]);
		queue_free(s->expire_queues[i]);
	}

	free(s->active_queues);
	free(s->expire_queues);
	free(s->quanta);

	free(s);
}

// is this scheduler empty?
bool linux_empty(void* state){
	linux_state_t* s = state;

	return (s->n_procs == 0);
}

// add a process to this scheduler
void linux_add(void* state, proc_t* proc){
	linux_state_t* s = state;

	s->n_procs += 1;
	// always add new processes to end of FIRST queue
	queue_enqueue(s->active_queues[0], proc);
}

// min of two integers
static int min(int a, int b){
	return (a < b) ? a : b;
}

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* linux_next(void* state, proc_t* proc, int* alarm){
	linux_state_t* s = state;

	if(proc){
		// time's out! back in the (next) queue you go
		queue_enqueue(s->expire_queues[min(s->prev+1, s->n_queues-1)], proc);
		s->n_procs++;
	}

	for(; s->prev < s->n_queues; s->prev++){
		if(queue_peek(s->active_queues[s->prev])){
			// there's something in the current queue!

			// set the alarm so that there's a clock interrupt soon
			*alarm = s->quanta[s->prev];

			// return this process for running
			s->n_procs--;
			return queue_dequeue(s->active_queues[s->prev]);
		}
	}

	// there was nothing in the active list if we get to here
	s->prev = 0;

	// swap queues
	queue_t** temp_queues;
	temp_queues = s->active_queues;
	s->active_queues = s->expire_queues;
	s->expire_queues = temp_queues;

	// try again with the other queues now
	for(; s->prev < s->n_queues; s->prev++){
		if(queue_peek(s->active_queues[s->prev])){
			// there's something in the current queue!

			// set the alarm so that there's a clock interrupt soon
			*alarm = s->quanta[s->prev];

			// return this process for running
			s->n_procs--;
			return queue_dequeue(s->active_queues[s->prev]);
		}
	}

	// there was nothing in any of the queues, if we get to here
	return NULL;
}
