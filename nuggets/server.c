/* 
 * server.c - 
 * This file has server.c which implements all game logic.  It defines functionality
 * for the server, which interacts with other modules for functionality
 * to offer an interactive game.
 *
 * Team 12- November 7th Fall 2023
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "libcs50/file.h"
#include "common/grid.h"
#include "common/player.h"
#include "common/game.h"
#include "libcs50/counters.h"
#include "libcs50/mem.h"

/**************** file-local global variables ****************/
static void parseArgs(const int argc, char *argv[], char **mapFileName, int *seed);
static bool handleMessage(void *arg, const addr_t fromAdress, const char *message);
static void gameIsOver(game_t *game);
static void addPlayer(game_t *game, addr_t fromAddress, const char *name);
static void addSpectator(game_t *game, addr_t from);
static void handleGameQuit(game_t *game, player_t *player);
static void updateSpectatorMessage(game_t *game);
static void updatePlayersMessage(game_t *game);
static void handleKey(game_t *game, player_t *player, const char *key);
bool handle_input(void *arg);

/**************** main() ****************/
/*
 *Inputs are mapfilename and seed
 *Initializes the game and the message network
 *Deletes the game and closes the message loop at the end
 */
int main(const int argc, char *argv[]) {
    char *mapFileName = NULL;
    int seed = 0;

    parseArgs(argc, argv, &mapFileName, &seed); // Parse command line arguments to get the map file name and seed

    // If seed is -1, generate a random seed
    if (seed == -1) {
        srand(getpid());
    }
    else{
        srand(seed);
    }
    // Creates a new game instance using the map file and seed.
    game_t *myGame = game_new(mapFileName, seed); /*see game.h for description*/

    // Initialize the network and announce the port number.
    int myPort = message_init(NULL); // initializes the message module,handles network communication.
    if (myPort == 0) {
        flog_v(stderr, "Error, failure to initialize message module\n");
        exit(2); // failure to initialize message module
    }
    else {
        // Announces the port number on which the server is listening
        printf("Server is listening on port %d \n", myPort);
    }

    // Enters a loop waiting for and processing network messages.
    // handle_input and handleMessage are callback functions for handling input and messages.
    bool Ok = message_loop(myGame, 0, NULL, handle_input, handleMessage); 
    message_done();  //cleans up the messages                                                    

    game_delete(myGame); // deletes all the players and the grid map

    return Ok ? 0 : 1; // status code depends on result of message_loop
}

/**************** parseArgs() ****************/
/*Checks the validity of all arguments */
static void parseArgs(const int argc, char *argv[], char **mapFileName, int *seed) {

    *mapFileName = argv[1]; // check the validity of the mapFileName

    FILE *fp = fopen(*mapFileName, "r");
    // Check for errors during file opening
    if (fp == NULL) {
        flog_v(stderr, "Could not open file...\n");
        exit(3);
    }
    else {
        fclose(fp); // close the file
    }

    // Make sure seed is a valid number
    if (argc == 3) {
        char *seedInput = argv[2];
        // convert seed to int using scanf
        char nextchar = '\0';
        if (sscanf(seedInput, "%d%c", seed, &nextchar) != 1) {
            flog_v(stderr, "seed is not an a valid integer.\n");
            exit(3);
        }
    }
    else {
        *seed = -1;
    }
}

/**************** handle_input() ****************/
/*
* Inputs: arg - A pointer to the address of the client.
* Validates the client's address and checks if the argument is non-NULL.
* Then reads a character from standard input, and sends this message to the specified client address.
* Returns true if input validation fails, otherwise false after sending the message.
*/
bool handle_input(void *arg) {
    addr_t *client = arg;

    // validate arguments
    if (!message_isAddr(*client)) { // Check if the client address is valid
        fprintf(stderr, "handle_input : invalid address.\n");
        return true;
    }
    if (client == NULL) {
        fprintf(stderr, "handle_input : argument shouldn't be NULL\n");
        return true;
    }

    // Prepare message with the format "KEY [char]"
    char key_input = getchar();
    char message[strlen("KEY ") + 1 + 1];
    strcpy(message, "KEY ");
    strcat(message, &key_input);
    message[strlen("KEY ") + 1] = '\0';

    // message_send(keystroke)
    message_send(*client, message);

    return false;
}

