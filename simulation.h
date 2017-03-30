/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                            Simulation Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 10/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "scheduler.h" // for algo_t

// a job to be completed by the simulation
typedef struct job job_t;

// returns a new job to be run by the simulation
job_t* job_new(int job_id, int arrival_time, int burst_time, int memory_size);

// return job waiting time (or negative if job has not finished)
int job_waiting_time(job_t* job);
// return job turnaround time (or negative if job has not finished)
int job_turnaround_time(job_t* job);

// free a job after it has been run by the simulation
void job_free(job_t* job);

// run a scheduling simulation with a given scheduler type,
// and array of jobs to run, and a memory module maximum size
void simulate(algo_t scheduler, job_t** jobs, int memory, bool silence);