/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                   Shortest Job First Scheduler Module                     *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>

#include "procs.h"

// set up state
void* sjf_init();

// clean up state
void sjf_free(void* state);

// is this scheduler empty?
bool sjf_empty(void* state);

// add a new process to this scheduler
void sjf_add(void* state, proc_t* proc);

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* sjf_next(void* state, proc_t* proc, int* alarm);