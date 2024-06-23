/* 
 * word.h - header file for CS50 word module
 * used in TSE to normalize a word
 *
 * Henry Morris Fall 2023
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdbool.h>

/**************** functions ****************/

/**************** normalizeWord() ****************/
/* 
 * changes the pointers to each letter in the string to their
 * lowercase version and returns the pointer to first letter in string
 */
char* normalizeWord(char* word);


#endif // __WORD_H
