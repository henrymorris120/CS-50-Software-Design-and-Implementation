/* 
 * grid.c - CS50 'game' module
 *
 * see grid.h for more information.
 *
 * Henry Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include "player.h"
#include "grid.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"

/**************** file-local global variables ****************/
static const int MinimumGoldPiles = 10;      // maximum number of gold piles
static const int MaximumGoldPiles = 30;      // minimum number of gold piles
static const int GoldTotal = 250;            // amount of gold in the game


/**************** global functions ****************/
/* that is, visible outside this file */
/* see grid.h for comments about exported functions */

/**************** grid_new() ****************/
/* see grid.h for description */
grid_t* grid_new(char* mapFilename, int seed) 
{
    grid_t* grid = mem_malloc_assert(sizeof(grid_t),"Memory is full");
    FILE* mapFile;
    // checked before function is called that readable and we assume as told in
    // REQUIREMENTS.md that file is in map file format
    grid->seed = seed;
    if ((mapFile = fopen(mapFilename, "r")) != NULL) {
      // use length of line in terms of characters and number of line to determine grid siz
      grid->rows = file_numLines(mapFile);
      char* line = file_readLine(mapFile);
      grid->columns = strlen(line);
      grid->gridSize = grid->columns * grid->rows;
      grid->gridVisual = (char*)(mem_malloc_assert(grid->gridSize * sizeof(char), "Memory is full"));
      grid->originalGrid = (char*)(mem_malloc_assert(grid->gridSize * sizeof(char), "Memory is full"));
      // read each line in mapFile and copy each character into the 1D grid arrays
      for (int i = 0; i < grid->columns; i++) {
        grid->gridVisual[i] = line[i]; 
        grid->originalGrid[i] = line[i];  
      }
      mem_free(line);
      for (int i = 1; i < grid->rows; i++) {
        char* nextLine = file_readLine(mapFile);
        for (int j = i * grid->columns; j < (i+1) * grid->columns; j++) {
          int k = j - (i * grid->columns);
          grid->gridVisual[j] = nextLine[k];
          grid->originalGrid[j] = nextLine[k];
        }
        mem_free(nextLine);
      }
      fclose(mapFile);
      // randomly create the gold piles counters struct (random how many piles and how
      // much gold in each)
      grid->goldAmt = createGoldPiles(grid->gridVisual, grid->gridSize, seed);
    }

    return grid;
}

/**************** createGoldPile() ****************/
/* see grid.h for description */
counters_t* createGoldPiles(char* gridVisual,int size, int seed) 
{
    // use seed to randomly determine pile number
    if (seed == 0) {
      srand(getpid());
    } else {
      srand(seed);
    }

    int piles = rand() % (MaximumGoldPiles - MinimumGoldPiles + 1) + MinimumGoldPiles;
    counters_t* goldPiles = counters_new();
    int pilesPicked = 0;
    int chosenPiles[piles];
    // continue in loop until chosen valid grid points for the randomly
    // decided above needed piles
    while (pilesPicked < piles) {
      // pick to look at random point in grid
      int idx = rand() % size;
      if ('.' == gridVisual[idx]) {
        // change character to indicate gold pile
        gridVisual[idx] = '*';
        // ensures that no pile is empty
        counters_add(goldPiles,idx);
        chosenPiles[pilesPicked] = idx;
        pilesPicked++;
      }
    }
    // now that all piles have 1 gold nugget, randomly allocate the remaining gold
    for (int i = 0; i < GoldTotal - piles; i++) {
      int pos = rand() % piles;
      counters_add(goldPiles,chosenPiles[pos]);
    }
    return goldPiles;
}

/**************** grid_delete() ****************/
/* see grid.h for description */
void grid_delete(grid_t* grid)
{
  counters_delete(grid->goldAmt);
  mem_free(grid->gridVisual);
  mem_free(grid->originalGrid);
  mem_free(grid);
}

/**************** grid_can_move() ****************/
/* see grid.h for description */
bool grid_can_move(grid_t* grid, int x, int y, int oldx, int oldy) 
{
  char c = grid->originalGrid[(grid->rows - y) * grid->columns + x];
  // check if player wants to move to a hallway, collide with another player, or empty spot
  if (c == '.' || c == '#') {
    // update grid because player will be moving
    grid->gridVisual[(grid->rows - oldy) * grid->columns + oldx] = grid->originalGrid[(grid->rows - oldy) * grid->columns + oldx];
    return true;
  }
  return false;
}

