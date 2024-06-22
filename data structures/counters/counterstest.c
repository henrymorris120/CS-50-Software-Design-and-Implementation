/* 
 * counterstest.c - test program for CS50 counters module
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"

// prototype function
static void itemcount(void* arg, const int key, const int count);

/* **************************************** */
int main() 
{
    // create new set counter and check no problem allocating memory to do this
    counters_t* ctrs = counters_new();
    if (ctrs == NULL) {
        fprintf(stderr, "counters_new failed for ctrs\n");
        return 1;
    }
    // add three new keys
    counters_add(ctrs, 100);
    counters_add(ctrs, 200);
    counters_add(ctrs, 300);
    
    // increment existing keys
    counters_add(ctrs, 200);
    counters_add(ctrs, 300);
    counters_add(ctrs, 300);

    int num = counters_get(ctrs, 300);
    // expect 3 to be printed
    fprintf(stdout,"%d\n", num);
    
    counters_set(ctrs, 300, 10);
    num = counters_get(ctrs, 300);
    // expect 10 to be printed
    fprintf(stdout,"%d\n", num);

    num = counters_get(ctrs,20);
    // expect 0 to be printed because no key equals 20
    fprintf(stdout,"%d\n", num);

    // should do nothing because invalid key
    counters_add(ctrs, -20);

    // should print 3
    num = 0;
    counters_iterate(ctrs, &num, itemcount);
    fprintf(stdout,"%d\n", num);
    
    // should print {100:1,200:2,300:10}
    counters_print(ctrs, stdout);

    // delete ctrs, freeing memory
    counters_delete(ctrs);

    return 0;
}


// count the counter pairs
static void itemcount(void* arg, const int key, const int count)
{
  int* pairs = arg;
  if (pairs != NULL && key >=0 && count >= 0) {
    (*pairs)++;
  }
}