/**************** handleMessage() ****************/
/* Processes messages received from clients and performs actions based on the message type.
* Send "malformed message" if message is invalid
*/
static bool handleMessage(void *arg, const addr_t fromAdress, const char *message) {
    // if any pointer arguments are NULL, return true and terminate game loop. */
    if (arg == NULL || message == NULL) {
        flog_v(stderr, "Entered message loop without gamestate. Fatal error occurred\n");
        return true; // Returning true to terminate the game loop due to an error.
    }

    // Cast arg to game_t pointer to access game state
    game_t *game = (game_t *)arg;

    // Handling 'PLAY' messages to add a new player.
    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
        const char *playerName = message + strlen("PLAY "); // extract player name from message
        addPlayer(game, fromAdress, playerName);
    }
    // Handling 'SPECTATE' messages to add a new spectator.
    else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
        addSpectator(game, fromAdress);
    }
    // Handling 'KEY' messages for player movement or spectator actions.
    else if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {
        const char *keystroke = message + strlen("KEY ");
        if (!message_eqAddr(fromAdress, game->specAddress)) {
            // Find the player corresponding to the sender's address
            player_t *mover = findPlayerFromAddress(game, fromAdress);
            handleKey(game, mover, keystroke);
        }
        else {
            // spectator can only quit
            if (strcmp(keystroke, "Q") == 0) {
                game->hasSpectator = false;
                message_send(game->specAddress, "QUIT Thanks for watching!");
            }
        }
    }

    else { // Handling invalid messages.
        flog_v(stderr, "ERROR: invalid message"); // sending the message back to client, i guess
        message_send(fromAdress, "ERROR invalid message\n");
    }

    updatePlayersMessage(game); /*see updateplayerMessage() for description*/
    updateSpectatorMessage(game); /*see updateplayerMessage() for description*/

    if (is_over(game)) { //if the bool is over is true, call the game is over function
        gameIsOver(game); /*see gameIsOver() for description*/
        return true;
    }
    return false;
}


/**************** gameIsOver() ****************/
/*
* Handles the actions to be taken when the game is over.
* Input is a game struct
*/
static void gameIsOver(game_t *game) {
    // prepare gameover message
    char gameOverMessage[5000];
    sprintf(gameOverMessage, "QUIT GAME OVER:\n");
    for (int i = 0; i < game->playerCount; i++) {
        char playerData[50];
        sprintf(playerData, "%-3c %7d %s\n", game->players[i]->ID, game->players[i]->gold, game->players[i]->name);
        strcat(gameOverMessage, playerData);
        free((void *)(game->players[i]->name));
    }

    // send game over message to all players
    for (int i = 0; i < game->playerCount; i++) {
        message_send(game->players[i]->addr, gameOverMessage);
    }

    // send game over message to spectator
    if (game->hasSpectator) {
        message_send(game->specAddress, gameOverMessage);
    }
}

/**************** addPlayer() ****************/
/*
* Adds a new player to the game.
* Inputs are a game struct, Address of the new player and the name of the new player
*/
static void addPlayer(game_t *game, addr_t fromAddress, const char *name) {

    const int maxPlayers = 26; //maximum number of players allowed in the game.
    // Checks if the maximum player count has been reached.
    if (game->playerCount == maxPlayers) { 
        // Sends a message to the player trying to join that the game is full.
        message_send(fromAddress, "QUIT Game is full: no more players can join.");
        return;
    }

    // Checks if the player's name is provided
    else if (name == NULL) {
        message_send(fromAddress, "QUIT Sorry - you must provide a player's name.\n");
        return;
    }

    // Truncate the name and replace invalid characters with '_'
    else {
        char *playerRealName = malloc(strlen(name) + 1);
        strcpy(playerRealName, name); // Copy at most 50 characters

        // Replaces any invalid characters in the name with '_'
        for (int i = 0; i < strlen(playerRealName); i++) {
            if (!isgraph(playerRealName[i]) && !isspace(playerRealName[i])) {
                playerRealName[i] = '_';
            }
        }
        // Attempts to add the player to the game.
        bool added = game_add_player(game, playerRealName, fromAddress);

        if (!added){ // Checks if adding the player was unsuccessful.
            message_send(fromAddress, "QUIT Game is full: no more players can join.\n");
            return;
        }

        else {
            // Iterate over the players to find the one with the matching address
            for (int i = 0; i < game->playerCount; i++) {
                if (message_eqAddr(game->players[i]->addr, fromAddress)) {
                    // Send OK message to client
                    char okMessage[10];
                    sprintf(okMessage, "OK %c\n", game->players[i]->ID);
                    message_send(fromAddress, okMessage);
                }
            }
            // Send GRID message to client
            char gridMessage[100];
            sprintf(gridMessage, "GRID %d %d\n", game->grid->rows, game->grid->columns);
            message_send(fromAddress, gridMessage);
        }
    }
}

