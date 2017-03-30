/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               COMP30023 Computer Systems - Semester 1 2016                *
 *          Assignment 1 - Process Scheduling and Memory Management          *
 *                                                                           *
 *                           Linked List Module                              *
 *                                                                           *
 *                  Submission by: Matt Farrugia <farrugiam>                 *
 *                  Last Modified 07/04/16 by Matt Farrugia                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include "list.h"


// nodes make up a linked list
typedef struct node node_t;

struct node{
	node_t* next;
	void* data;
};


// a linked list is a collection of linked nodes
struct list{
	node_t* first;
	node_t* last;
	int length;
};


/* * * * * * * * * * * FUNCTION DEFINITIONS * * * * * * * * * * */

// returns a pointer to a fresh, empty linked list
list_t* list_new(){
	
	// declare and initialise the new list
	list_t* new_list = (list_t*)malloc(sizeof(list_t)*1);
	
	new_list->first = NULL;
	new_list->last = NULL;
	new_list->length = 0;

	return new_list;
}


// add a new node (holding data) to the end of a list
void list_append(list_t* list, void* data){
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	
	new_node->next = NULL;
	new_node->data = data;

	if(list->first==NULL){
		// this is the first piece of data for the list
		// grad the new node at the start
		list->first = new_node;
	} else {
		// there is already at least one node in the list
		// grab the new node at the end
		list->last->next = new_node;
	}
	//either way the new node is the new last node
	list->last =  new_node;

	list->length++;

	return;
}

// add a new node (holding data) to the start of a list
void list_prepend(list_t* list, void* data){
	node_t* new_node = (node_t*)malloc(sizeof(node_t));
	
	new_node->data = data;
	new_node->next = list->first; // may be null (all good)

	list->first = new_node;
	
	if(!list->last){
		list->last = new_node;
	}
	
	list->length++;

	return;
}

// return the ith element of a list, counting from the start element
// NULL if there is no ith element
void* list_get(list_t* list, int i){
	
	if(i<0 || i>=list->length){
		return NULL;
	}

	node_t* node;

	// skip to ith node
	for(int me = 0; me < i; me++){
		node = node->next;
	}

	return node->data;
}

// return the end element of a list
// or NULL if empty
void* list_last(list_t* list){
	return list->first;
}

// return the start element of a list
// or NULL if empty
void* list_first(list_t* list){
	return list->last;
}

// return AND REMOVE the start element of a list
// or NULL if empty
void* list_behead(list_t* list){
	if(!list->length){
		return NULL;
	}

	node_t* node = list->first;

	void* data = node->data;

	list->first = node->next;

	if(!node->next){
		list->last = NULL;
	}

	list->length--;
	free(node);

	return data;
}

// return the number of elements in a linked list
int list_length(list_t* list){
	return list->length;
}

// traverse a list performing a function on each element
void list_traverse(list_t* list, void (*f)(void*)){
	node_t* node = list->first;
	while(node){
		f(node->data);
		node = node->next;
	}
}

// free a list but not the data
void list_free(list_t* list){
	node_t* node = list->first;
	node_t* me;

	// carefully free each node when we have a hold on the next
	while(node){
		me = node;
		node = node->next;
		free(me);
	}

	free(list);
}