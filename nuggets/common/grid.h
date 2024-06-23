/* 
 * grid.c - CS50 'game' module
 *
 * see grid.h for more information.
 *
 * Henry Fall 2023
 */

#ifndef __GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"
#include "../libcs50/counters.h"

/**************** global types ****************/
typedef struct grid {
  int gridSize;           // number of total points on grid
  int columns;            // numer of columns
  char* originalGrid;     // array that maps the nugget game before players and gold added
  int rows;               // number or rows
  char* gridVisual;       // array of characters correspond to their  grid positon
  int seed;               // seed for randomness
  counters_t* goldAmt;    // tracks the amount of gold in each pile
} grid_t;

/**************** functions ****************/

/**************** grid_new() ****************/
/*
 * inputs are a string which is the pathname to a readable map
 * and integer seed, which is zero if wish use getpid(), when determining
 * random behavior in game
 * creates and returns a new grid_t which has just been initialized according to
 * REQUIREMENTS.md
 */ 
grid_t* grid_new(char* mapFilename, int seed); 


/**************** createGoldPile() ****************/
/*
 * Randomly, using seed, places gold in the map following REQUIREMENTS.md
 * returns a counters struct type where key is the index location
 * in gridVisual of a gold pile and item is the amount of gold stored there
 */
counters_t* createGoldPiles(char* gridVisual, int size, int seed);

/**************** grid_delete() ****************/
/*
 * input is grid_t*
 * frees all allocated memory
 */
void grid_delete(grid_t* grid);

/**************** grid_can_move() ****************/
/*
 * checks if the point in grid(x,y) is movable to
 * if so, reverts based on original grid the character in the (oldx, oldy)
 * returns boolean value if space is movable to
 */
bool grid_can_move(grid_t* grid, int x, int y, int oldx, int oldy);

/**************** gridHasGold() ****************/
/*
 * inputs are grid struct and x,y 
 Checks if gold has been stored in point (x,y) of the map
 * if so, returns the amount of gold that has been stored there
 * else return zero
 */
int gridHasGold(grid_t* grid, int x, int y); 

/**************** gridUpdateVisibility() ****************/
/*
 * inputs are grid and player structure
 * using players x and y, assign boolean values for each grid point's visibility
 * only updates if a point has not yet been seen
 * this boolean grid is stored in the player structure
 * see REQUIREMENTS.md for more information about how visibility is determined 
 */
void gridUpdateVisibility(grid_t* grid, player_t* player);

/**************** gridHelper() ****************/
/*
 * helper function in checkVisible() that determines if a point can be seen, 
 * assuming we are looking at a target point that is not the same vertical or
 * horizontal line as the origin point
 * returns a boolean if a target point can be seen, employs recursion to solve
 */
bool gridHelper(grid_t* grid, int change_x, int change_y, int target_x, int target_y, int origin_x, int origin_y, int target, bool up, bool right);

/**************** checkVisible() ****************/
/*
 * returns boolean corresponding to if in gridArray a player at the origin (x,y)
 * can see the grid point target
 */
bool checkVisible(grid_t* grid, int origin_x, int origin_y, int target); 

/**************** checkVisibilityType() ****************/
/*
 * returns boolean corresponding to if in the original grid a
 * point corresponding to (x,y) is an empty spot or not
 */
bool checkVisibiltyType(int x, int y, grid_t* grid);

/**************** gridUpdatePlayer() ****************/
/*
 * inputs are grid and player structure
 * marks the location of the player in gridVisual which is part of the grid struct
 * the ID of that player
 */
void gridUpdatePlayer(grid_t* grid, player_t* player);

/**************** getRandomStartPosition() ****************/
/*
 * returns randomly an index in gridVisual from the inputted grid struct that is empty
 */
int getRandomStartPosition(grid_t* grid);

#endif // __GRID_H
