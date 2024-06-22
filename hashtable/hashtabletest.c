/* 
 * hashtabletest.c - test program for CS50 hashtable module
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// prototype functions
static void itemdelete(void* item);
static void itemcount(void* arg, const char* key, void* item);
static void setpairprint(FILE* fp, const char* key, void* item);

/* **************************************** */
int main() 
{
    // check that we cant create a hashtable with a negative number of slots
    hashtable_t* fail = hashtable_new(-10);
    if (fail == NULL) {
        fprintf(stdout, "hashtable_new failed as expected because of negative argument\n");
    } else {
        fprintf(stderr, "error: created hashtable when we shouldn't have\n");
        return 1;
    }

    // create hash table with 2 slots and check no problem allocating memory to do this
    hashtable_t* hashtable = hashtable_new(2);
    if (hashtable == NULL) {
        fprintf(stderr, "hashtable_new failed\n");
        return 1;
    }

    char* ptr1 = "key number 1";
    char* ptr2 = "2";
    char* ptr3 = "third time the charm";
    if (!hashtable_insert(hashtable, ptr1, "#1")) {
        fprintf(stderr, "error in hashtable_insert\n");
        return 1;
    }
    if (!hashtable_insert(hashtable, ptr2, "#2")) {
        fprintf(stderr, "error in hashtable_insert\n");
        return 1;
    }
    if (!hashtable_insert(hashtable, ptr3, "#3")) {
        fprintf(stderr, "error in hashtable_insert\n");
        return 1;
    }

    // should print out "#3"
    char* value = hashtable_find(hashtable, ptr3);
    fprintf(stdout, "%s\n", value);

    // "output is as expected" should be printed her
    char* failed = "not here";
    if (hashtable_find(hashtable, failed) == NULL) {
        fprintf(stdout, "output is as expected\n");
    }

    // should print out 3 as we only inserted 3 key item pairs
    int num = 0;
    hashtable_iterate(hashtable, &num, itemcount);
    fprintf(stdout,"%d\n", num);

    // print out the hashtable which has 2 slots and 3 key-item pair
    hashtable_print(hashtable, stdout, setpairprint);

    // delete the hashtable, freeing memory
    hashtable_delete(hashtable,itemdelete);

    return 0;
}


// delete an item
void itemdelete(void* item)
{
  if (item != NULL) {
    free(item);   
  }
}

// count the key-item pairs
static void itemcount(void* arg, const char* key, void* item)
{
  int* pairs = arg;
  if (pairs != NULL && key != NULL && item != NULL) {
    (*pairs)++;
  }
}


// print setpair
void setpairprint(FILE* fp, const char* key, void* item)
{
  char* name = item; 
  if (name != NULL && key != NULL) {
    fprintf(fp, "%s:%s", key, name);
    
  } else {
    fprintf(fp, "(null)");
  }
}
