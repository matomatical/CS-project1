/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                Multilevel Feedback Queue Scheduler Module                 *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "multi.h"
#include "queue.h"

typedef struct {
	queue_t **queues;
	int* quanta;
	int prev;
	int n_queues;
} multi_state_t;

// set up scheduler state
void* multi_init(int n_queues){
	multi_state_t* s = malloc(sizeof(multi_state_t));

	s->queues = malloc(sizeof(queue_t*)*n_queues);
	s->quanta = malloc(sizeof(int)*n_queues);

	for(int i = 0, j=1; i < n_queues; i++){
		s->queues[i] = queue_new();
		s->quanta[i] = (j*=2);
	}

	s->prev = 0;
	s->n_queues = n_queues;

	return s;
}

// clean up scheduler state
void multi_free(void* state){
	multi_state_t* s = state;

	for(int i = 0; i < s->n_queues; i++){
		queue_free(s->queues[i]);
	}

	free(s->queues);
	free(s->quanta);

	free(s);
}

// is this scheduler empty?
bool multi_empty(void* state){
	multi_state_t* s = state;

	bool empty = true;

	for(int i = 0; i < s->n_queues; i++){
		if(queue_length(s->queues[i])>0){
			empty = false;
		}
	}

	return empty;
}

// add a process to this scheduler
void multi_add(void* state, proc_t* proc){
	multi_state_t* s = state;

	// always add to end of FIRST queue
	queue_enqueue(s->queues[0], proc);
}

// min of two integers
static int min(int a, int b){
	return (a < b) ? a : b;
}

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* multi_next(void* state, proc_t* proc, int* alarm){
	multi_state_t* s = state;

	if(proc){
		// time's out! back in the (next) queue you go
		queue_enqueue(s->queues[min(s->prev+1, s->n_queues-1)], proc);
	}

	for(int i = 0; i < s->n_queues; i++){
		if(queue_peek(s->queues[i])){
			// there's something in the ith queue!
			s->prev = i;

			// set the alarm so that there's a clock interrupt soon
			*alarm = s->quanta[i];

			// return this process for running
			return queue_dequeue(s->queues[i]);
		}
	}

	// there was nothing in any of the queues, if we get to here
	return NULL;
}
