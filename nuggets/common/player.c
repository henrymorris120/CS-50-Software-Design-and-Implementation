/* 
 * player.c - CS50 'player' module
 *
 * see player.h for more information.
 *
 * Team 12-Henry Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/mem.h"
#include "../support/message.h"
#include "player.h"

/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see player.h for comments about exported functions */

/**************** player_new() ****************/
/* see player.h for description */
player_t* player_new(const char* name, char ID, addr_t addr, int x, int y, int masterGridSize)
{
    // allocate space for new player
    player_t* player = mem_malloc_assert(sizeof(player_t),"Memory is full");
    
    
    // set values from arguments into player
    player->name= name;
    player->ID = ID;
    player->x = x;
    player->y = y;
    player->addr = addr;
    player->gold = 0;
    player->hasQuit = false;

    player->visibiityMap = (bool*)mem_malloc_assert(masterGridSize * sizeof(bool), "Memory is full");
    for (int i=0;i<masterGridSize;i++) {
        player->visibiityMap[i]= false;
    }
    
    return player;
}


/**************** player_delete() ****************/
/* see player.h for description */
void player_delete(player_t* player)
{
    // check if not null before freeing
    if (player-> visibiityMap != NULL) {
        mem_free(player->visibiityMap);
    }
    if (player != NULL) {
        mem_free(player);
    }
}

/**************** player_quit() ****************/
/* see player.h for description */
void player_quit(player_t* player) {
    if (player != NULL) {
        player->hasQuit = true;
        player->x = -1;
        player->y = -1;
    }
}
