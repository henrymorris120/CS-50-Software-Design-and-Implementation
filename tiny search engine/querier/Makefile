# Makefile for querier
#
# Henry Morris Fall 2023

L = ../libcs50
C = ../common
LIBS = -lm
LLIBS = $C/common.a $L/libcs50-given.a
PROG = querier
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc

MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: $(PROG) test valgrind clean

# executable depends on object files
querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# object files depend on include files
querier.o: $L/counters.h $L/mem.h $L/file.h $C/index.h $C/word.h $C/pagedir.h 

test: querier testing.sh
	bash -v testing.sh &> testing.out

valgrind: $(PROG)
	$(VALGRIND) ./testing.sh

# cleaning up
clean:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf $(PROG)
