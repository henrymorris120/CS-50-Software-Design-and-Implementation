# Makefile for 'Nuggets'
#
# Team 12, Sookyoung Park

S = support
L = libcs50
C = common
OBJS = 
LIBS =
LLIBS = $S/support.a $C/common.a $L/libcs50.a -lm

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S -I$C
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all:
	make -C support
	make -C libcs50
	make -C common
	make client
	make server


client: client.o $(LLIBS)
			$(CC) $(CFLAGS) $^ $(LIBS) -lcurses -o $@ 

server: server.o $(LLIBS)
			$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

client.o: $S/message.h

server.o : $C/player.h $C/game.h $L/file.h $L/mem.h

test: 

valgrind: 
	$(VALGRIND) ./server maps/visdemo.txt 5

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f client
	# rm -f server
	make -C support clean
	make -C libcs50 clean
	make -C common clean
