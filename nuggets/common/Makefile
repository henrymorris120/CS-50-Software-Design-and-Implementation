# Makefile for modules
#
# Henry Morris Fall 2023

L = ../libcs50
S = ../support

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
OBJS = game.o grid.o player.o
LIB = common.a

# executable depends on object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

.PHONY: clean

# object files depend on include files
$(OBJS): $L/mem.h $L/counters.h $L/file.h $S/message.h $S/log.h

# Compile log module
$(S)/log.o: $(S)/log.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: test.c $(LIB) $(S)/log.o
	$(CC) $(CFLAGS) -o test test.c  $(LIB) $(S)/message.o $(S)/log.o -L$S -L$L -lcs50 -lm
	./test > test.out 2>&1

valgrind: test
	$(VALGRIND) ./test > test.out 2>&1

# cleaning up
clean:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf common.a