/**************** gridHasGold() ****************/
/* see grid.h for description */
int gridHasGold(grid_t* grid, int x, int y) 
{
  // check if point is a gold pile and if so look the counters goldAmt
  if (grid->gridVisual[(grid->rows - y) * grid->columns + x] == '*') {
    int nuggets = counters_get(grid->goldAmt, (grid->rows - y) * grid->columns + x);
    return nuggets;
  }
  return 0;
}

/**************** gridUpdateVisibility() ****************/
/* see grid.h for description */
void gridUpdateVisibility(grid_t* grid, player_t* player) 
{
  for (int i = 0; i < grid->gridSize; i++) {
    // check if point has still not been seen
    if (player->visibiityMap[i] == false) {
        player->visibiityMap[i]= checkVisible(grid, player->x, player->y, i);
    }
  }  
}

/**************** gridHelper() ****************/
/* see grid.h for description */
bool gridHelper(grid_t* grid, int change_x, int change_y, int target_x, int target_y, int origin_x, int origin_y, int target, bool up, bool right)
{
  // from direction booleans
  int change_vert;
  int change_across;
  if (up) {
    change_vert = 1;
  } else {
    change_vert = -1;
  }
  if (right) {
    change_across = 1;
  } else {
    change_across = -1;
  }
  // get vertical and horizontal slope both as floats and rounded when casted as ints
  float fhorizontal_movement_per_up = (float)change_x / change_y;
  int ihorizontal_movement_per_up = change_x / change_y;
  float fvertical_movement_per_across = (float)change_y / change_x;
  int ivertical_movement_per_across = change_y / change_x;
  bool upInt;
  bool acrossInt;
  // check if the float and int version are the same, assigning boolean values
  if (fabs(fhorizontal_movement_per_up - (float)ihorizontal_movement_per_up)< 1e-6) {
    acrossInt = true;
  } else {
    acrossInt = false;
  }
  if (fabs(fvertical_movement_per_across - (float)ivertical_movement_per_across)< 1e-6) {
    upInt = true;
  } else {
    upInt = false;
  }
  // check if float and int version are the same
  if (upInt && acrossInt) {
    // check if target is the same as the one step away grid point
    if (target_x == origin_x + change_vert*fhorizontal_movement_per_up && target_y == origin_y + change_across*fvertical_movement_per_across) {
      return true;
    }
    // return false if the grid point we have to go through is not visible through
    if(!checkVisibiltyType(origin_x + change_vert*fhorizontal_movement_per_up, origin_y + change_across*fvertical_movement_per_across,grid)) {
      return false;
    }
    // use recursion, starting at this new point as the origin
    return checkVisible(grid,origin_x + change_vert*fhorizontal_movement_per_up, origin_y + change_across*fvertical_movement_per_across,target);
  } else {
    // find the next grid point the line from target to origin goes through exactly and use recursion like above
    // return false if on the way to the next grid this line goes through exactly, we go through non-visible section
    
    // check if visible horizontally
    float fhorizontal_movement = change_vert * fhorizontal_movement_per_up;;
    int ihorizontal_movement = (int)fhorizontal_movement;

    int step =1;
    while (!acrossInt) {       
      if (!checkVisibiltyType(origin_x + ihorizontal_movement, origin_y + step*change_vert,grid) && !checkVisibiltyType(origin_x + ihorizontal_movement + change_across, origin_y + step*change_vert,grid)) {
        return false;
      }
      fhorizontal_movement = fhorizontal_movement + change_vert * fhorizontal_movement_per_up;
      ihorizontal_movement = (int)fhorizontal_movement;
      if (fabs(fhorizontal_movement - roundf(fhorizontal_movement))< 1e-4) {
        acrossInt = true;
        ihorizontal_movement = (int)roundf(fhorizontal_movement);
      }
      step++;
    }
    // check if visible vertically
    float fvertical_movement = change_across * fvertical_movement_per_across;
    int ivertical_movement = (int)fvertical_movement;
    step = 1;
    while (!upInt) {  
   
      if (!checkVisibiltyType(origin_x + step*change_across, origin_y + ivertical_movement,grid) && !checkVisibiltyType(origin_x + step*change_across, origin_y + ivertical_movement + change_vert,grid)) {
        return false;
      }
      fvertical_movement = fvertical_movement + change_across * fvertical_movement_per_across;
      ivertical_movement = (int)fvertical_movement;
      if (fabs(fvertical_movement - roundf(fvertical_movement))< 1e-4) {
        upInt = true;
        ivertical_movement = (int)roundf(fvertical_movement);
      }
      step++;
    }
    if (target_x == origin_x + ihorizontal_movement && target_y == origin_y + ivertical_movement) {
      return true;
    }
    if(!checkVisibiltyType(origin_x + ihorizontal_movement, origin_y + ivertical_movement,grid)) {
      return false;
    }
    return checkVisible(grid,origin_x + ihorizontal_movement,origin_y +  ivertical_movement,target);
  }
}



