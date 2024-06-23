# CS50 Nuggets
## Design Spec
### Fall, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: **a client and a server.**
Our design also includes **game, grid, and player modules**.

We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

---

### **Client**
The *client* acts in one of two modes:

 - *spectator* : the passive spectator mode described in the requirements spec.
 - *player* : the interactive game-playing mode described in the requirements spec.


### **User interface**
The first row will indicate the game's status, while the remaining rows will show the client's perspective of the game state.


### **Inputs and outputs**
The first interface with the user is on the **command-line**. It must always have **two arguments**.

```
./client hostName port [playerName]
```
- This command starts the game client, which connects to the game server.
- The **hostName** specifies the IP address or domain name of the server
- The **port** specifies the port number on which the server is listening for client connections.
- The **[playername]** can be used to set the player's name in the game, and if absent
specifies the user as the spectator. (Optional)

#### **Inputs**
User keystrokes from stdin during gameplay. These keystrokes are read by the program to determine the player's actions in the game. For more details on which keystrokes are valid and what
a particular keystroke might do, see the [Requirements Spec](REQUIREMENTS.md).

#### **Outputs**
The client outputs a **display on the terminal**
  - **Grid** : representation of the game ([Requirements Spec](REQUIREMENTS.md).)
  - **Game status**
    - Player's ID
    - The number of nuggets collected by the player
    - The number of unclaimed nuggets
    - Any relevant messages / error messages.


### **Functional decomposition into modules**

The client module implements the following functions:

* **_main_** : It parses command-line arguments, connects to the server, initializes the message module, and starts the message loop.
* **_handle_message_** : Handles user input(keystrokes) and call a proper function which sends a message to the server.
* **_handle_ok_message_** : Store a playerName in the data struct and initialize the map and curses.

* **_validate_grid_** : Initialize a rows and cols in the data sturct.
* **_validate_display_** : Concatentate strings and visualize the map.
* **_validate_gold_** : Display the gold status message in the terminal.

* **_visualize_map_** : Display the map into curses.
* **_initialize_map_** : Initialize a map with blanks.
* **_initialize_curses_** : Initialize a curses.

* **_handle_input_** : Read a input(keystrokes) and send it to the server.
* **_clear_message_** : clears string after handling gold status message.




### **Pseudo code for logic/algorithmic flow**

#### **_main_**
  ```
parses command-line arguments
initializes the message module  
initialize address using hostName and port 
connects to the server : connect as player orspectator
start looping for handling input and incomingmessages.
```
#### **_handle_message_**
```
validate arguments
compare the inputs and strings
call a function which calls proper function
```
#### **_handle_ok_message_**
```
initialize a player in the data structure to 0
initialize a map
initialize a curses
```
#### **_validate_grid_**
```
loop until data->rows and cols reach the valid sizeof the screen
check the input and initialize the curses
reassign the values to the rows and cols in the datastructure
```
#### **_validate_display_**
```
calculate the size of display length
create a new string array size of dislay length
add terminate charater in the end of the string array
visualize map using the string array
```
#### **_validate_gold_**
```
validate gold message
if a user is a spectator mode, show a gold messageof a spectator's view
else show gold messages of a spectator's view
refresh the curses
```
#### **_visualize_map_**
```
loop through all the columns and rows 
display the map into CURSES or blanks depends on the condition
```
#### **_initialize_map_**
```
retrieve the max size of the screen
loop through all the columns and rows
initialize a map with blank
```
#### **_initialize_curses_**
```
initiialize the screen
get the max size of the screen
set curses invisible
```
#### **_handle_input_**
```
validate arguments
convert a keystroke input to the validate message syntax string
clear previous message
send a message to the server
```
#### **_clear_message_** 
```
retrieve the max size of the screen
loop through all the columns and go to the next column when '.' appears
refresh curses
```

### **Major data structures**

Global static variable clientData. This struct contains three primitive data types which store important information : playerName, rows and columns so that the program can send the proper message to the server depends on a user's position.

