/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                Multilevel Feedback Queue Scheduler Module                 *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>

#include "procs.h"

// set up scheduler state
void* multi_init(int n_queues);

// clean up scheduler state
void multi_free(void* state);

// is this scheduler empty?
bool multi_empty(void* state);

// add a process to this scheduler
void multi_add(void* state, proc_t* proc);

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* multi_next(void* state, proc_t* proc, int* alarm);