/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                      Polymorphic Scheduler Module                         *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scheduler.h"

// different types of schedulers available
#include "fcfs.h"
#include "multi.h"
#include "linux.h"
#include "sjf.h"

#define N_QUEUES 3
#define M_QUEUES 10

algo_t schedule_algo(char* string){
	if(strcmp(string, "fcfs") == 0){
		return FCFS;
	} else if(strcmp(string, "multi") == 0){
		return MULTI;
	} else if(strcmp(string, "more") == 0){
		return MORE;
	} else if(strcmp(string, "sjf") == 0){
		return SJF;
	} else if(strcmp(string, "cfs") == 0){
		return CFS;
	} else if(strcmp(string, "linux") == 0){
		return LINUX;
	} else {
		return NONE;
	}

}

// a scheduler is a series of function definitions and some internal state
struct scheduler{
	void* state;
	bool 	(*empty)(void*);
	void 	(*add)(void*, proc_t*);
	proc_t* (*next)(void*, proc_t*, int*);
	void 	(*free)(void*);
};

// create and initialise a scheduler of a specified type
scheduler_t* scheduler_new(algo_t schedule){
	
	scheduler_t* sched = malloc(sizeof(scheduler_t));

	// initialise the scheduler with the right kinds of behaviours
	switch(schedule){
		case FCFS:
			sched->state = fcfs_init();
			sched->empty = fcfs_empty;
			sched->add   = fcfs_add;
			sched->next  = fcfs_next;
			sched->free  = fcfs_free;
			break;
		case MULTI:
			sched->state = multi_init(N_QUEUES);
			sched->empty = multi_empty;
			sched->add   = multi_add;
			sched->next  = multi_next;
			sched->free  = multi_free;
			break;
		case MORE:
			sched->state = multi_init(M_QUEUES);
			sched->empty = multi_empty;
			sched->add   = multi_add;
			sched->next  = multi_next;
			sched->free  = multi_free;
			break;
		case LINUX:
			sched->state = linux_init(M_QUEUES);
			sched->empty = linux_empty;
			sched->add   = linux_add;
			sched->next  = linux_next;
			sched->free  = linux_free;
			break;
		case SJF:
			sched->state = sjf_init();
			sched->empty = sjf_empty;
			sched->add   = sjf_add;
			sched->next  = sjf_next;
			sched->free  = sjf_free;
			break;
		default:
			fprintf(stderr, "Error: Invalid scheduler type. Exiting.\n");
			exit(EXIT_FAILURE);
			break;
	}

	return sched;
}

/* * * * * * * * * * * WRAPPERS * * * * * * * * * * */
// responding with the specified behaviour for the scheduler type

bool schedule_empty(scheduler_t* sched){
	return sched->empty(sched->state);
}

void schedule_add(scheduler_t* sched, proc_t* proc){
	sched->add(sched->state, proc);
}

proc_t* schedule_next(scheduler_t* sched, proc_t* proc, int* alarm){
	return sched->next(sched->state, proc, alarm);
}

void scheduler_free(scheduler_t* sched){
	sched->free(sched->state);
	free(sched);
}