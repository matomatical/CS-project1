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

#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define INITIAL_SIZE 16 // will dynamically grow

typedef struct item item_t;

struct heap{
        item_t* items; // array of items, stored as (key,data) pairs
        int nitems;    // current number of items in heap
        int titems;    // maximum number of items in heap
};

struct item{
        int key;    // value which the minheap is structured on
        void* data;
};

/* * * * * * * * * * * HELPER FUNCTION PROTOTYPES * * * * * * * * * * */

// compares item at heap *h index hid with its ancestors, swapping
// up until minheap structure is satisfied
static void siftup(heap_t *h, int hid);

// compares item at heap *h index hid with its children, swapping
// down until minheap structure is satisfied
static void siftdown(heap_t* h, int hid);

/* * * * * * * * * * * FUNCTION CODE * * * * * * * * * * */

// return a new heap
heap_t* heap_new(){

        heap_t* h = (heap_t*)malloc(sizeof(heap_t));

        h->titems=INITIAL_SIZE;
        h->items=(item_t*)malloc(h->titems*sizeof(item_t));
        
        h->nitems=0;

        return h;
}

void heap_free(heap_t* h){
        free(h->items);
        free(h);
        return;
}

void heap_insert(heap_t *h, void* data, int key){

        if (h->nitems>=h->titems){
            // no more space! time to grow
            h->titems *= 2;
            h->items = realloc(h->items, h->titems*sizeof(item_t));
        }

        // there is space in the heap, now

        // initialise the new item and increiment nitems
        h->items[h->nitems].key=key;
        h->items[h->nitems].data=data;

        h->nitems++;

        // maintain heap order (also fixes up hidof[oid])
        siftup(h,h->nitems-1);
}



// removes the root of the heap, maintaining minheap structure
// returns the data stored with that item, or NULL if emtpy
void* heap_remove_min(heap_t *h){

    if(h && h->nitems){
        // if there is a min, save its data
        void* min = h->items[0].data;

        // copy the last heap item into the root, and decriment nitems
        int last=h->nitems-1;
        h->items[0].key=h->items[last].key;
        h->items[0].data=h->items[last].data;
        h->nitems--;

        // sift this last item down to restore minheap structure
        siftdown(h,0);

        // finally, return the oid of the min element
        return min;
    } else {
        return NULL;
    }
        
}

// assuming there IS a min, return its data w/o touching heap order
void* heap_peek_min(heap_t *h){
    if(h && h->nitems){
        return h->items[0].data;    
    } else {
        return NULL;
    }
}

// returns 1 if heap is empty, 0 is heap is nonempty
int heap_is_empty(heap_t* h){
    // !(h->nitems) is 1 if h->nitems is 0
    // else it evaluates to 0, as required
    return !(h->nitems);
}


/* * * * * * * * * * * HELPER FUNCTION CODE * * * * * * * * * * */

// compares item at heap *h index hid with its ancestors, swapping
// up until minheap structure is satisfied
static void siftup(heap_t *h, int hid){

        // copy the item we're sifting up (me)
        item_t me = {.key = h->items[hid].key, .data = h->items[hid].data, };

        // calculate the hid of the parent
        int parent = (hid-1)/2;

        // while the parent exists and is larger than me,
        while(hid > 0 && h->items[parent].key > me.key){

                // cascade parent down in items and hidof
                h->items[hid].key  = h->items[parent].key;
                h->items[hid].data = h->items[parent].data;

                // update hid and parent, then loop again
                hid=parent;
                parent=(hid-1)/2;
        }

        // paste the item we're sifting up into the position we reached
        h->items[hid].key  = me.key;
        h->items[hid].data = me.data;

        return;
}

// compares item at heap *h index hid with its children, swapping
// down until minheap structure is satisfied
static void siftdown(heap_t *h, int hid){

    // copy the item we're sifting down
    item_t me;
    me.key=h->items[hid].key;
    me.data=h->items[hid].data;

    // calculate the position of the first child
    int child = 2*hid+1;

    // while there are two children, and the smaller is smaller than me,
    while(child+1 < h->nitems && me.key > h->items[child+=
                    (h->items[child].key > h->items[child+1].key)].key){
    // !!~NOTE~!! guard sets child to hid of the min child

            //cascade smaller child up one into spot
            h->items[hid].key=h->items[child].key;
            h->items[hid].data=h->items[child].data;

            //update hid and child, then loop again
            hid=child;
            child=2*hid+1;
    }

    // if there's a leftover child and it's smaller than me,
    if(child < h->nitems && me.key > h->items[child].key){
            //cascade child up one spot
            h->items[hid].key  = h->items[child].key;
            h->items[hid].data = h->items[child].data;
    }

    // finally, paste item we're sifting down into position reached
    h->items[hid].key  = me.key;
    h->items[hid].data = me.data;

    return;
}