---
### **Server**
#### **User interface**
See the requirements spec for the command-line interface.
**There is no interaction with the user** but it does take in arguments from the command line

```
./server map.txt [seed]
```
- map.txt : the pathname for a map file
- [seed] : the random-number generator, if provided, the seed must be a positive integer.(optional)


#### **Inputs and outputs**

#### **Inputs**
The inputs are the messages from the client to the server.

#### **Outputs**
- **The port number** that will be used by the client to connect to the server upon initialization. 
- **the map** that is updated based on the keystroke inputs from the client. 
- **continuous messages to the client to update the map or game status** that report errors, or tell the client to quit, during game play.
- **The game summary** including player names and scores when the game is over.


#### **Functional decomposition into modules**
The `server` module will implement the following functions.
* **_main_** : A function that parses and validates the command-line arguments, extracting the map file name and seed if given.


#### **Pseudo code for logic/algorithmic flow**
The server will run as follows:
 
 ******NEED TO UPDATE here******

---
### **Game**

#### **Functional decomposition into modules**

The `game` module will implement the following functions:

* **_gameNew_** : create a new game struct and initialize it.
* **_playerCountToID_** : validate if the argument is in range of 0 and 25
* **_game_add_player_** : creates a new player and adds it to array of player pointers in order to updates playerCount
* **_game_delete_** : delete the game struct, free all allocated memory space
* **_findPlayerFromAddress_** : retrieve a player using address.
* **_findPlayerFromID_** : retrieve a player using ID.
* **_gameCollide_** : check if any player is at that exact position.
* **_game_player_move_** : update a player to the new position and and update the game and player's gold status if needed. otherwise update the new position and player's visability only.
* **_is_over_** : if all the gold is collected, return true
* **_handlePlayerQuit_** : set in grid a new character in the postion that the player was calls playerQuit function in player module
* **_returnVisibilityMap_** : iterate through the players visibility grid and create grid map that shows all visiability of the players.



### **Pseudo code for logic/algorithmic flow**

#### **_game_New_**
```
create a new game struct
initialize players, goldColleted, grid, playerCount, and hasSpectator values of the struct
```
#### **_playerCountToID_**
```
validate the argument
if it's in the range
  return a concatenated string
otherwise
  return terminated character.
```
#### **_game_add_player_**
```
if the number of players haven't reached limit
  get random start postion in empty spot of grid
  update the values in the game struct
```
#### **_game_delete_**
```
if the argument(game struct) is not null
  loop through players and delete a player
  free a player
  delete grid
  free game
```
#### **_findPlayerFromAddress_**
```
loop through all the players
compare the address(argument) and each player's address
```
#### **_findPlayerFromID_**
```
loop through all the players
compare the id(argument) and each player's address
```
#### **_gameCollide_** 
```
loop through all the players
compare the x and y positions(argument) and each player's x and y positions
```
#### **_game_player_move_**
```
check if valid space to move to
if collide with another player, switch with them
if there is gold in new position, update the game and the player's gold status
update a new position
update player's position
update player's visability
```
#### **_is_over_** 
```
check if the number of gold collected and the number of total gold are same
```
#### **__handlePlayerQuit__** 
```
compare the grid based on the user position and originial grid
if they are same, quit the game of the plyer
```
#### **_returnVisibilityMap_** 
```
loop through the players visibility grid
create grid map the shows points only if visibility true and have been seen
mark the player's individual positon with an "@"
return a grid map which the user is responsible for freeing
```


### **Major data structures**
  * The **game struct** will store the information of the game.
    - **_players_** that holds pointer to struct player in array
    - **_specAddress_** for storing an address of spectator
    - **_grid_** for storing the grid of the game
    - **_goldCollected_** that holds the number of gold collected by players 
    - **_playerCount_** that holds the number of players that have joint
    - **_hasSpectator_** that storing boolean if a spectator is watching

