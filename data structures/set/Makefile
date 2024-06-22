# Makefile for 'set' module
#
# Henry Morris Fall 2023

OBJS = settest.o set.o ../lib/file.o ../lib/mem.o 
LIBS =

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

settest: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

settest.o: set.h ../lib/file.h
set.o: set.h
../lib/file.o: ../lib/file.h

.PHONY: test valgrind clean


test: settest
	./settest

valgrind: settest
	$(VALGRIND) ./settest

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f settest
	rm -f core
