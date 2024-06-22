/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"
#include "hash.h"
#include "../set/set.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
  int num_slots;            // number of slots in the hashtable
  struct set** table;       // array of pointers to sets
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hastable.h for comments about exported functions */


/**************** hashtable_new() ****************/
/* see hashtable.h for description */
hashtable_t* hashtable_new(const int num_slots)
{
    if (num_slots > 0) {
        hashtable_t* hashtable = malloc(sizeof(hashtable_t));
        if (hashtable == NULL) {
            return NULL;              // error allocating hashtable
        } else {
            // initialize contents of hashtable structure
            hashtable->num_slots = num_slots;
            hashtable-> table = calloc(num_slots, sizeof(struct set*));
            if (hashtable-> table == NULL) {
                return NULL;    // error allocating the array
            } else {
                for (int i=0; i < num_slots; i++) {
                    hashtable->table[i] = set_new();
                    if (hashtable->table[i] == NULL) {
                        return NULL; // error allocating the set
                    }
                }
                return hashtable;
            }
        }
    }
    return NULL;
}

/**************** hashtable_insert() ****************/
/* see hashtable.h for description */
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
    if (ht != NULL && key != NULL && item != NULL) {
        // hash value
        int hashNum = (int)hash_jenkins(key, ht->num_slots);
        // insert into correct slot
        // notice copy value as we copy in set_insert()
        return set_insert(ht->table[hashNum], key, item);
    }
    return false;
}

/**************** hashtable_find() ****************/
/* see hashtable.h for description */
void* hashtable_find(hashtable_t* ht, const char* key)
{
    if (ht != NULL && key != NULL) {
        int hashNum = (int)hash_jenkins(key, ht->num_slots);
        // search in correct slot
        return set_find(ht->table[hashNum],key);
    }
    return NULL;
}

/**************** hashtable_print() ****************/
/* see hashtable.h for description */
void hashtable_print(hashtable_t* ht, FILE* fp, 
                     void (*itemprint)(FILE* fp, const char* key, void* item))
{
    if (fp != NULL) {
        if (ht != NULL) {
            for(int i =0; i<ht->num_slots; i++) {
                // one line per hash slot
                set_print(ht->table[i], fp, itemprint);
                // new line if not iterating on final slot
                if(i != ht->num_slots - 1) {
                    fputs("\n", fp);
                }
            }
        } else {
            fputs("(null)", fp);
        }
    }
}

/**************** hashtable_iterate() ****************/
/* see hashtable.h for description */
void hashtable_iterate(hashtable_t* ht, void* arg,
                       void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (ht != NULL && itemfunc != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            set_iterate(ht->table[i], arg, itemfunc);
        }
    }
}

/**************** hashtable_delete() ****************/
/* see hashtable.h for description */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) )
{
    if (ht != NULL) {
        // iterate and free each set
        for (int i = 0; i < ht->num_slots; i++) {
            set_delete(ht->table[i], itemdelete);
        }
        free(ht->table);
        free(ht);
    }
}


