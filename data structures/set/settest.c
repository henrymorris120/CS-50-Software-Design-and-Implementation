/* 
 * settest.c - test program for CS50 set module
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

// prototype functions
static void setpairprint(FILE* fp, const char* key, void* item);
static void itemdelete(void* item);
static void itemcount(void* arg, const char* key, void* item);

/* **************************************** */
int main() 
{
    // create new set and check no problem allocating memory to do this
    set_t* set = set_new();
    if (set == NULL) {
        fprintf(stderr, "counters_new failed for ctrs\n");
        return 1;
    }

    char* ptr1 = "door";
    char* ptr2 = "mat";
    if (!(set_insert(set, ptr1, ptr2))) {
        fprintf(stderr, "set_insert failed\n");
        return 1;
    }
    
    char* ptr3 = "hello";
    char* ptr4 = "goodbye";
    if (!(set_insert(set, ptr3, ptr4))) {
        fprintf(stderr, "set_insert failed\n");
        return 1;
    }

    char* ptr5 = "day";
    char* ptr6 = "night";
     if (!(set_insert(set, ptr5, ptr6))) {
        fprintf(stderr, "set_insert failed\n");
        return 1;
    }

    // expect 3 to be printed
    int num = 0;
    set_iterate(set, &num, itemcount);
    fprintf(stdout,"%d\n", num);

    // expect "night" to be printed
    char* word = set_find(set, ptr5);
    fprintf(stdout,"%s\n", word);
    
    // expect the pointer word to be NULL
    char* failure = set_find(set, NULL);
    if (failure == NULL) {
        fprintf(stdout,"Correctly dealt with a non existant key\n");
    }

    // expect no insert because that key already used
    char* ptr7 = "day";
    char* ptr8 = "wtv";
    bool noGood = set_insert(set, ptr7, ptr8);
    if (noGood == false) {
        fprintf(stdout,"Correctly dealt with a key already used\n");
    }



    // should print {day:night,hello:goodbye,door:mat}
    set_print(set, stdout, setpairprint);

    // delete set, freeing memory
    set_delete(set,itemdelete);


    return 0;
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



// delete an item
void itemdelete(void* item)
{
  if (item != NULL) {
    free(item);
  }
}

// count the set pairs
static void itemcount(void* arg, const char* key, void* item)
{
  int* pairs = arg;
  if (pairs != NULL && key != NULL && item != NULL) {
    (*pairs)++;
  }
}
