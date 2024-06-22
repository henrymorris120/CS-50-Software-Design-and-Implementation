/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setpair {
  void* item;                 // pointer to data for this key
  char* key;                  // pointer to key for this setpair
  struct setpair *next;       // link to next setpair
} setpair_t;

/**************** global types ****************/
typedef struct set {
  struct setpair *head;       // head of the list of setpairs in the set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setpair_t* setpair_new(char* key, void* item);

/**************** set_new() ****************/
/* see set.h for description */
set_t* set_new(void)
{
  set_t* set = malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL;              // error allocating set
  } else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
/* see set.h for description */
bool set_insert(set_t* set, const char* key, void* item)
{
    if (set != NULL && key != NULL && item != NULL) {
        // check if key has not been used
        if (set_find(set, key) == NULL) {
            // copy key
            char* keyCopy = malloc(strlen(key) + 1);
            if (keyCopy == NULL) {
                return false;
            } else {
                strcpy(keyCopy, key);
                // create the new pair
                setpair_t* new = setpair_new(keyCopy,item);
                // add to head of set list is no allocation error
                if (new != NULL) {
                    new->next = set->head;
                    set->head = new;
                    return true;
                } else {
                    free(keyCopy);
                }
            }
        }
    }
    return false;
}

/**************** setpair_new ****************/
/* Allocate and initialize a setpair */
// not visible outside this file
static setpair_t* setpair_new(char* key, void* item)
{
  setpair_t* spair = malloc(sizeof(setpair_t));

  if (spair == NULL) {
    // error allocating memory for spair; return error
    return NULL;
  } else {
    spair->item = item;
    spair->key = key;
    spair->next = NULL;
    return spair;
  }
}

/**************** set_find() ****************/
/* see set.h for description */
void* set_find(set_t* set, const char* key)
{
    if (set != NULL && key != NULL) {
        for (setpair_t* spair = set->head; spair != NULL; spair = spair->next) {
            if (strcmp(spair->key, key) == 0) {
                return spair->item;
            }
        }
    }
    return NULL;
}

/**************** set_print() ****************/
/* see set.h for description */
void set_print(set_t* set, FILE* fp, 
               void (*itemprint)(FILE* fp, const char* key, void* item) )
{
    if (fp != NULL) {
        if (set != NULL) {
            fputc('{', fp);
            for (setpair_t* spair = set->head; spair != NULL; spair = spair->next) {
                // print this key-item pair
                if (itemprint != NULL) { 
                    (*itemprint)(fp, spair->key, spair->item); 
                    // check if not last pair, in which case add comma
                    if (spair->next != NULL) {
                        fputc(',', fp);
                    }
                }
            }
            fputc('}', fp);
        } else {
            fputs("(null)", fp);
        }
    }
}

/**************** set_iterate() ****************/
/* see set.h for description */
void set_iterate(set_t* set, void* arg,
                 void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (set != NULL && itemfunc != NULL) {
        //call the itemfunc on each item, with (arg, key, item)
        for (setpair_t* spair = set->head; spair != NULL; spair = spair->next) {
            (*itemfunc)(arg, spair->key, spair->item);
        }
    }
}

/**************** set_delete() ****************/
/* see set.h for description */
void set_delete(set_t* set, void (*itemdelete)(void* item) )
{
    if (set != NULL) {
        for (setpair_t* spair = set->head; spair != NULL;) {
            if (itemdelete != NULL) {   // if possible...
                (*itemdelete)(spair->key);      // delete node's key
            }
            setpair_t* next = spair->next;     // remember what comes next
            free(spair);                        // free the node
            spair = next;                      // and move on to next
        }
        free(set);
    }
}