/**************** addSpectator() ****************/
/*
* Adds a spectator to the game or replaces the existing one.
* Inputs are the game struct and adress of the new spectator
*/
static void addSpectator(game_t *game, addr_t from) {
    
    // if there is already a spectator, send QUIT
    if (game->hasSpectator == true) {
        // Notify the existing spectator that they are being replaced.
        message_send(game->specAddress, "QUIT You have been replaced by a new spectator");
        
         // Send the grid dimensions to the new spectator.
        char gridMessage[100];
        sprintf(gridMessage, "GRID %d %d", game->grid->rows, game->grid->columns);
        message_send(from, gridMessage);

        // Calculate the remaining gold and send the information to the new spectator.
        char* goldMsg = mem_malloc_assert(sizeof(char) * 30, "mem0ry is full");
        int goldLeft = 250 - game->goldColleted;
        sprintf(goldMsg, "GOLD %d %d %d\n", 0, 0, goldLeft);
        message_send(game->specAddress, goldMsg);
        free(goldMsg);
        char *gridString = returnSpectatorVisibilityMapWithLineBreaks(game);

        // Prepare and send the current game display to the new spectator.
        char *prefix = "DISPLAY\n";
        char *displayMessage = mem_malloc_assert(strlen(prefix) + strlen(gridString) + 1, "memory is full");
        strcpy(displayMessage, prefix); // Copy the first string
        strcat(displayMessage, gridString);
        message_send(game->specAddress, displayMessage);
        mem_free(gridString); // free allocated memory
        mem_free(displayMessage); // free allocated memory

        game->specAddress = from; // Update the spectator's address.
    }
    else {
        // If there is no existing spectator, add the new one.
        game->specAddress = from;
        game->hasSpectator = true;

        // Send the grid dimensions to the new spectator
        char gridMessage[100];
        sprintf(gridMessage, "GRID %d %d", game->grid->rows, game->grid->columns);
        message_send(from, gridMessage);

        char* goldMsg = mem_malloc_assert(sizeof(char) * 30, "mem0ry is full");
        int goldLeft = 250 - game->goldColleted;
        sprintf(goldMsg, "GOLD %d %d %d\n", 0, 0, goldLeft);
        message_send(game->specAddress, goldMsg);
        free(goldMsg);

        char *gridString = returnSpectatorVisibilityMapWithLineBreaks(game);

        // send DISPLAY message to spectator
        char *prefix = "DISPLAY\n";
        char *displayMessage = mem_malloc_assert(strlen(prefix) + strlen(gridString) + 1, "memory is full");
        strcpy(displayMessage, prefix); // Copy the first string
        strcat(displayMessage, gridString);
        message_send(game->specAddress, displayMessage);
        mem_free(gridString); // free allocated memory
        mem_free(displayMessage); // free allocated memory

    }
}

/**************** handleGameQuit() ****************/
/*
* Handles the quitting process for players and the spectator in the game.
* If the player is the spectator, change the game's hasSpect boolean to false.
*/
static void handleGameQuit(game_t *game, player_t *player) {
    // Checks if the game or player data is invalid.
    if (player == NULL || game == NULL) {
        fprintf(stderr, "Error: Invalid game or player data\n");
        return;
    }
    // Checks if the player is the spectator.
    if (message_eqAddr(player->addr, game->specAddress)) {
        game->hasSpectator = false;
        message_send(game->specAddress, "QUIT Thanks for watching!");
       }
    else {
        handlePlayerQuit(game, player); // Handle quitting for a regular player
        message_send(player->addr, "QUIT Thanks for playing!");
    }
}

/*
* Input is a game struct
* Sends GOLD and DISPLAY message to the spectator.
*/
static void updateSpectatorMessage(game_t *game) {
    // Check if the game has a spectator.
    if (game->hasSpectator) {
        char* goldMsg = mem_malloc_assert(sizeof(char) * 30, "mem0ry is full");
        int goldLeft = 250 - game->goldColleted;
        sprintf(goldMsg, "GOLD %d %d %d\n", 0, 0, goldLeft);
        message_send(game->specAddress, goldMsg);
        free(goldMsg);

        char *gridString = returnSpectatorVisibilityMapWithLineBreaks(game);

        // send DISPLAY message to spectator
        //  allocate memory to display message string
        char *prefix = "DISPLAY\n";
        char *displayMessage = mem_malloc_assert(strlen(prefix) + strlen(gridString) + 1, "memory is full");
        strcpy(displayMessage, prefix); // Copy the first string
        strcat(displayMessage, gridString);
        message_send(game->specAddress, displayMessage);
        // free allocated memory
        mem_free(gridString);
        mem_free(displayMessage);
    }
}

