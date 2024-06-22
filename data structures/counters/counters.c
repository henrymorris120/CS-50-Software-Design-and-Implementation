/* 
 * counters.c - CS50 'set' module
 *
 * see counters.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "counters.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct countpair {
  int count;                   // pointer to count for this key
  int key;                     // pointer to key for this countpair
  struct countpair *next;      // link to next countpair
} countpair_t;

/**************** global types ****************/
typedef struct counters {
  struct countpair *head;       // head of the list of countpairs
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see count.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static countpair_t* countpair_new(const int key);

/**************** counters_new() ****************/
/* see counters.h for description */
counters_t* counters_new(void) {
    counters_t* counters = malloc(sizeof(counters_t));
    if (counters == NULL) {
        return NULL;              // error allocating counters
    } else {
        // initialize contents of counters structure
        counters->head = NULL;
        return counters;
  }
}

/**************** counters_add() ****************/
/* see counters.h for description */
int counters_add(counters_t* ctrs, const int key) {
    if (ctrs != NULL && key >= 0) {
        // check if key exists
        for (countpair_t* cpair = ctrs->head; cpair != NULL; cpair = cpair-> next) {
            if (cpair->key == key) {
                cpair->count++;
                return cpair->count;
            }
        }
        // key does not exist already so create new count pair and add to head of list
        countpair_t* new = countpair_new(key);
        if (new != NULL) {
            new->next = ctrs->head;
            ctrs->head = new;
            return new->count;
        }
    }
    // error occurred
    return 0;
}

/**************** countpair_new ****************/
/* Allocate and initialize a countpair */
// not visible outside this file
static countpair_t* countpair_new(const int key)
{
  countpair_t* cpair = malloc(sizeof(countpair_t));

  if (cpair == NULL) {
    // error allocating memory for cpair; return error
    return NULL;
  } else {
    cpair->key = key;
    cpair->count = 1;
    cpair->next = NULL;
    return cpair;
  }
}

/**************** counters_get() ****************/
/* see counters.h for description */
int counters_get(counters_t* ctrs, const int key)
{
    if (ctrs != NULL && key >= 0) {
        // check if key exists, if so return count
        for (countpair_t* cpair = ctrs->head; cpair != NULL; cpair = cpair-> next) {
            if (cpair->key == key) {
                return cpair->count;
            }
        } 
    }
    return 0;
}

/**************** counters_set() ****************/
/* see counters.h for description */
bool counters_set(counters_t* ctrs, const int key, const int count)
{
    if (ctrs != NULL && key >= 0 && count >=0) {
        // check if key exists
        for (countpair_t* cpair = ctrs->head; cpair != NULL; cpair = cpair-> next) {
            if (cpair->key == key) {
                // set count
                cpair->count = count;
                return true;
            }
        }
        // key does not exist so create new countpair and recursively call function again
        if (counters_add(ctrs, key) == 1) {
            return counters_set(ctrs, key, count);
        }
    }
    return false;
}

/**************** counters_print() ****************/
/* see counters.h for description */
void counters_print(counters_t* ctrs, FILE* fp)
{
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);
            for (countpair_t* cpair = ctrs->head; cpair != NULL; cpair = cpair->next) {
                // print this key-item pair
                fprintf(fp, "%d=%d", cpair->key, cpair->count);
                if (cpair->next != NULL) {
                    fputc(',', fp);
                }
            }
            fputc('}', fp);
        } else {
            fputs("(null)", fp);
        }
    }
}

/**************** counters_iterate() ****************/
/* see counters.h for description */
void counters_iterate(counters_t* ctrs, void* arg, 
                      void (*itemfunc)(void* arg, 
                                       const int key, const int count))
{
    if (ctrs != NULL && itemfunc != NULL) {
        // call itemfunc once for each item, with (arg, key, count)
        for (countpair_t* cpair = ctrs->head; cpair != NULL; cpair = cpair->next) {
            (*itemfunc)(arg, cpair->key, cpair->count);
        }
    }
}

/**************** counters_delete() ****************/
/* see counters.h for description */
void counters_delete(counters_t* ctrs)
{
     if (ctrs != NULL) {
        for (countpair_t* cpair = ctrs->head; cpair != NULL;) {
            countpair_t* next = cpair->next;     // remember what comes next
            free(cpair);                        // free the node
            cpair = next;                      // and move on to next
        }
        free(ctrs);
    }
}


