#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#*               COMP30023 Computer Systems - Semester 1 2016                *
#*          Assignment 1 - Process Scheduling and Memory Management          *
#*                                                                           *
#*                                 Makefile                                  *
#*                                                                           *
#*                  Submission by: Matt Farrugia <farrugiam>                 *
#*                  Last Modified 08/04/16 by Matt Farrugia                  *
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

CC	= gcc
LDFLAGS  = -Wall
CFLAGS 	= -Wall -std=c99 -MMD

## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =	main.c list.c queue.c simulation.c memory.c procs.c scheduler.c multi.c fcfs.c memanager.c linux.c heap.c sjf.c
OBJ =	main.o list.o queue.o simulation.o memory.o procs.o scheduler.o multi.o fcfs.o memanager.o linux.o heap.o sjf.o
EXE = 	simulation

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(LDFLAGS) -o $(EXE) $(OBJ) -lm

test:
		sh test.sh

## Clean: Remove object files
clean:
		rm *.o *.d

## Clobber: Performs Clean and removes executable file.
clobber: clean
		rm $(EXE)

## Cleanly: make top level target, and then clean object files immediately
cleanly: $(EXE)
		rm *.o *.d

## Dependencies
-include $(DEP)