---

## **grid.c**
The grid module represents the game grid/map. It is represented by a two dimensional char array structure that holds the layout of the game map.


### **Functional decomposition into modules**
The grid module will implement the following functions:

* **_grid_new_** : creates and initialize a new grid_t struct
* **_createGoldPiles_** : randomly generate gold in the map and initialize a counters struct wher e key is the index location in gridVisual of a gold pile and item is the amount of gold stored in them map
* **_grid_delete_** : delete the counters and free all the values in the grid struct and free grid in the end
* **_grid_can_move_** : if the point in grid(x,y) is movable and reverts based on original grid
* **_gridHasGold_** : Checks if gold has been stored in point (x,y) of the map and returns the amount of gold that has been stored there
* **_gridUpdateVisibility_** : using players x and y, assign boolean values for each grid point's visibility.
* **_checkVisible_** : return boolean corresponding to if in gridArray a player at the origin (x,y) can see the grid point target
* **_checkVisibiltyType_** : returns boolean corresponding to if in the original grid a point corresponding to (x,y) is an empty spot or not.
* **_gridUpdatePlayer_** : marks the location of the player in gridVisual which is part of the grid struct.
* **_getRandomStartPosition_** : return randomly an index in gridVisual from the inputted grid struct that is empty



### **Pseudo code for logic/algorithmic flow**

#### **_grid_new_**
```
allocate memory for a new grid struct
open a file named fileName(argument)
initialize the values in the grid struct
close the file
randomly create the gold piles counters struct
return grid struct
```
#### **_createGoldPiles_**
```
randomly determine pile number using seed
continue in loop until chosen valid grid points for the randomly decided above needed piles
  select at random point in grid
  allocate 1 gold in the piles
loop through piles and allocate remaining gold randomly
```
#### **_grid_delete_** 
```
delete counters
free all values in the grid struct
free grid
```
#### **_grid_can_move_**  
```
if player wants to move to a hallway or collide with another player, or empty spot
  update grid
```
#### **_gridHasGold_**
```
if point is a gold pile 
  look the counters to retrieve the amount of the gold at the point
```
#### **_gridUpdateVisibility_**
```
loop through all the grids
  check if point has still not been seen
```
#### **_checkVisible_** 
```
check if a target is at the same position
all above points before target are visible through if a target is above
all below points before target are visible through if a target is below 
all rightward points before target are visible through if a target is to the right
check if all leftward points before target are visible through if a target is to the left
target is up and to the left, check if a point can be seen
target is down and to the left, check if a point can be see
```
#### **_checkVisibiltyType_**
```
if in the original grid a point corresponding to (x,y) is an empty spot
  return true
else 
  return false
```
#### **_getRandomStartPosition_**
```
loop through grid until find an empty spot
return gridVisual index number
```

 
### **Major data structures**
The major data structure in the **grid struct**
```
typedef struct grid {
  int gridSize;           // number of total points on grid
  int columns;            // numer of columns
  char* originalGrid;     // array that maps the nugget game before players and gold added
  int rows;               // number or rows
  char* gridVisual;       // array of characters correspond to their  grid positon
  int seed;               // seed for randomness
  counters_t* goldAmt;    // tracks the amount of gold in each pile
} grid_t;
```
---
## **player.c**

The player module contains a string of the players name, a character of the players ID, int of the x and y coordinate of the player, the amount of gold the player has, a string of the player_map with what is visible to the player, and the adress of the player.


### Functional decomposition into modules
The player module will implement the following structures:

* **_player_new_** : create a new player struct and initialize values in the struct
* **_player_delete_** : frees in memory visibilty map and player if not null
* **_player_quit_** : sets a player struct's hasQuit to true

### Pseudo code for logic/algorithmic flow
#### **_player_new_**
```
allocate memory for a new player struct
set values from arguments into player struct
return a player struct
```
#### **_player_delete_**
```
check if values is NULL
free them if it's not NULL
```
#### **_player_quit_**
```
set hasQuit value to true
set position to x=-1, y=-1
```

