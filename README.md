# COMP30023 Computer Systems 2016 --- Project 1

From the README submitted with my 2016 Computer Systems project (see specification.pdf for the task):

> Hi there!

> My submission for this project is rather expansive! I've tried to keep things
modular and extensible, and the result was a few more `.c` files than expected

> I hope you'll also enjoy my report; since I made scheduling algorithms so
flexible I decided to experiment with a few different algorithms to see how
they handled different processor loads in comparison to one another.

> Here's a breif overview of the components of my simulation.

> Matt (`farrugiam`) 694719

## core components

`main.c`

main function which reads command line inputs and starts the simulation

#### `simulation.c`

contains the actual simulation loop logic, working through a list 
of jobs and simulating their execution as processes on an imaginary CPU

#### memanager.c

memory manager, wrapping a memory storage module with behaviours such
as first-fit insertion and oldest-largest deletion

uses: memory.c to store memory blocks in a chain of fixed total length

#### scheduler.c

(polymorphic) process scheduler, wrapping the correct functionality in
a uniform set of functions

uses: fcfs.c OR multi.c OR linux.c OR sjf.c (depending on which algorithm type is specified at creation)


#### procs.c

(pronounced 'pross' 'ess' 'dot see') process structs which are used
pretty much everywhere

## scheduling algorithms

#### fcfs.c

first-come, first-serve scheduling functions

uses: queue.c to store runqueue

#### multi.c

multi-level feedback queue scheduling functions

uses: queue.c to store runqueues

#### linux.c

simplified old linux kernel O(1) scheduling functions

uses: queue.c to store runqueues

#### sjf.c

shortest job first scheduling functions

uses: heap.c to store runqueues sorted by remaining time

note: could add preemption to these to create shortest-remaining-time scheduler

## data structures

#### memory.c

doubly linked list of memory blocks that can store process, also supports iteration

#### queue.c

polymorphic FIFO queue

uses: list.c

#### list.c

polymorphic singly linked list

#### heap.c

polymorphic binary minheap appropriated from my old COMP20007 assignment (I know a lot more about C now, please don't mark this code haha)