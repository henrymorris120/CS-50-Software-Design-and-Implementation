# CS50 Nuggets
## Implementation Spec

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a **client** and a **server**. Our design also includes **game**, **grid** and **player** modules.
We describe each program and module separately. We do not describe the support library nor the modules that enable features that go beyond the spec. Here we focus on the core subset:

* Data structure
* Control flow: pseudo code for overall flow, and each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan 

---
### **Data structure**

#### **_client struct_**
```c
typedef struct client_data {
  char player;
  int rows;
  int cols;
} client_data_t;
```
#### **_game struct_**
```c
typedef struct game {
  player_t** players;      
  addr_t specAddress;       
  grid_t* grid;              
  int goldColleted;         
  int playerCount;        
  bool hasSpectator;       
} game_t;
```
#### **_grid struct_**
```c
typedef struct grid {
  int gridSize;          
  int columns;           
  char* originalGrid;    
  int rows;               
  char* gridVisual;       
  int seed;               
  counters_t* goldAmt;   
} grid_t;
```
#### **_player struct_**
```c
typedef struct player {
  const char* name;     
  char ID;             
  int x;                
  int y;                
  addr_t addr;          
  int gold;             
  bool hasQuit;         
  bool* visibiityMap;   
} player_t;
```

---

### **Control flow**
pseudo code for overall flow, and each of the functions of the client module.

#### **Client**
#### **_main_**
```c
parses command line arguments
initialize a new client data struct

if the number of command line arguments != 3 or 4
    exit 1
if one of the arguments are NULL
    exit 1
if initialize the message module fails,
    exit 1
if initialize address using hostName and port fails,
    exit 1
if playerName == NULL
    concatenate strings and store it to a message char array
    send a message to the server
else
    send "SPECTATE" message to the server

free data

if message_loop()==false
 return -1

return 0
```

#### **_handle_message_**
```c
if arguments(message or arg) is NULL
    return false

if message is "GRID"
    if !validate_grid(message)
        send a message(KEY Q) to the server
        shut down the module
        free data
        exit 1
    return false

else if message is "OK "
    if !handle_ok_message
        send a message(KEY Q) to the server
        shut down the module
        free data
        exit 1

else if message is "DISPLAY"
    validate_display
    return false

else if message is "GOLD "
    validate_gold
    return false

else if message is "QUIT "
    close curses
    clean stdout prints
    return true

else if message is "ERROR "
    print error message

return false
```

#### **_handle_ok_message_**
```c
if message != NULL
    return false

set player value in the data struct to 0

initialize_curses()
initialize_map()

return true
```

#### **_validate_grid_**
```c
if row and col is not extracted successfully
    return false

while col > data->rows || row+1 > data->cols
    close the curses mode
    if input == '\n'
        close the curses
        initialize the curses
```

#### **_validate_display_**
```c
extract a character length of "display"
create a new string array size of displayLength

create a new string arr size of displayLength
copy the string into the new string array

add '\0' in the end of the string array
visualize_map(the new string array)
```

#### **_validate_gold_**
```c
if collected_gold, gold_purse, not_found_gold is not extracted successfully
    return false

get the terminal window size

loop through columns and add space

if data->player==0
    send a message of spectator's view
else
    send a message of player's view

print text in the terminal
refresh the curses
```

#### **_visualize_map_**
```c
loop through all rows
    loop through all columns
        curses+=1
        if (row * (data->cols+1) + col)>=strlen(map)
            fill up with blank
        else
            add map[row * (data->cols+1) + col]
refresh curses
```

#### **_initialize_map_**
```c
get the terminal window size
loop through rows
    loop through clumns
        fill up with blank
refresh curses
```

#### **_initialize_curses_**
```c
initialize the terminal window size
get the terminal window size
make curses invisible
```

#### **_handle_input_**
```c
if it's not a valid address
    return true
if server is null
    return true

clear message
send a user's message to the server

return false
```