### **Major data structures**
The major data structure in the **player struct**
```
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
```
---
### **Testing plan**
#### **unit tests**
#### **integration tests**
#### **system tests**
# CS50 Nuggets
## Design Spec
### Team 12-Sookyoung Park, Fall, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: **a client and a server.**
Our design also includes **game, grid, and player modules**.

We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

---

### **Client**
The *client* acts in one of two modes:

 - *spectator* : the passive spectator mode described in the requirements spec.
 - *player* : the interactive game-playing mode described in the requirements spec.


### **User interface**
The first row will indicate the game's status, while the remaining rows will show the client's perspective of the game state.


### **Inputs and outputs**
The first interface with the user is on the **command-line**. It must always have **two arguments**.

```
./client hostName port [playerName]
```
- This command starts the game client, which connects to the game server.
- The **hostName** specifies the IP address or domain name of the server
- The **port** specifies the port number on which the server is listening for client connections.
- The **[playername]** can be used to set the player's name in the game, and if absent
specifies the user as the spectator. (Optional)

#### **Inputs**
User keystrokes from stdin during gameplay. These keystrokes are read by the program to determine the player's actions in the game. For more details on which keystrokes are valid and what
a particular keystroke might do, see the [Requirements Spec](REQUIREMENTS.md).

#### **Outputs**
The client outputs a **display on the terminal**
  - **Grid** : representation of the game ([Requirements Spec](REQUIREMENTS.md).)
  - **Game status**
    - Player's ID
    - The number of nuggets collected by the player
    - The number of unclaimed nuggets
    - Any relevant messages / error messages.


### **Functional decomposition into modules**

The client module implements the following functions:

* **_main_** : It parses command-line arguments, connects to the server, initializes the message module, and starts the message loop.
* **_handle_message_** : Handles user input(keystrokes) and call a proper function which sends a message to the server.
* **_handle_ok_message_** : Store a playerName in the data struct and initialize the map and curses.

* **_validate_grid_** : Initialize a rows and cols in the data sturct.
* **_validate_display_** : Concatentate strings and visualize the map.
* **_validate_gold_** : Display the gold status message in the terminal.

* **_visualize_map_** : Display the map into curses.
* **_initialize_map_** : Initialize a map with blanks.
* **_initialize_curses_** : Initialize a curses.

* **_handle_input_** : Read a input(keystrokes) and send it to the server.
* **_clear_message_** : clears string after handling gold status message.




### **Pseudo code for logic/algorithmic flow**

#### **_main_**
  ```
parses command-line arguments
initializes the message module  
initialize address using hostName and port 
connects to the server : connect as player orspectator
start looping for handling input and incomingmessages.
```
#### **_handle_message_**
```
validate arguments
compare the inputs and strings
call a function which calls proper function
```
#### **_handle_ok_message_**
```
initialize a player in the data structure to 0
initialize a map
initialize a curses
```
#### **_validate_grid_**
```
loop until data->rows and cols reach the valid sizeof the screen
check the input and initialize the curses
reassign the values to the rows and cols in the datastructure
```
#### **_validate_display_**
```
calculate the size of display length
create a new string array size of dislay length
add terminate charater in the end of the string array
visualize map using the string array
```
#### **_validate_gold_**
```
validate gold message
if a user is a spectator mode, show a gold messageof a spectator's view
else show gold messages of a spectator's view
refresh the curses
```
#### **_visualize_map_**
```
loop through all the columns and rows 
display the map into CURSES or blanks depends on the condition
```
#### **_initialize_map_**
```
retrieve the max size of the screen
loop through all the columns and rows
initialize a map with blank
```
#### **_initialize_curses_**
```
initiialize the screen
get the max size of the screen
set curses invisible
```
#### **_handle_input_**
```
validate arguments
convert a keystroke input to the validate message syntax string
clear previous message
send a message to the server
```
#### **_clear_message_** 
```
retrieve the max size of the screen
loop through all the columns and go to the next column when '.' appears
refresh curses
```

