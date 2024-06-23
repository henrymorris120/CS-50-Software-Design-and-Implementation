/* 
 * index.h - header file for CS50 index module in TSE
 *
 * An index is a hashtable mapping from words to (
 * documentID, count) pairs, wherein each count represents 
 * the number of occurrences of the given word in the given document
 *
 * Henry Morris Fall 2023 
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/counters.h"

/**************** global types ****************/
typedef struct index index_t;  // opaque to users of the module

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   data is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* index_new(const int num_slots);

/**************** delete_helper() ****************/
/* needed for imcompatible of types */
void delete_helper(void *counter);


/**************** index_insert() ****************/
/*
 *Insert word into the given index, creater a new counter if necessary
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid pointer for docID.
 * We return:
 *   false if there was error/invalid inputs;
 *   true iff word was inserted. 
 */
bool index_insert(index_t* index, const char* word, const int* docID);

/**************** index_set() ****************/
/*
 * Sets the current value of counter associated with gived docID for given word.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   docID (must be > 0), 
 *   counter value (must be >= 0).
 * We return:
 *   false if ctrs is NULL, if docID < 1 or count < 0, or if out of memory.
 *   otherwise returns true.
 * We do:
 *   If the word docID pairing does not yet exist, create a counter for it and initialize to
 *   the given value, otherwise just set the count to count
*/

bool index_set(index_t* index, const char* word, const int* docID, int count);

/**************** index_find() ****************/
/* Return the counter associated with the given word.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word.
 * We return:
 *   pointer to the counter corresponding to the given word, if found;
 *   NULL if index is NULL, word is NULL, or word is not found.
 */

counters_t* index_find(index_t* index, const char* word);

/**************** index_delete() ****************/
/* Delete index, calling a delete function on everything.
 *
 * Caller provides:
 *   valid index pointer,
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, calls delete_helper on each item.
 *   free all the key strings and counters and the index itself itself.
 */
void index_delete(index_t* index);

/**************** index_read() ****************/
/*
 * read the file and creates index based on what is written in the file
 * returns null if file is null or no memory to allocate space for new index
 */
index_t* index_read(FILE* file);

/**************** index_write_helper2() ****************/
/*
 * helper function for index_write for iterating counter
 */
void index_write_helper2(void* arg, const int key, const int count);

/**************** index_write_helper() ****************/
/*
 * helper function for index_write for iterating a hashtable
 */
void index_write_helper(void* arg, const char* word, void* item);

/**************** index_write() ****************/
/*
 * writes out in the specified format the given index
 * onto the file
 * return false if cannot write onto file or index is NULL
 * else return true
 */
bool index_write(index_t* index, FILE* file);

#endif // __INDEX_H
