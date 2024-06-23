/* 
 * pagedir.h - header file for the pagedir module
 * which has an initialize and save local function
 * Henry Morris Fall 2023
 */

#ifndef __SET_H
#define __SET_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/*
 * input is the pointer to a pageDirectory
 * output is true or false depending on if it exists
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/*
 * input is the name of the directory, the webpage, and the docID
 * output is just saving
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** getWebpageFromDirectoryID ****************/
/*
 * input is the name of the directory and the docID
 * output is just a pointer to webpage struct
 * Assumes valid arguments
*/
webpage_t* getWebpageFromDirectoryID(char* pageDirectory, const int docID);



#endif // __SET_H