### **Major data structures**

Global static variable clientData. This struct contains three primitive data types which store important information : playerName, rows and columns so that the program can send the proper message to the server depends on a user's position.

---
### **Server**
#### **User interface**
See the requirements spec for the command-line interface.
**There is no interaction with the user** but it does take in arguments from the command line

```
./server map.txt [seed]
```
- map.txt : the pathname for a map file
- [seed] : the random-number generator, if provided, the seed must be a positive integer.(optional)


#### **Inputs and outputs**

#### **Inputs**
The inputs are the messages from the client to the server.

#### **Outputs**
- **The port number** that will be used by the client to connect to the server upon initialization. 
- **the map** that is updated based on the keystroke inputs from the client. 
- **continuous messages to the client to update the map or game status** that report errors, or tell the client to quit, during game play.
- **The game summary** including player names and scores when the game is over.


#### **Functional decomposition into modules**
The `server` module will implement the following functions.
* **_main_** : initializes the game and send the message to the client. Delete the game at the end
* **_parseArgs_** : check the validity of all arguments
* **_handleMessage_** : processes messages received from clients and performs actions based on the message type.
* **_gameIsOver_** : handle the actions to be taken when the game is over.
* **_addPlayer_** : add a new player to the game.
* **_addSpectator_** : add a spectator to the game or replaces the existing one.
* **_handleGameQuit_** : handle the quitting process for players and the spectator in the game.
* **_updateSpectatorMessage_** : send GOLD and DISPLAY messages to the spectator(client).
* **_updatePlayersMessage_** : loops through players and sends GOLD and DISPLAY messages to their clients.
* **_handleKey_** : process player's key input for movement in the game.
* **_handleGameQuit_** : Handle quitting process for players and the spectator in the game.


#### **Pseudo code for logic/algorithmic flow**
The server will run as follows:
#### **_main_** 
```
parse command line arguments to get the map file name and seed
initialize the game 
initialize message module and port number
execute message_loop
clean up all the messages
delets all players, grid, and map
```
#### **_parseArgs_** 
```
check the number of arguments
check the validity of the mapFileName
open map files
check if seed is a valid number
```
#### **_handleMessage_**
```
if any arguments are NULL, return true and terminate game loop
compare a user's string and call a function to update struct if needed
```
#### **_gameIsOver_**
```
send game over message to all players
send game over message to a spectator
```
#### **_addPlayer_**
```
check if game is full
validate a player's name
truncate the name and replace invalid characters with '_'
add a player
iterate over the players to find the one with the matching address
Send GRID message to a player
```
#### **_addSpectator_**
```
if there is already a spectator, quit a game
create new spectator
send GRID message to a player
```
#### **_handleGameQuit_**
```
validate game and player data
check if the player is the spectator and send quit message to spectator
if not quite a game
```
#### **_updateSpectatorMessage_**
```
allocate memory to display message string
send GOLD message to specator
free message
```
#### **_updatePlayersMessage_**
```
check if game state is NULL
find a current player in the game struct
if player has not quit, send GOLD message to player
send DISPLAY message to spectator
free allocated memory
```
#### **_handleKey_**
```
compare the input string and execute a function to update a game struct
```
#### **_handleGameQuit_**
```
validate game and player data
if spectator, send quit message to spectator
if player, send quit message to a player
```

---
### **Game**

#### **Functional decomposition into modules**

The `game` module will implement the following functions:

* **_gameNew_** : create a new game struct and initialize it.
* **_playerCountToID_** : validate if the argument is in range of 0 and 25
* **_game_add_player_** : creates a new player and adds it to array of player pointers in order to updates playerCount
* **_game_delete_** : delete the game struct, free all allocated memory space
* **_findPlayerFromAddress_** : retrieve a player using address.
* **_findPlayerFromID_** : retrieve a player using ID.
* **_gameCollide_** : check if any player is at that exact position.
* **_game_player_move_** : update a player to the new position and and update the game and player's gold status if needed. otherwise update the new position and player's visability only.
* **_is_over_** : if all the gold is collected, return true
* **_handlePlayerQuit_** : set in grid a new character in the postion that the player was calls playerQuit function in player module
* **_returnVisibilityMapWithLineBreaks_** : iterate through the players visibility grid and create grid map the shows points only if visibility true
* **_returnSpectatorVisibilityMapWithLineBreaks_** :  returns the visual grid that spectator see with line breaks


### **Pseudo code for logic/algorithmic flow**

#### **_game_New_**
```
create a new game struct
initialize players, goldColleted, grid, playerCount, and hasSpectator values of the struct
```
#### **_playerCountToID_**
```
validate the argument
if it's in the range
  return a concatenated string
otherwise
  return terminated character.
```
#### **_game_add_player_**
```
if the number of players haven't reached limit
  get random start postion in empty spot of grid
  update the values in the game struct
```
#### **_game_delete_**
```
if the argument(game struct) is not null
  loop through players and delete a player
  free a player
  delete grid
  free game
```
#### **_findPlayerFromAddress_**
```
loop through all the players
compare the address(argument) and each player's address
```
#### **_findPlayerFromID_**
```
loop through all the players
compare the id(argument) and each player's address
```
#### **_gameCollide_** 
```
loop through all the players
compare the x and y positions(argument) and each player's x and y positions
```
#### **_game_player_move_**
```
check if valid space to move to
if collide with another player, switch with them
if there is gold in new position, update the game and the player's gold status
update a new position
update player's position
update player's visability
```
#### **_is_over_** 
```
check if the number of gold collected and the number of total gold are same
```
#### **__handlePlayerQuit__** 
```
compare the grid based on the user position and originial grid
if they are same, quit the game of the plyer
```
#### **_returnVisibilityMapWithLineBreaks_** 
```
allocate memory for playerVisualMap
check if point is visible or just was seen
mark this players postion with an @
create a line breaks
add a terminate character in the end
```
#### **_returnSpectatorVisibilityMapWithLineBreaks_** 
```
allocate memory for playerVisualMap
create a line breaks
add a terminate character in the end
```

### **Major data structures**
  * The **game struct** will store the information of the game.
    - **_players_** that holds pointer to struct player in array
    - **_specAddress_** for storing an address of spectator
    - **_grid_** for storing the grid of the game
    - **_goldCollected_** that holds the number of gold collected by players 
    - **_playerCount_** that holds the number of players that have joint
    - **_hasSpectator_** that storing boolean if a spectator is watching

---

## **grid.c**
The grid module represents the game grid/map. It is represented by a two dimensional char array structure that holds the layout of the game map.


### **Functional decomposition into modules**
The grid module will implement the following functions:

* **_grid_new_** : creates and initialize a new grid_t struct
* **_createGoldPiles_** : randomly generate gold in the map and initialize a counters struct wher e key is the index location in gridVisual of a gold pile and item is the amount of gold stored in them map
* **_grid_delete_** : delete the counters and free all the values in the grid struct and free grid in the end
* **_grid_can_move_** : if the point in grid(x,y) is movable and reverts based on original grid
* **_gridHasGold_** : Checks if gold has been stored in point (x,y) of the map and returns the amount of gold that has been stored there
* **_gridUpdateVisibility_** : using players x and y, assign boolean values for each grid point's visibility.
* **_checkVisible_** : return boolean corresponding to if in gridArray a player at the origin (x,y) can see the grid point target
* **_checkVisibiltyType_** : returns boolean corresponding to if in the original grid a point corresponding to (x,y) is an empty spot or not.
* **_gridUpdatePlayer_** : marks the location of the player in gridVisual which is part of the grid struct.
* **_getRandomStartPosition_** : return randomly an index in gridVisual from the inputted grid struct that is empty