#### **_clear_message_** 
```c
get the max window terminal size

loop through cols
    clear status message

refresh curses
```


#### **Server**
#### **_parseArgs_**
```c
if argc is 2 or 3
    exit 2
check the validity of the mapFileName
if argc ==3
    save seed value to a character pointer variable
else:
    save -1 to a chracter pointer variable
```
#### **_handleMessage_**
```c
if arg == NULL || message == NULL
    return true
initialize a new game struct

if strncmp(message, "PLAY ", strlen("PLAY "))
    addPlayer(game, fromAdress ,playerName);
else if strncmp(message, "SPECTATE", strlen("SPECTATE"))
    addSpectator(game,fromAdress);

else if strncmp(message, "KEY ", strlen("KEY "))
        handleKey(game, mover, keystroke)
else
    if strcmp(keystroke, "Q") == 0
        player_delete(spectator);

updatePlayersMessage(game); 
updateSpectatorMessage(game);

if is_over(game)
    return true;

return false;
```
#### **_gameIsOver_**
```c
loop through game->player struct
    sprintf(playerData, "%-3c %7d %s\n", game->players[i]->ID, game->players[i]->gold, game->players[i]->name);
    free the data

loop through game->playerCount
    message_send()

if a spectator
    message_send(game->specAddress, gameOverMessage);
```
#### **_addPlayer_**
```c
if game->playerCount == maxPlayers
    message_send();
    return

else if name == NULL
    message_send();
    return

else
    loop through strlen of a player name
        if !isgraph && !isspace
            change a character to '_'

if !game_add_player
    return 
else
    loop through playerCount
        send okMessage to the client
        send GRID message to the client
```
#### **_addSpectator_**
```c
if already has a spectator in a game
    message_send(Quit);
else
    create a new spectaor
    initialize variables in a game struct
    send GRID message to the client
```
#### **_handleGameQuit_**
```c
if player == NULL or game == NULL
    return

if a spectator
    game->hasSpectator = false
    message_send()
else
    handlePlayerQuit()
    message_send()
```
#### **_updateSpectatorMessage_**
```c
if a game has a spectator
    send a gold message to the client
    send a DISPLAY message to the client
```
#### **_updatePlayersMessage_**
```c
if game is NULL
    return
else
    loop through game->playerCount
        if a currentPlayer !hasQuit
            send GOLD message to a client
            set player counters
            send DISPLAY message to a client
```
#### **_handleKey_**
```c
switch key
    case key character:
        execute function to update game struct
```
#### **_handleGameQuit_**
```c
if player == NULL or game == NULL
    return 

if a spectator
    update game struct
    message_send()
else
    handlePlayerQuit()
    message_send()
```
#### **_handle_input_**
```c
if !message_isAddr(client address)
    return true
if client is NULL
    return true

string concatenationcon user character input to follow the syntax for message
send message to the client
```



#### **Game**
#### **_game_new_** 
```c
allocate memory for a new game struct

allocate memory for player of a new game struct
initialize values of the game struct

return game struct
```

#### **_playerCountToID_**
```c
if the input is within the valid range, between 0 and 25
    return the corresponding uppercase letter by adding n to the ASCII value of 'A'
else
    return terminated character '\0'
```

#### **_game_add_player_**
```c
if the number of game players < the max number of players
    determine a random starting position on the grid
    store position to the x,y
    generate id and store it to char ID

    create a new player struct and intialize it
    Increment 1 to th playerCount of the game struct

    update player's ID to the visuaLGrid of the game struct
    update the visibility map

    return true
```
 
#### **_game_delete_**
```c
if game is not NULL
    for playerCount
        delete player

    free the memory of players
    delete grid
    free the memory of game struct
```

#### **_findPlayerFromAddress_**
```c
for playerCount
    if an address of the arugment and an address from the player is equivalent
        return players of game struct
    
return null
```

#### **_findPlayerFromID_**
```c
for playerCount
    if a player's id maches with argument c
        return player of the game struct

return NULL
```

