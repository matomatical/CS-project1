/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                            Simulation Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "simulation.h"
#include "memanager.h"

/* * * * * * * * * * * JOB HANDLING * * * * * * * * * * */

// a job is a recepie for a process to be scheduled
struct job{
	int job_id;
	int arrival_time;
	int burst_time;
	int memory_size;
	int finish_time;
};

// returns a new job to be run by the simulation
job_t* job_new(int job_id, int arrival_time, int burst_time, int memory_size){

	// allocate
	job_t* new_job = malloc(sizeof(job_t));

	// initialise
	new_job->job_id = job_id;
	new_job->arrival_time = arrival_time;
	new_job->burst_time = burst_time;
	new_job->memory_size = memory_size;
	new_job->finish_time = -1;
	
	// return
	return new_job;	
}

// return job waiting time (or negative if job has not finished)
int job_waiting_time(job_t* job){
	return job->finish_time - job->arrival_time - job->burst_time;
}

// return job turnaround time (or negative if job has not finished)
int job_turnaround_time(job_t* job){
	return job->finish_time - job->arrival_time;
}

// free a job after it has been run by the simulation
void job_free(job_t* job){
	free(job);
}

/* * * * * * * * * * * SIMULATION AND HELPER FUNCTIONS * * * * * * * * * * */

// core simulation loops
void simulate_loop(job_t** jobs, memanager_t* meman, scheduler_t* sched);

// helper function to receive new jobs from the job list
// and add them to the scheduler
void receive_jobs(job_t** jobs, int* j, int clock, scheduler_t* sched);

// helper function to print the status of the simulation
void print_status(proc_t* proc, memanager_t* meman, int clock);


bool print_things;

// initiate simulation
void simulate(algo_t scheduler, job_t** jobs, int memory, bool silence){

	print_things = !silence;

	memanager_t* meman = memanager_new(memory);
	scheduler_t* sched = scheduler_new(scheduler);

	simulate_loop(jobs, meman, sched);

	memanager_free(meman);
	scheduler_free(sched);
}

// core simulation loop
void simulate_loop(job_t** jobs, memanager_t* meman, scheduler_t* sched){

	int j = 0; 		// position in job list
	int clock = -1; // simulation timer
	int alarm = -1; // alarm for clock interrupts (dedicated to scheduling)

	proc_t* proc = NULL; // currently running process

	bool need_new_proc = false;

	while(proc || jobs[j] || !schedule_empty(sched)){
		
		// take care of timing
		clock++;
		if(alarm>0){ alarm--; }

		// add any incoming jobs to the queue
		receive_jobs(jobs, &j, clock, sched);

		// expire completed process
		if(proc && proc_remaining_time(proc) <= 0){
			// finish the process
			memanager_unload(meman, proc);
			jobs[proc_jid(proc)]->finish_time = clock;
			proc_free(proc);
			proc = NULL;

			// need another process
			need_new_proc = true;
		}

		// clock interrupt?
		if(alarm == 0){
			// need another process
			need_new_proc = true;
		}

		if(!proc || need_new_proc){
			
			// get next process from scheduler
			proc = schedule_next(sched, proc, &alarm);
			need_new_proc = false;

			if(proc){
				// swap into memory if not in
				if( ! proc_in_memory(proc)){
					memanager_load(meman, proc, clock);
				}

				// print a status message
				print_status(proc, meman, clock);
			}
		}

		if(proc){
			// we have a process this step!
			proc_step(proc, 1);
		}
	}

	if(print_things){
		printf("time %d, simulation finished.\n", clock);
	}
}

// helper function to receive new jobs from the job list
// and add them to the scheduler
void receive_jobs(job_t** jobs, int* j, int clock, scheduler_t* sched){
	
	proc_t* new_proc;

	int i;
	for(i = *j; jobs[i] && jobs[i]->arrival_time <= clock; i++){
		// jobs[i] is a new job arriving since the last step!

		// make a new proc and add it to the schedule
		new_proc = proc_new(i, jobs[i]->job_id, jobs[i]->burst_time,
			jobs[i]->memory_size);

		schedule_add(sched, new_proc);
	}

	*j = i;
}

// helper function to print the status of the simulation
void print_status(proc_t* proc, memanager_t* meman, int clock){
	if(print_things){
		printf("time %d, %d running, ", clock, proc_pid(proc));
		memanager_print_status(meman, stdout);
		printf("\n");
	}
}