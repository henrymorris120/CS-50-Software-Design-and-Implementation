/* 
 * game.h - header file for CS50 player module
 *
 * a *game* is a structure which holds all the information about the game
 * Henry Fall 2023
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "grid.h"
#include "../libcs50/mem.h"
#include "../support/message.h"

/**************** global types ****************/
typedef struct game {
  player_t** players;        // holds pointer to struct player in array
  addr_t specAddress;        // address of spectator
  grid_t* grid;              // grid of the game
  int goldColleted;          // amount gold collected by players
  int playerCount;           // number of players that have joined
  bool hasSpectator;         // whether or not someone is watching
} game_t;

/**************** functions ****************/

/**************** game_new() ****************/
/*
 * creates new struct game 
 * Caller provides:
 *  readabel map file and seed (0 if no seed was provided in the server)
 * We return:
 *  pointer to the initialed game
 * Caller is responsible for:
 *   later calling player_delete.
*/
game_t* game_new(char* mapFilename, int seed);


/**************** playerCountToID() ****************/
/*
 * input is an integer 0 through 25
 * output is corresponding capital alphabet character
*/
char playerCountToID(int n);


/**************** game_add_player() ****************/
/*
 * input the game struct, name, and address
 * creates a new player isuing arguments and adds it to array of 
 * player pointers updates playerCount
 * returns true of false depending on success of insertion
*/
bool game_add_player(game_t* game, char* name, addr_t addr); 


/**************** game_delete() ****************/
/*
 * deletes the game imputted, free all allocated memory space
*/
void game_delete(game_t* game);


/**************** findPlayerFromAddress() ****************/
/*
 * input is the game struct and and an address
 * output in the player with that address, or NULL if no player has it
*/
player_t* findPlayerFromAddress(game_t* game, addr_t addr); 


/**************** findPlayerFromID() ****************/
/*
 * input is the game struct and and a character ID
 * output in the player with that ID, or NULL if no player has it
*/
player_t* findPlayerFromID(game_t* game, char c); 


/**************** gameCollide() ****************/
/*
 * inputs are the game struct and x-y values
 * returns the player at that exact position, or NULL if a player is not
 * occupying that spot then
*/
player_t* gameCollide(game_t* game, int pos_x, int pos_y); 


/**************** game_player_move() ****************/
/*
 * inputs are the game, player, x, and y
 * returns boolean whether it is possible to move there, updating the values
 * if possible and calls other functions to update game other game variables
 * if find gold or another player
*/
bool game_player_move(game_t* game, player_t* player, int new_x, int new_y);


/**************** is_over() ****************/
/*
 * return boolean value if game should,
 * checking if all the gold has been found
*/
bool is_over(game_t* game); 


/**************** handlePlayerQuit() ****************/
/*
 * inputs are the game and player structs
 * sets in grid a new character in the postion that the player was
 * calls playerQuit function in player module
*/
void handlePlayerQuit(game_t* game, player_t* player);

/**************** steal() ****************/
/* 
 * have player 1 steal all the gold from the second player
*/
void steal(game_t* game, player_t* player1, player_t* player2); 

/**************** returnVisibilityMapWithLineBreaks() ****************/
/*
 * inputs are the game struct and player struct
 * iterate through the players visibility grid
 * and create grid map the shows points only if visibility true there
 * as well as points that have been seen (but dont show gold or players)
 * mark the player's individual positon with an "@"
 * account for line breaks as well when get to end of row
 * return a grid map which the user is responsible for freeing
*/
char* returnVisibilityMapWithLineBreaks(game_t* game, player_t* player); 

/**************** returnSpectatorVisibilityMapWithLineBreaks() ****************/
/*
 * returns the visual grid that spectator see with line breaks
*/
char* returnSpectatorVisibilityMapWithLineBreaks(game_t* game);

#endif // __PLAYER_H
