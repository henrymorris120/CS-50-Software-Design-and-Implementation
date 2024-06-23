/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see word.h for comments about exported functions */

/**************** normalizeWord() ****************/
/* see word.h for description */

char* normalizeWord(char* word)
{
    if (word != NULL) {
        for (char *p = word; *p != '\0'; p++) {
            *p = tolower(*p);
        }
   }
   return word; 
}
