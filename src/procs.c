/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                               Process Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 08/04/16 by Matt Farrugia                  *
 * Notes:                                                                    *
 *  processes and jobs are similar, in my simulation i consider the list of  *
 *  CPU tasks in the input file 'jobs' which arrive at a certain time, upon  *
 *  which a corresponding 'proc' is created (which has remaining time, etc)  *
 *  So, a job is a request to start a process at a certain time, and procs   *
 *  are the tasks that the scheduler must share CPU time between             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdbool.h>

#include "procs.h"

// a model process
struct proc{
	int jid, pid; // the job array index and process id
	int remaining_time;
	int memory_size;
	bool in_memory;
};

// allocate, initialise and return a new process with given values
// 'in_memory' defaults to false as processes begin their life on the disk
proc_t* proc_new(int jid, int pid, int remaining_time, int memory_size){

	proc_t* new_proc = malloc(sizeof(proc_t));

	new_proc->jid = jid;
	new_proc->pid = pid;
	new_proc->remaining_time = remaining_time;
	new_proc->memory_size = memory_size;
	new_proc->in_memory = false; // processes begin their life on the disk

	return new_proc;
}

// getters
int proc_jid(proc_t* proc){
	return proc->jid;
}
int proc_pid(proc_t* proc){
	return proc->pid;
}
int proc_remaining_time(proc_t* proc){
	return proc->remaining_time;
}
int proc_memory_size(proc_t* proc){
	return proc->memory_size;
}
bool proc_in_memory(proc_t* proc){
	return proc->in_memory;
}

// decrement a process' remaining time (to simulate running the process)
void proc_step(proc_t* proc, int delta){
	proc->remaining_time -= delta;
}
// mark the process as in memory
void proc_swap_in(proc_t* proc){
	proc->in_memory = true;
}
// mark the process as no-longer in memory
void proc_swap_out(proc_t* proc){
	proc->in_memory = false;
}

// free a process
void proc_free(proc_t* proc){
	free(proc);
}