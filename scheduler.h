/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                      Polymorphic Scheduler Module                         *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>

#include "procs.h"

// enumerated scheduling algorithm type
typedef enum {
	NONE, FCFS, MULTI, MORE, SJF, CFS, LINUX,
} algo_t;

// get the algo_t named by a string
algo_t schedule_algo(char* string);


// a process scheduler
typedef struct scheduler scheduler_t;

// create a new schedule of a specified type
scheduler_t* scheduler_new(algo_t schedule);

// free a scheduler
void scheduler_free();

// ask if a scheduler has no more processes to schedule
bool schedule_empty(scheduler_t* sched);
// add a new process to a scheduler
void schedule_add(scheduler_t* sched, proc_t* proc);

// get the next process from this scheduler, and give it back the current one
// also pass in an alarm to set for a a clock interrupt
proc_t* schedule_next(scheduler_t* sched, proc_t* proc, int* alarm);