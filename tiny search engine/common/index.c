/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "word.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct index {
  hashtable_t* ht;      
} index_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see index.h for comments about exported functions */


/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new(const int num_slots)
{
    if (num_slots > 0) {
        index_t* index = mem_malloc_assert(sizeof(index_t),"Memory is full");
        index->ht = hashtable_new(num_slots);
        return index;
    } else {
        return NULL;
    }
}

/**************** delete_helper() ****************/
/* needed for imcompatible of types */
void delete_helper(void *counter)
{
    if(counter != NULL) {
       counters_delete(counter);
    }
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, const char* word, const int* docID)
{
    if (index != NULL && word != NULL && *docID > 0) {
         // find counter for word
        counters_t *counter = hashtable_find(index->ht, word);
        // check if counter exists already or not
        if (counter != NULL) { 
            // Increment
            counters_add(counter, *docID);
        } else {
            // create counter
            counters_t* new_counter = counters_new();
            counters_add(new_counter, *docID);
            return hashtable_insert(index->ht, word, new_counter);
        }
    }
    return false;
}

/**************** index_set() ****************/
/* see index.h for description */
bool index_set(index_t* index, const char* word, const int* docID, int count)
{
    if (index != NULL && word != NULL && *docID > 0 && count >= 0) {
         // find counter for word
        counters_t *counter = hashtable_find(index->ht, word);
        // check if counter exists already or not
        if (counter != NULL) { 
            // Increment
            counters_set(counter, *docID, count);
        } else {
            // create counter
            counters_t* new_counter = counters_new();
            counters_set(new_counter, *docID, count);
            return hashtable_insert(index->ht, word, new_counter);
        }
    }
    return false;
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t* index_find(index_t* index, const char* word)
{
    counters_t* pair = hashtable_find(index->ht, word);
    return pair;
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index)
{
    if (index != NULL) {
        hashtable_delete(index->ht, delete_helper);
        // hashtable_delete(index->ht, NULL);

        mem_free(index);
    }
}

/**************** index_read() ****************/
/* see index.h for description */
index_t* index_read(FILE* file)
{
    if (file != NULL) {
      int lines = file_numLines(file); 
      // load factor of 0.7, rounding up
      int slots = (int)ceil(0.7 * lines);
      index_t* index = index_new(slots);
      char* word;
      int docID;
      int count;
      while ((word = file_readWord(file)) != NULL) {
        while (fscanf(file, "%d %d", &docID, &count) == 2) {
           index_set(index, normalizeWord(word), &docID, count); 
        }
        mem_free(word);
      }
      return index;
    } else {
        return NULL;
    }
}

/**************** index_write_helper2() ****************/
/* see index.h for description */
void index_write_helper2(void* arg, const int key, const int count) {
    FILE* file = arg;
    fprintf(file, " %d %d", key, count);
}

/**************** index_write_helper() ****************/
/* see index.h for description */
void index_write_helper(void* arg, const char* word, void* item)
{
    // cast as file and counter respectively
    FILE* file = arg;
    counters_t* counter = item;
    fprintf(file, "%s", word);
    counters_iterate(counter, file,(*index_write_helper2));
    fprintf(file, "\n");
}

/**************** index_write() ****************/
/* see index.h for description */
bool index_write(index_t* index, FILE* file)
{
    if (file != NULL && index != NULL) {
        hashtable_iterate(index->ht, file, (*index_write_helper));
        return true;
    } else {
        return false;
    }
}
