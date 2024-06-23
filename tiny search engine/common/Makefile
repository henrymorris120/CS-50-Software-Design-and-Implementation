# Makefile for common
#
# Henry Morris Fall 2023

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
OBJS = pagedir.o index.o word.o
LIB = common.a

# executable depends on object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

.PHONY: clean


# object files depend on include files
$(OBJS): $L/webpage.h $L/mem.h $L/counters.h $L/hashtable.h $L/file.h $L/hash.h

# cleaning up
clean:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf common
