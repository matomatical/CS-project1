/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           FIFO Queue Module                               *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 07/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// a FIFO queue
typedef struct queue queue_t;

// returns a pointer to an empty queue
queue_t* queue_new();

// insert some data into a queue
void queue_enqueue(queue_t* q, void* data);

// remove some data from a queue (in FIFO order)
void* queue_dequeue(queue_t* q);

// return (but do not remove) the next item in FIFO order
void* queue_peek(queue_t* q);


// return the number of items in a queue
int queue_length(queue_t* q);

// free a queue
// it's your responsibility to free the data
void queue_free(queue_t* q);