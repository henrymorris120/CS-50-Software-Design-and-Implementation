/* 
 * player.h - header file for CS50 player module
 *
 * a *player* is a structure in the nuggets game which holds all the
 * relavent informaition about the player in the game
 *
 * Team 12-Henry Fall 2023
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../support/message.h"

/**************** global types ****************/
typedef struct player {
  const char* name;     // player name    
  char ID;              // ID character for player 
  int x;                // x position in grid
  int y;                // y postion in grid
  addr_t addr;          // address
  int gold;             // amount of gold collected
  bool hasQuit;         // indicated if still in game
  bool* visibiityMap;   // pointers to true/false values based on visibility
} player_t;


/**************** functions ****************/

/**************** player_new() ****************/
/* create new player
 *
 * Caller provides:
 *  name, ID, address, x-y position, and size of main grid
 *  assumes inputs are all valid
 * We return:
 *   pointer to the new player;
 * Caller is responsible for:
 *   later calling player_delete.
 */
player_t* player_new(const char* name, char ID, addr_t addr, int x, int y, int masterGridSize);

/**************** player_delete() ****************/
/* frees in memory visibilty map and player if not null */
void player_delete(player_t* player);

/**************** player_quit() ****************/
/* sets hasQuit to true */
void player_quit(player_t* player);

#endif // __PLAYER_H