/**************** updatePlayersMessage() ****************/
/* Loops through players and sends GOLD and DISPLAY messages to their clients.
* Input is the game struct
*/

static void updatePlayersMessage(game_t *game) { 
    // Check if the game state is valid.
    if (game == NULL) {
        fprintf(stderr, "Error: Game state is NULL in updatePlayersMessage.\n");
        return;
    }
    // Loop through all players in the game.
    for (int i = 0; i < game->playerCount; i++) {
        player_t *currentPlayer = game->players[i];
        if (currentPlayer->hasQuit) { // Skip updating messages for players who have quit.
            continue;
        }
        else {
            // Send GOLD message to each player
            char goldMsg[100];
            const int pos = (game->grid->rows - currentPlayer->y) * game->grid->columns + currentPlayer->x;
            sprintf(goldMsg, "GOLD %d %d %d\n", counters_get(game->grid->goldAmt, pos), currentPlayer->gold, 250 - game->goldColleted);
            message_send(currentPlayer->addr, goldMsg);
            counters_set(game->grid->goldAmt, pos, 0);

            /* see game.h for description */
            char *gridString = returnVisibilityMapWithLineBreaks(game, currentPlayer);

            // send DISPLAY message to spectator
            //  allocate memory to display message string
            char *prefix = "DISPLAY\n";
            char *displayMessage = mem_malloc_assert(strlen(prefix) + strlen(gridString) + 1, "memory is full");
            strcpy(displayMessage, prefix); // Copy the first string
            strcat(displayMessage, gridString);
            message_send(currentPlayer->addr, displayMessage);
            // free allocated memory
            mem_free(gridString);
            mem_free(displayMessage);
        }
    }
}

/**************** handleKey() ****************/
/* Processes player's key input for movement in the game.
* Inputs are game, player and the key from the player.
* The function uses a switch statement to determine the player's action based on the key input
*/

static void handleKey(game_t *game, player_t *player, const char *key) {
    switch (*key) {

    case 'h': // move left
        game_player_move(game, player, player->x - 1, player->y);
        break;

    case 'l': // move right
        game_player_move(game, player, player->x + 1, player->y);
        break;

    case 'j': // move down
        game_player_move(game, player, player->x, player->y - 1);
        break;

    case 'k': // move up
        game_player_move(game, player, player->x, player->y + 1);
        break;

    case 'y': //  move diagonally up and left,
        game_player_move(game, player, player->x - 1, player->y + 1);
        break;

    case 'u': // move diagonally up and right
        game_player_move(game, player, player->x + 1, player->y + 1); 
        break;

    case 'b': // move diagonally down and left
        game_player_move(game, player, player->x - 1, player->y - 1); 
        break;

    case 'n': // move diagonally down and right
        game_player_move(game, player, player->x + 1, player->y - 1); 
        break;
        ;
    case 'L': // move right automatically and repeatedly till you hit a wall
        while (game_player_move(game, player, player->x + 1, player->y)) {
        }
        break;

    case 'H': // move left automatically and repeatedly till you hit a wall
        while (game_player_move(game, player, player->x - 1, player->y)) {
        }
        break;

    case 'K': // move up automatically and repeatedly till you hit a wall
        while (game_player_move(game, player, player->x, player->y + 1)) {
        }
        break;

    case 'J': // move down automatically and repeatedly till you hit a wall
        while (game_player_move(game, player, player->x, player->y - 1)) {
        }
        break;

    case 'U': // move diagonally up and right repeatedly till you hit a wall
        while (game_player_move(game, player, player->x + 1, player->y + 1)) {
        }
        break;

    case 'Y': // move diagonally up and left repeatedly till you hit a wall
        while (game_player_move(game, player, player->x - 1, player->y + 1)) {
        }
        break;

    case 'B': // move diagonally down and left repeatedly till you hit a wall
        while (game_player_move(game, player, player->x - 1, player->y - 1)) {
        }
        break;

    case 'N': // move diagonally down and right repeatedly till you hit a wall
        while (game_player_move(game, player, player->x + 1, player->y - 1)) {
        }
        break;

    case 'Q': // quits the game
        handleGameQuit(game, player);
        break;

    default: // Handle unknown keystroke
        flog_v(stderr, "ERROR usage: unknown keystroke");
        message_send(player->addr, "ERROR usage: unknown keystroke");
        break;
    }
}