### **Pseudo code for logic/algorithmic flow**

#### **_grid_new_**
```
allocate memory for a new grid struct
open a file named fileName(argument)
initialize the values in the grid struct
close the file
randomly create the gold piles counters struct
return grid struct
```
#### **_createGoldPiles_**
```
randomly determine pile number using seed
continue in loop until chosen valid grid points for the randomly decided above needed piles
  select at random point in grid
  allocate 1 gold in the piles
loop through piles and allocate remaining gold randomly
```
#### **_grid_delete_** 
```
delete counters
free all values in the grid struct
free grid
```
#### **_grid_can_move_**  
```
if player wants to move to a hallway or collide with another player, or empty spot
  update grid
```
#### **_gridHasGold_**
```
if point is a gold pile 
  look the counters to retrieve the amount of the gold at the point
```
#### **_gridUpdateVisibility_**
```
loop through all the grids
  check if point has still not been seen
```
#### **_checkVisible_** 
```
check if a target is at the same position
all above points before target are visible through if a target is above
all below points before target are visible through if a target is below 
all rightward points before target are visible through if a target is to the right
check if all leftward points before target are visible through if a target is to the left
target is up and to the left, check if a point can be seen
target is down and to the left, check if a point can be see
```
#### **_checkVisibiltyType_**
```
if in the original grid a point corresponding to (x,y) is an empty spot
  return true
else 
  return false
```
#### **_getRandomStartPosition_**
```
loop through grid until find an empty spot
return gridVisual index number
```

 
### **Major data structures**
The major data structure in the **grid struct**
```
typedef struct grid {
  int gridSize;           // number of total points on grid
  int columns;            // numer of columns
  char* originalGrid;     // array that maps the nugget game before players and gold added
  int rows;               // number or rows
  char* gridVisual;       // array of characters correspond to their  grid positon
  int seed;               // seed for randomness
  counters_t* goldAmt;    // tracks the amount of gold in each pile
} grid_t;
```
---
## **player.c**

The player module contains a string of the players name, a character of the players ID, int of the x and y coordinate of the player, the amount of gold the player has, a string of the player_map with what is visible to the player, and the adress of the player.


### Functional decomposition into modules
The player module will implement the following structures:

* **_player_new_** : create a new player struct and initialize values in the struct
* **_player_delete_** : frees in memory visibilty map and player if not null
* **_player_quit_** : sets a player struct's hasQuit to true

### Pseudo code for logic/algorithmic flow
#### **_player_new_**
```
allocate memory for a new player struct
set values from arguments into player struct
return a player struct
```
#### **_player_delete_**
```
check if values is NULL
free them if it's not NULL
```
#### **_player_quit_**
```
set hasQuit value to true
set position to x=-1, y=-1
```

### **Major data structures**
The major data structure in the **player struct**
```
typedef struct player {
  const char* name;     // playser name    
  char ID;              // ID character for player 
  int x;                // x position in grid
  int y;                // y postion in grid
  addr_t addr;          // address
  int gold;             // amount of gold collected
  bool hasQuit;         // indicated if still in game
  bool* visibiityMap;   // pointers to true/false values based on visibility
} player_t;
```
---
### **Testing plan**
#### **Unit tests**
- **Testing for game, grid, map modules** 

  test.c file for confirming visibility of maps, grid, game modules.

#### **Integration tests**
- Testing client and server modules

  - integration tests for client and server modules focus on validating the interaction between the two components.
  - For a typical client-server architecture, this may involve testing how well the client communicates with the server and vice versa.


#### **System tests**
- Functional Testing
  - Validates the primary functions and user scenarios of the system.
Ensures that the system's features operate correctly and in accordance with the defined requirements.
- User Interface (UI) Testing:
  - Evaluates the user interface for intuitiveness, consistency, and user-friendliness. Examines the UI's layout, design, and overall user experience.
