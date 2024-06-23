# Makefile for indexer and indextest
#   
# Henry Morris

LIBS = -lm
LLIBS = ../common/common.a ../libcs50/libcs50-given.a

PROG = indexer
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: object files depend on header files
indexer.o: ../libcs50/webpage.h ../common/pagedir.h ../libcs50/mem.h ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/set.h ../libcs50/file.h

indextest.o: ../libcs50/mem.h ../common/index.h ../libcs50/counters.h

.PHONY: all clean

# run testing script
test: indexer testing.sh
	bash -v testing.sh &> testing.out


# clean up after our compilation
clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f indexer
	rm -f indextest