#### **_gameCollide_**
```c
for playerCount
    if a player's position and the x and y values in the argument are the same
        return player in the game struct

return NULL
```

#### **_game_player_move_**
```c
store a grid from the game struct

if there's a valid space to move
    check if collide with another player in which case switch with them
    check if there is gold in new position  
else if
    check for gold at the new position
    update gold and goldCollected values

    update x and y position
    update grid
    update visability

    return true

return false
```

#### **_is_over_**
```c
if total gold = collected gold 
    return true
else
    return false
```

#### **_handlePlayerQuit_**
```c
calculate the position index in the gridVisual array

update the gridVisual at the player's position to the original gridValue

player_quit()
```

#### **_steal_**
```c
if player1 and player2 are not NULL
    gold of player1 equals the amount of gold of player1 and player2
    gold of player2 set to 0

``` 
#### **_returnVisibilityMapWithLineBreaks_**
```c
allocate memory of visualMap of a player struct

loop through the grid 
    if visibilityMap
        update playerVisualMap to game gridVisual
        if mark this players postion
            print @
        if end of the column 
            add linebreak
    else
        playerVisualMap set to blank
    
return playerVisualMap
```
#### **_returnSpectatorVisibilityMapWithLineBreaks_**
```c
allocate memory of visualMap of a player struct

loop through the grid 
    if visibilityMap[i]
        update playerVisualMap to game gridVisual
        create a line break in the end of each column
    else
        playerVisualMap set to blank
    
return playerVisualMap
```

#### **Grid**
#### **_grid_new_** 
```c
allocate memory for a new grid structure

if the map file opens successfully
    initialize values of a grid struct

    loop through all the columns    
        initialize gridVisual
        initialize originalGrid

    close the map file
    randomly generate gold piles counters struct

    return grid struct
```

#### **_createGoldPiles_** 
```c
if the seed is 0
    generate random number using process Id
else
    generate random number using seed

calcuate gold piles
create a new gold pile counter struct
initialize an chosen gold piles array to hold the chosen positions

while pilesPicked < piles
    generate a random index within the size of the grid
    if the grid position at the index is an open spot (denoted by '.')
        change the character at that position to '*' to indicate a gold pile
        add a counter for that index to the counters structure 
        store the index in the chosenPiles array
        increment the count of piles picked

for the rest of the gold piece
    randomly select one of the chosen piles
    add a gold piece to that pile in the counters structure 

return goldPiles counter struct
```

#### **_grid_delete_**
```c
delete goldAmt counters
free gridVisual of the grid struct
free originalGrid of the grid struct
free grid struct
```

#### **_grid_can_move_** 
```c
store originalGrids that can be reachable to a char variable 

if a char variable is a hallway, a wall or another player
    update gridvisual
    return true

return false
```

#### **_gridHasGold_** 
```c
    if gridVisual has a gold pile
        get the number of gold using counters_get()
        return the number of gold
    
    return 0
```

#### **_gridUpdateVisibility_** 
```c
for each girdSize of the grid struct
    if player's visabilityMap is false
        update the player's visibility map
```

#### **_checkVisible_** 
```c
if target is at the same point
    continue

else if target is above
    for all points above before target
        if !visibilityType()
            return false

else if target is below
    for all points below target
        if !visibilityType()
        return false

else if target is to the right
    for all points right to the target
        if !visibilityType()
        return false

else if target is to the left
    for all points left to the target
        if !visibilityType()
        return false

else if target is above and to the left
    return gridHelper()

else if target is below to the left
    return gridHelper()
```

#### **_checkVisibiltyType_** 
```c
retrieve the character at the grid cell

if the character is '.'
    return true

return false
```

#### **_gridUpdatePlayer_** 
```c
if player is not NULL and the player is active
    update the gridVisual of the player struct
```

