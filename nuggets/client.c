/* 
 * client.c - CS50 'client' module
 *
 * The client side for the Nuggets.
 * The client is launched by the user providing hostname and port to connect to the server.
 * 
 * Send a PLAY message to the client after connecting to the server.
 *      responses from the server are displayed
 *      the user interacts with the display using keystrokes.
 * 
 * exit 1 if, errors
 * 
 * Sookyoung Park
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "message.h"

/**************** global types ****************/
typedef struct client_data {
  char player;
  // initialized window size
  int rows;
  int cols;
} client_data_t;

void initialize_map();
void initialize_curses();
void clear_message();

bool handle_input(void* arg);
bool handle_message(void* arg,const addr_t from, const char* message);
bool check_ok_message(const char* message);

bool resize_grid(const char* message);
void display_map(const char* message);
bool display_gold_message(const char* message);

client_data_t* data_new();
static client_data_t* data;


/* ******** client_data_new *******
 * make initialize new client_data struct
 * allocate memory for client struct
 * initialize the values in the struct
 * 
 * Caller is responsible for:
 * later free the struct; 
 */
client_data_t* client_data_new(){

  client_data_t* data = malloc(sizeof(client_data_t));
  if (data == NULL) {
      fprintf(stderr, "data allocation failed\n");
      exit(1);
  }
  data->rows = -1;
  data->cols = -1;
  data->player = 0;

  return data;
}


/* ******** initialize_map *******
 * initialize a map with blanks before starting a game
 */
void initialize_map() {
  // get the max width and height of a window.
  int max_x = 0;
  int max_y = 0;
  getmaxyx(stdscr, max_x, max_y);

  // loops through pixels and add ' '
  for (int x = 1; x < max_x; x++) {
    for (int y = 0; y < max_y; y++) {
      move(x, y);
      addch(' ');                               
    }
  }
  // refresh the window
  refresh();
}


/* ******** initialize_curses *******
 * initialize a curses with ncruses library funtions
 */
void initialize_curses(){
  // initialize the screen
  initscr();
  raw();
  noecho(); 
  // make cursor not visible
  curs_set(0);
  refresh();
  sleep(2);
}


/* ******** clear_message *******
 * clear previous message before sending a new mesasge
 */
void clear_message(){
  int max_x = 0;
  int max_y = 0;

  getmaxyx(stdscr, max_x, max_y);
  // to avoid gcc warning
  (void)max_x;

  // clear status messages
  int y = 0;
  for (y = 0; y < max_y; y++) {
    char letter = mvinch(0, y) & A_CHARTEXT;
    if (letter == '.') {
      y+=1;
      break;
    }
  }
   // get all the columns overlapped with blanks
  if (y != 0) {
    while(y < max_y) {
      move(0, y++);
      addch(' ');
    }
  }
  refresh();
}


/* ******** handle_input *******
 * read a character input from a user and send it to the server.
 *
 * return true if it has an error
 *        false if it's successfully sent
 */
bool handle_input(void* arg){
  // void *arg= &server
  addr_t* server = arg;

  // validate arguments
  if (!message_isAddr(*server)) {
    fprintf(stderr, "handle_input : invalid address.\n");
    return true;
  }
  if (server == NULL) {
    fprintf(stderr, "handle_input : argument shouldn't be NULL\n");
    return true;
  }

  char input = getch();
  char message[strlen("KEY ") + 1 + 1];

  strcpy(message, "KEY ");
  strcat(message, &input);
  message[strlen("KEY ") + 1] = '\0';

  // clear previous message
  clear_message();
  message_send(*server, message);

  return false;
}


/* ******** check_ok_message *******
 * check the ok message from the server and then intialize a curses and map.
 *
 * return true if the ok message is suceessfully received and initialize a curses and map
 *        false if the message("OK playerID") is not received."OK playerID"
 */
bool check_ok_message(const char* message){
  if (message != NULL) {
    if (sscanf(message, "OK %c", &data->player) != 1) {
      return false;
    }
  }
  initialize_curses();
  initialize_map();
  return true;
}

/* ******** resize_grid *******
 * check if a curses window is at least the required size
 *
 * return true if the ok message is suceessfully received and grid is enough size for a grid
 *        false if the message("GRID %d %d") is invalid
 */
bool resize_grid(const char* message){
  int row;
  int col;

  if (sscanf(message, "GRID %d %d", &row, &col) != 2) {
    fprintf(stderr, "Invalid GRID message\n");
    return false;
  }

  int max_x=0;
  int max_y=0;
  getmaxyx(stdscr, max_x, max_y);

  data->rows=max_x;
  data->cols=max_y;

  while (row + 1 > data->rows || col + 1 > data->cols) {
    endwin();
    mvprintw(0,0, "ERROR: incompatible screen size [%d, %d] for grid [%d, %d]\nRESIZE to continue", data->rows, data->cols, row+1, col);
    
    char enter = getch();

    getmaxyx(stdscr, max_x, max_y);
    data->rows=max_x;
    data->cols=max_y;

    if (enter == '\n') {
      endwin();
      initialize_curses();
      refresh();
    }
  }
  data->rows = row+1;
  data->cols = col;

  return true;
}


/* ********** display_map ***********
 * convert chars from display to curses 
*/

