/*
 * test.c - testing if the modules work correctly, notably if visibilty is
 * implemented correctly
 * 
 * CS50 Nuggets Final Project
 * Team 12 - Henry
 */


#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/mem.h"
#include "../support/message.h"
#include "player.h"
#include "grid.h"
#include "game.h"


// main function with no arguments
// purpose it just to test if the visibility graphs are done correctly
int main() 
{
    char* mapfile = "../../shared/students_files/nuggets/linux/maps/main.txt";
    game_t* newGame = game_new(mapfile,0);
    char* name1 = "Henry";
    char* name2 = "AnotherName";
    char* name3 = "ThirdName";
    addr_t address1;
    addr_t address2;
    addr_t address3;
    // add 2 players and update visibility
    game_add_player(newGame, name1, address1);
    game_add_player(newGame, name2, address2);
    game_add_player(newGame, name3, address3);
    char* main_grid = returnSpectatorVisibilityMapWithLineBreaks(newGame);
    char* player1grid = returnVisibilityMapWithLineBreaks(newGame, newGame->players[0]);
    char* player2grid = returnVisibilityMapWithLineBreaks(newGame, newGame->players[1]);
    char* player3grid = returnVisibilityMapWithLineBreaks(newGame, newGame->players[2]);
    // print the 4 maps, one with a spectator view
    // the other with view of player A
    // the next with view of player B
    // the last with view from player C
    printf("%s",main_grid);
    printf("next map, personal only to player A \n");
    printf("%s", player1grid);
    printf("next map, personal only to player B \n");
    printf("%s", player2grid);
    printf("next map, personal only to player c \n");
    printf("%s", player3grid);
    // free all allocated memory
    mem_free(main_grid);
    mem_free(player1grid);
    mem_free(player2grid);
    mem_free(player3grid);
    game_delete(newGame);
}
