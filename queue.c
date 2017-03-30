/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           FIFO Queue Module                               *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 07/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "queue.h"
#include "list.h"

// a FIFO queue, which is just a linked list
struct queue{
	list_t* list;
};

/* * * * * * * * * * * FUNCTION DEFINITIONS * * * * * * * * * * */

// returns a pointer to an empty queue
queue_t* queue_new(){
	queue_t* q = malloc(sizeof(queue_t));

	q->list = list_new();
	
	return q;
}

// insert some data into a queue
void queue_enqueue(queue_t* q, void* data){
	list_append(q->list, data);
}

// remove some data from a queue (in FIFO order)
void* queue_dequeue(queue_t* q){
	void* data = list_behead(q->list);
	return data;
}

// return (but do not remove) the next item in FIFO order
void* queue_peek(queue_t* q){
	void* data = list_first(q->list);
	return data;
}

// return the number of items in a queue
int queue_length(queue_t* q){
	return list_length(q->list);
}

// free a queue (but not the data)
void queue_free(queue_t* q){
	list_free(q->list);
	free(q);
}