#### **_getRandomStartPosition_** 
```c
if seed of the grid struct is 0

    if seed provided
        verify it is a valid seed number
        seed the random-number generator with that seed
    else
        seed the random-number generator with getpid()

    loop through all the grid
        if '.' is found in the gridVisual
        break
    
    return index of gridVisual which contains '.'
```


#### **Player**
#### **_player_new_**
```c
allocate memory for a new Player structure 

set gold of a player struct to 0
set address of a player struct
set hasQuit of a player struct to false
set ID of a player struct 
set playerName of a player struct
set x and y position of a player struct

allocate memory for visability map of a player struct

loop through grid size
    initialize visability map false

return a player struct
```

#### **_player_delete_**
```c
if a visability map of player's struct is not NULL
    free visability_map
if player is not NULL
    free player struct
```

#### **_player_quit_**
```c
if player is not NULL
    set hasQuit of the player struct to true
    reset x and y position to -1
```

---

### **Detailed function prototypes and parameters**

#### **Client**
```c
bool handle_message(const char* message, void* arg, const addr_t from);
bool handle_ok_message(const char* message);

bool validate_grid(const char* message);
void validate_display(const char* message);
bool validate_gold(const char* message);

void visualize_map(char* map);
void initialize_map();
void initialize_curses();

bool handle_input(void* arg);
void clear_message();
```

#### **Server**
```c
static void parseArgs(const int argc, char *argv[], char** mapFileName, int* seed);
static bool handleMessage(void* arg, const addr_t fromAdress, const char* message);
static void gameIsOver(game_t* game) ;
static void addPlayer(game_t* game, addr_t fromAddress, const char* name);
static void addSpectator(game_t* game, addr_t from);
static void handleGameQuit(game_t* game, player_t* player);
static void updateSpectatorMessage(game_t* game);
static void updatePlayersMessage(game_t* game);
static void handleKey(game_t* game, player_t* player, const char* key);
static void handleGameQuit(game_t* game, player_t* player);
bool handle_input(void* arg);

```

#### **Game**
```c
game_t* game_new(char* mapFilename, int seed);

char playerCountToID(int n);
bool game_add_player(game_t* game, char* name, addr_t addr); 
void game_delete(game_t* game);

player_t* findPlayerFromAddress(game_t* game, addr_t addr); 
player_t* findPlayerFromID(game_t* game, char c); 

player_t* gameCollide(game_t* game, int pos_x, int pos_y); 
bool game_player_move(game_t* game, player_t* player, int new_x, int new_y);

bool is_over(game_t* game); 
void handlePlayerQuit(game_t* game, player_t* player);
void steal(game_t* game, player_t* player1, player_t* player2); 
char* returnVisibilityMap(game_t* game, player_t* player); 
```

#### **Grid**
```c
grid_t* grid_new(char* mapFilename, int seed); 
void grid_delete(grid_t* grid);

counters_t* createGoldPiles(char* gridVisual, int size, int seed);
bool grid_can_move(grid_t* grid, int x, int y, int oldx, int oldy);
int gridHasGold(grid_t* grid, int x, int y); 
void gridUpdateVisibility(grid_t* grid, player_t* player);

bool checkVisible(grid_t* grid, int origin_x, int origin_y, int target); 
bool checkVisibiltyType(int x, int y, grid_t* grid);

void gridUpdatePlayer(grid_t* grid, player_t* player);
int getRandomStartPosition(grid_t* grid);
```

#### **Player**
```c
player_t* player_new(const char* name, char ID, addr_t addr, int x, int y, int masterGridSize);
void player_delete(player_t* player);
void player_quit(player_t* player);
```

---

### **Error handling and recovery**


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
---

### **Limitation**
<!-- > Bulleted list of any limitations of your implementation.
> This section may not be relevant when you first write your Implementation Plan, but could be relevant after completing the implementation. -->
---
### **Plan for division of labor**
We have 4 group members, Henry, Soo, Seamus and Pauline.
- **Soo** implemented on the client and server module and documentation.
- **Henry** implemented the game, grid and player modules and server
- **Pauline** implemented server module.
- Seamus
