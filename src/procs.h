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

#pragma once // to compile on dimefox without complaints (#included everywhere!)

// a model process
typedef struct proc proc_t;

// returns a new process to queue
proc_t* proc_new(int jid, int pid, int remaining_time, int memory_size);

// getters
int proc_jid(proc_t* proc);
int proc_pid(proc_t* proc);
int proc_remaining_time(proc_t* proc);
int proc_memory_size(proc_t* proc);
bool proc_in_memory(proc_t* proc);

// work on a process for delta units of time
void proc_step(proc_t* proc, int delta);

// mark a process as 'in memory'
void proc_swap_in(proc_t* proc);
// mark a process as 'not in memory'
void proc_swap_out(proc_t* proc);

// free a process
void proc_free(proc_t* proc);