void display_map(const char* message){
  char* display="display";
  size_t display_message_length = strlen(message) - strlen(display) + 1;

  char display_message[display_message_length]; 

  strncpy(display_message, message + strlen(display), display_message_length);
  display_message[display_message_length] = '\0';
  
  char *map = malloc(strlen(message)+1);
  strcpy(map, &message[7]);

  for (int x = 0; x < data->rows; x++) {
    for (int y = 0; y < data->cols+1; y++) {
      //curses, +1
      move(x+1,y);  

      if((x * (data->cols+1) + y)>=strlen(map)) {
        // otherwise, fill up with blank
        addch(' ');                             
      }
      else {                
        addch(map[x * (data->cols+1) + y]); 
      }
    }
  }
  refresh(); 
}


/* ********** display_gold_message ***********
 * scan the gold message from the server and prints in the message on the curses
 */
bool display_gold_message(const char* message){

  int collected_gold;
  int gold_purse;
  int unclaimed_gold;

  if (sscanf(message, "GOLD %d %d %d", &collected_gold, &gold_purse, &unclaimed_gold) != 3) {
    fprintf(stderr, "Invalid gold message.\n");
    return false;
  } 

  // display info message
  int max_x = 0;
  int max_y = 0;
  getmaxyx(stdscr, max_x, max_y);

  // for warning msg : unsed variable
  (void)max_x;

  for (int y = 0; y < max_y; y++) {
    move(0, y);
    addch(' ');
  }
  char display_message[max_y];

  if (data->player == 0) {
    snprintf(display_message, max_y, "Spectator: %d nuggets unclaimed yet.", unclaimed_gold);
  } 
  else {
    int message_length = snprintf(display_message, max_y, "%c has %d nuggets, %d nuggets unclaimed.", data->player, gold_purse, unclaimed_gold);
    
    char unclaimend_gold_message[max_y];
    int unclaimend_gold_message_length=-1;
    
    if (collected_gold > 0) {
      if (max_y - message_length > 0) {
        unclaimend_gold_message_length = snprintf(unclaimend_gold_message, max_y - unclaimend_gold_message_length, "  GOLD recieved: %d", collected_gold);
      }
    }

    for (int i = message_length; i < message_length + unclaimend_gold_message_length; i++) {
      display_message[i] = unclaimend_gold_message[i-message_length];
    }
    display_message[message_length + unclaimend_gold_message_length] = '\0';
  }

  mvprintw(0,0,"%s", display_message);
  
  refresh();
  return true;
}



/**************** handleMessage ****************/
/* Data received; print it.
 * 'arg' is not used for the fuction. (ignored args)
 * Return true if any fatal error.
 */
bool handle_message(void* arg,const addr_t from, const char* message) {
  if (message == NULL || arg == NULL) {
    return false;
  }
 
  if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {
    // check if a window has enough size for the grid
    if(!resize_grid(message)){  
      message_send(from, "KEY Q");
      message_done();

      free(data);
      fprintf(stderr, "failed sending a grid message");
      exit(1);
    }
    return false;
  }
  else if (strncmp(message, "OK ", strlen("OK ")) == 0){
    if(!check_ok_message(message)) {
      message_send(from, "KEY Q");
      message_done();

      free(data);
      fprintf(stderr, "failed sending ok message to the server");
      exit(1);
    }
  } 
  else if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    display_map(message);
    return false;
  } 
  else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    display_gold_message(message);
    return false;
  }
  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    endwin();
    // clean existing stdout prints
    fflush(stdout);
    fprintf(stdout, "\n%s\n", message);
    return true;
  } 
  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    // make mesasge loop stop
    return false;
  } 
  else {
    // make mesasge loop stop
    fprintf(stderr, "ERROR Malformed message '%s'\n", message);
    return false;
  }
  return false;
}


/* 
 *  main function
 * ./clients hostName port [playerName]
 *  client interacts with server to play the nugget game by sending keystrokes.
 */
int main(int argc, char *argv[]){

  const char* hostName = argv[1];
  const char* port = argv[2];
  const char* playerName = argv[3];

  // address of the server
  addr_t server; 
  
  data= client_data_new();

  // parse and validate arguments
  if (argc < 3 || argc > 4) {
      fprintf(stderr, "USAGE: ./client hostName port [playerName]\n");
      exit(1);
  }

  for (int i = 1; i < argc; i++) {
      if (argv[i] == NULL) {
          fprintf(stderr, "Argmument(s) shouldn't be NULL.\n");
          exit(1);
      }
  }

  // initialize message module
  // message_init => return portnumber or 0 if errors.
  int msg_init=message_init(NULL);
  if (msg_init == 0) {
    free(data);
    fprintf(stderr, "failed to initialize message module.\n");
    exit(1);
  }
  
  // initialize address using hostName and port
  bool msg_setAdd=message_setAddr(hostName, port, &server);
  if (!msg_setAdd) {
      fprintf(stderr, "A bad hostname or port number.\n");
      free(data);
      exit(1);
  }

  // connect to server
  if (playerName != NULL) {
    char message[message_MaxBytes];
    strcpy(message, "PLAY ");
    strncat(message, playerName, message_MaxBytes-strlen("PLAY "));

    // connect as player
    message_send(server, message);
  }
  else{
    // connect as a spectator
    message_send(server, "SPECTATE");

    // if ok message is not sent
    if(!check_ok_message(NULL)) {
      message_send(server, "KEY Q");
      message_done();
      free(data);
      fprintf(stderr, "failed to initialize player name");
      exit(1);
    }

  }

  // loop, handling input and incoming messages.
  // true, in the normal case when the loop ends due to handler return true;
  // false, when fatal errors indicate we cannot keep looping.
  // 'arg': carry a pointer to 'server'.
  bool check_message = message_loop(&server, 0, NULL, handle_input, handle_message);

  free(data);
  endwin();
  message_done();

  if(!check_message){
    return 1;
  }
  return 0;
}