/**************** checkVisible() ****************/
/* see grid.h for description */
bool checkVisible(grid_t* grid, int origin_x, int origin_y, int target) 
{
  int target_x = target % grid->columns;
  int target_y = grid->rows - target / grid->columns;
  int change_x = target_x - origin_x;
  int change_y = target_y - origin_y;
  // note about x,y coordinates for visualization have been transformed so looks the first 
  // quadrand in cartesian graph
  if (change_x == 0) {
    // target is same point
    if (change_y == 0) {
      
    } else if (change_y > 0) {  // target is above
      // check if all above points before target are visible through
      for (int i = origin_y + 1; i < target_y; i++) {
         if (!checkVisibiltyType(origin_x, i, grid)) {
          return false;
         }
      }
    } else {                    // target is below
      // check if all below points before target are visible through
      for (int i = origin_y - 1; i > target_y; i--) {
         if (!checkVisibiltyType(origin_x, i, grid)) {
          return false;
         }
      }
  }
  } else if (change_x > 0) {
    // target is to the right
    // check if all rightward points before target are visible through
    if (change_y == 0) {
      for (int i = origin_x + 1; i < target_x; i++) {
         if (!checkVisibiltyType(i, origin_y, grid)) {
          return false;
         }
      }
    } else if (change_y > 0) {  // target is up and to the right
      return gridHelper(grid, change_x, change_y, target_x, target_y, origin_x, origin_y, target, true, true);
    } else {                    // target is down and to the right
      return gridHelper(grid, change_x, change_y, target_x, target_y, origin_x, origin_y, target, false, true);
    }

  } else {
    //target is to left
    if (change_y == 0) {
      // check if all leftward points before target are visible through
      for (int i = origin_x - 1; i > target_x; i--) {
         if (!checkVisibiltyType(i, origin_y, grid)) {
          return false;
         }
      }    
    } else if (change_y > 0) {  // target is up and to the left
      return gridHelper(grid, change_x, change_y, target_x, target_y, origin_x, origin_y, target, true, false);

    } else {                    // target is down and to the left
      return gridHelper(grid, change_x, change_y, target_x, target_y, origin_x, origin_y, target, false, false);
    }
  }
  return true;
}

/**************** checkVisibilityType() ****************/
/* see grid.h for description */
bool checkVisibiltyType(int x, int y, grid_t* grid)
{
  char c = grid->originalGrid[(grid->rows -y) * grid->columns + x];
  if (c == '.') {
    return true;
  }
  return false;
}

/**************** gridUpdatePlayer() ****************/
/* see grid.h for description */
void gridUpdatePlayer(grid_t* grid, player_t* player) 
{
  // update if needed in grid visual the locations of the players in grid
  if (player != NULL && !(player->hasQuit)) {
    int pos = (grid->rows - player->y) * grid->columns + player->x;
    grid->gridVisual[pos] = player->ID;
  }
}

/**************** getRandomStartPosition() ****************/
/* see grid.h for description */
int getRandomStartPosition(grid_t* grid)
{
 
  // use seed to randomly determine start position
  if (grid->seed == 0) {
    srand(getpid());
  } else {

    srand(grid->seed);
  }
  // continue until find empty spot
  int idx;
  while (true) {
    // pick to look at random point in grid
    idx = rand() % grid->gridSize;
    if ('.' == grid->gridVisual[idx]) {
      break;
    }
  }
  return idx;
}
