/* 
 * player.c - CS50 'game' module
 *
 * see player.h for more information.
 * Henry Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "grid.h"
#include "game.h"
#include "../libcs50/mem.h"
#include "../support/message.h"

/**************** file-local global variables ****************/
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game

/**************** global functions ****************/
/* that is, visible outside this file */
/* see game.h for comments about exported functions */

/**************** game_new() ****************/
/* see game.h for description */
game_t* game_new(char* mapFilename, int seed) 
{
    game_t* game = mem_malloc_assert(sizeof(game_t),"Memory is full");
    
    // initialize values in game
    game->players = mem_malloc_assert(MaxPlayers * sizeof(player_t*), "Memory is full");
    game->goldColleted = 0;
    game->grid = grid_new(mapFilename, seed);
    game->playerCount = 0;
    game->hasSpectator = false;
    return game;
}

/**************** playerCountToID() ****************/
/* see game.h for description */
char playerCountToID(int n) 
{
    if (n >= 0 && n <= 25) {
        return 'A' + n;
    } else {
        return '\0';
    }
}

/**************** game_add_player() ****************/
/* see game.h for description */
bool game_add_player(game_t* game, char* name, addr_t addr) 
{

    // check if havent reached players limit
    if (game->playerCount < MaxPlayers) {
        // get random start postion in empty spot of grid
        int pos = getRandomStartPosition(game->grid);

         

        int x = pos % game->grid->columns;
        int y = game->grid->rows - pos / game->grid->columns;
        char ID = playerCountToID(game->playerCount);
        player_t* addedPlayer = player_new(name, ID, addr, x, y, game->grid->gridSize);
        (game->players)[game->playerCount] = addedPlayer;
        game->playerCount++;
        game->grid->gridVisual[pos] = ID;
        gridUpdateVisibility(game->grid, addedPlayer);
        return true;
    }
    return false;
}

/**************** game_delete() ****************/
/* see game.h for description */
void game_delete(game_t* game)
{
    if (game != NULL) {
        for(int i=0; i < game->playerCount; i++) {
            player_delete(game->players[i]);
        }
        mem_free(game->players);
        grid_delete(game->grid);
        mem_free(game);
    }
}

/**************** findPlayerFromAddress() ****************/
/* see game.h for description */
player_t* findPlayerFromAddress(game_t* game, addr_t addr) 
{
    for(int i=0; i < game->playerCount; i++) {
        if (message_eqAddr(addr,game->players[i]->addr)) {
            return game->players[i];
        }
    }
    return NULL;
}

/**************** findPlayerFromID() ****************/
/* see game.h for description */
player_t* findPlayerFromID(game_t* game, char c) 
{
    for(int i=0; i < game->playerCount; i++) {
        if (c == game->players[i]->ID) {
            return game->players[i];
        }
    }
    return NULL;
}

/**************** gameCollide() ****************/
/* see game.h for description */
player_t* gameCollide(game_t* game, int pos_x, int pos_y) 
{
    for(int i=0; i < game->playerCount; i++) {
        if (game->players[i]->x == pos_x && game->players[i]->y == pos_y) {
            return game->players[i];
        }
    }
    return NULL;    
}

/**************** game_player_move() ****************/
/* see game.h for description */
bool game_player_move(game_t* game, player_t* player, int new_x, int new_y) 
{
    grid_t* grid = game->grid;
    // check if valid space to move to
    if (grid_can_move(grid, new_x, new_y, player->x, player->y)) {
        player_t* differentPlayer;
        int goldHere;
        // check if collide with another player in which case switch with them
        if ((differentPlayer = gameCollide(game, new_x, new_y)) != NULL) {
            differentPlayer->x = player->x;
            differentPlayer->y = player->y;
            gridUpdatePlayer(game->grid, differentPlayer);
            gridUpdateVisibility(game->grid,differentPlayer);
        // check if there is gold in new position    
        } else if ((goldHere = gridHasGold(game->grid,new_x, new_y)) != 0) {
            player->gold = player->gold + goldHere;
            game->goldColleted = game->goldColleted + goldHere;
        }
        // update postion
        player->x = new_x;
        player->y = new_y;
        gridUpdatePlayer(game->grid, player);
        gridUpdateVisibility(game->grid, player);
        return true;
    }
    return false;
}

/**************** is_over() ****************/
/* see game.h for description */
bool is_over(game_t* game) 
{
    // check if all gold in game gas been collected
    if (game->goldColleted == GoldTotal) {
        return true;
    } else {
        return false;
    }
}

/**************** handlePlayerQuit() ****************/
/* see game.h for description */
void handlePlayerQuit(game_t* game, player_t* player)
{
    int pos = (game->grid->rows - player->y) * game->grid->columns + player->x;
    game->grid->gridVisual[pos] =  game->grid->originalGrid[pos];
    player_quit(player);
}

/**************** steal() ****************/
/* see game.h for description */
void steal(game_t* game, player_t* player1, player_t* player2) 
{
    if (player1 != NULL && player2 != NULL) {
        player1->gold = player1->gold + player2->gold;
        player2->gold = 0;
    }
}

/**************** returnVisibilityMapWithLineBreaks() ****************/
/* see game.h for description */
char* returnVisibilityMapWithLineBreaks(game_t* game, player_t* player) 
{
    char* playerVisualMap = (char*)(mem_malloc_assert((game->grid->gridSize + game->grid->rows + 1) * sizeof(char), "Memory is full"));
    int jumps = 0;
    for (int i = 0; i < game->grid->gridSize; i++) {
        if (player->visibiityMap[i]) {
            // check if point is visible or just was seen
            if (checkVisible(game->grid, player->x, player->y, i)) {
                playerVisualMap[i+jumps] = game->grid->gridVisual[i];
            } else {
                playerVisualMap[i+jumps] = game->grid->originalGrid[i];
            }
        } else {
            playerVisualMap[i+jumps] = ' ';
        }
         // mark this players postion with an @
        if(((game->grid->rows - player->y) * game->grid->columns + player->x) == i) {
            playerVisualMap[i+jumps] = '@';
        }
        // do line breaks
        if((i+1) % game->grid->columns == 0) {
            jumps++;
            playerVisualMap[i + jumps] = '\n';
        }
    }
    playerVisualMap[game->grid->gridSize + game->grid->rows] = '\0';
    return playerVisualMap;
}

/**************** returnSpectatorVisibilityMapWithLineBreaks() ****************/
/* see game.h for description */
char* returnSpectatorVisibilityMapWithLineBreaks(game_t* game)
{
    char* visualMap = (char*)(mem_malloc_assert((game->grid->gridSize + game->grid->rows + 1) * sizeof(char), "Memory is full"));
    int jumps = 0;
    for (int i = 0; i < game->grid->gridSize; i++) {
        visualMap[i+jumps] = game->grid->gridVisual[i];
        // do line breaks
        if((i+1) % game->grid->columns == 0) {
            jumps++;
            visualMap[i + jumps] = '\n';
        }
    }
    visualMap[game->grid->gridSize + game->grid->rows] = '\0';
    return visualMap;
}
