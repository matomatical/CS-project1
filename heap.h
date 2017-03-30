/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                             Min-heap Module                               * 
 *        (Appropriated from Heap used for COMP20007 Assignment 2015)        *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 18/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// heap_t type is a minimum binary heap of items
// sorted by key in binary minimum-heap order
typedef struct heap heap_t;

// returns a pointer to a new dynamically growing minheap
heap_t* heap_new();

// frees all memory associated with heap *h
void heap_free(heap_t* h);

// creates an item with its data and key 'key' and
// places it in the heap *h
void heap_insert(heap_t* h, void* data, int key);

// removes the root of the heap, maintaining minheap structure
// returns the data stored with that item, or NULL if emtpy
// note that NULL could also come out if data is NULL to begin with
void* heap_remove_min(heap_t* h);

// returns the oid of the root of the heap without altering heap, NULL if empty
// note that NULL could also come out if data is NULL to begin with
void* heap_peek_min(heap_t* h);

// returns 1 if heap is empty, 0 is heap is nonempty
int heap_is_empty(heap_t* h);
