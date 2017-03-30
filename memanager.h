/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           Memory Management Module                        *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 10/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>
#include "procs.h"

// a memory manager
typedef struct memanager memanager_t;

// returns a pointer to a new, fresh memory manager with a memory unit of a
// specified size
memanager_t* memanager_new(int size);

// free a memory manager
void memanager_free(memanager_t* meman);

// ask a memory manager to load a particular process at a certain time
void memanager_load(memanager_t* meman, proc_t* proc, int clock);
// ask a memory manager to unload one instance of a particular process
void memanager_unload(memanager_t* meman, proc_t* proc);

// ask a memory manager to prints its current status
void memanager_print_status(memanager_t* meman, FILE* out);