/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           Linked List Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 07/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// a linked list
typedef struct list list_t;

// returns a pointer to a fresh, empty linked list
list_t* list_new();


// add a new node (holding data) to the end of a list
void  list_append(list_t* list, void* data);

// add a new node (holding data) to the start of a list
void  list_prepend(list_t* list, void* data);


// return the ith element of a list, counting from the start element
// NULL if there is no ith element
void* list_get(list_t* list, int i);

// return the end element of a list
// or NULL if empty
void* list_last(list_t* list);

// return the start element of a list
// or NULL if empty
void* list_first(list_t* list);

// return AND REMOVE the start element of a list
// or NULL if empty
void* list_behead(list_t* list);


// return the number of elements in a linked list
int list_length(list_t* list);

// traverse a list performing a function on each element
void  list_traverse(list_t* list, void (*f)(void*));

// free a list
// it's your responsibility to free the data
void  list_free(list_t* list);