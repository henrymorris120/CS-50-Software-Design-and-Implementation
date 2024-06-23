# Makefile for crawler
#
# Henry Morris Fall 2023

L = ../libcs50
C = ../common

PROG = crawler
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
OBJS = crawler.o
LIBS = -lm
LLIBS = $L/libcs50-given.a $C/common.a
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: $(PROG) test valgrind clean

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

# object files depend on include files
crawler.o: $L/bag.h $L/webpage.h $L/mem.h $L/hashtable.h $C/pagedir.h

test: crawler testing.sh
	bash -v testing.sh &> testing.out

valgrind: $(PROG)
	$(VALGRIND) ./testing.sh

# cleaning up
clean:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf $(PROG)
