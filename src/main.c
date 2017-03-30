/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                          Simulation Driver Module                         *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

#include "simulation.h"

/* * * * * * * * * * * HELPER FUNCTION PROTOTYPES * * * * * * * * * * */

// holds configuration options
typedef struct {
	char* filename;
	algo_t schedule;
	int memory;
	bool stats, silence;
} opts_t;
// parses these inputs from command line arguments
opts_t load_options(int argc, char** argv);

// returns a null-terminated array of jobs read in from a file
job_t** load_jobs(char* filename);

// traverse a null-terminated array of jobs, printing statistics
void inspect_jobs(job_t** jobs);

// frees a null-terminated array of jobs
void unload_jobs(job_t** jobs);


/* * * * * * * * * * * MAIN FUNCTION * * * * * * * * * * */

int main(int argc, char** argv){

	// handle command line input
	opts_t options = load_options(argc, argv);

	// parse input from file into array of jobs
	job_t** jobs = load_jobs(options.filename);

	// run simulation of chosen algorithm
	simulate(options.schedule, jobs, options.memory, options.silence);

	// possibly print extra information
	if(options.stats){
		inspect_jobs(jobs);
	}

	// free array of jobs
	unload_jobs(jobs);

	// finished!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * HELPER FUNCTION CODE * * * * * * * * * * */

// parses command line inputs into options structure
opts_t load_options(int argc, char** argv){

	// set defaults
	opts_t options = {	.filename = NULL,	
						.schedule = NONE,
						.memory = 0,		
						.stats = false,
						.silence = false,	};

	// use getopt to scan inputs
	char input;
	while ((input = getopt(argc, argv, "f:m:a:sS")) != EOF){
		switch (input){
			case 'f':
				// set filename
				options.filename = optarg;
				break;

			case 'a':
				// set scheduling algorithm
				options.schedule = schedule_algo(optarg);
				break;

			case 'm':
				// set memory
				options.memory = atoi(optarg);
				break;
			case 's':
				options.stats = true;
				break;
			case 'S':
				options.silence = true;
				break;
			default:
				break;
		}
	}

	// validation and printing error / usage messages
	bool valid = true;
	// check filename validity
	if(!options.filename || access(options.filename, R_OK) == -1){
		fprintf(stderr,
			"please provide a valid input filename via the -f flag\n");
		valid = false;
	}
	// check scheduling algorithm validity
	if(options.schedule == NONE){
		fprintf(stderr,
			"please provide a scheduling algorithm via the -a flag:\n");
		fprintf(stderr, " - 'multi' for multilevel feedback queue\n");
		fprintf(stderr, " - 'fcfs'  for first-come, first-serve\n");
		fprintf(stderr,
			 " - 'more' for multilevel feedback queue with more layers (10)\n");
		// fprintf(stderr, " - 'cfs'  for completely fair scheduling\n");
		fprintf(stderr, " - 'linux' for O(1) scheduling algorithm\n");
		fprintf(stderr, " - 'sjf'  for shortest job first\n");
		valid = false;
	}
	// check memory size validity
	if(options.memory < 1){
		fprintf(stderr,
			"please provide a positive integer memory size via the -m flag\n");
		valid = false;
	}

	if(!valid){
		exit(EXIT_FAILURE);
	}

	return options;
}


// returns a null-terminated array of jobs read in from a file
job_t** load_jobs(char* filename){

	// open the input file specified
	FILE* fp;
	if(!(fp = fopen(filename, "r"))){
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	// make an array, starting with size 1 (+1 for null termination)
	int njobs = 0; // num jobs
	int mjobs = 1; // max jobs
	job_t** jobs = (job_t**)malloc(sizeof(job_t*)*(mjobs+1));

	// loop through file storing each job in the array
	int a, b, c, d;
	while(4==fscanf(fp, "%d %d %d %d\n", &a, &b, &c, &d)){
		
		// make sure there is enough space in the array
		if(njobs>=mjobs){
			mjobs *= 2;
			jobs = realloc(jobs, sizeof(job_t*)*(mjobs+1));
		}

		// add this job to array
		jobs[njobs++] = job_new(b, a, d, c);
	}

	// close file
	fclose(fp);

	// use the last space in the array for NULL termination
	// (NOTE there will be space as we always alloc one more than we need :) )
	jobs[njobs] = NULL;

	// and return the finished array !
	return jobs;
}

// traverse a null-terminated array of jobs, printing statistics
void inspect_jobs(job_t** jobs){
	
	long waiting_time = 0, turnaround_time = 0;
	int n;
	for(n = 0; jobs[n]; n++){
		waiting_time += job_waiting_time(jobs[n]);
		turnaround_time += job_turnaround_time(jobs[n]);
	}
	fprintf(stdout, "%d jobs completed:\n", n);
	fprintf(stdout, " average waiting time:    %4.2f\n", 1.0*waiting_time/n);
	fprintf(stdout, " average turnaround time: %4.2f\n", 1.0*turnaround_time/n);
}

// frees a null-terminated array of jobs
void unload_jobs(job_t** jobs){
	
	// free individual jobs
	for(int i = 0; jobs[i]; i++){
		job_free(jobs[i]);
	}
	// free array itself
	free(jobs);
}
