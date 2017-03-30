/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *              First-Come, First-Serve Queue Scheduler Module               *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>

#include "procs.h"

// set up state
void* fcfs_init();

// clean up state
void fcfs_free(void* state);

// is this scheduler empty?
bool fcfs_empty(void* state);

// add a new process to this scheduler
void fcfs_add(void* state, proc_t* proc);

// get the next process from this scheduler, and give it back the current one
// can set scheduling alarm to ensure a clock interrupt
proc_t* fcfs_next(void* state, proc_t* proc, int* alarm);