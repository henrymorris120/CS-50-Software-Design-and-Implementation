/* 
 * pagedir.c - CS50 pagedir module
 *
 * see pagedir.h for more information.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/*
 * see pagedir.h for description
*/
bool pagedir_init(const char* pageDirectory) 
{
  FILE* file;
  char* path = mem_malloc_assert(strlen(pageDirectory) + strlen("/.crawler") + 1, "Memory is Full");
  sprintf(path, "%s/%s", pageDirectory, ".crawler");
  file = fopen(path, "r");
  bool valid = file != NULL;
  if (valid) fclose(file);
  mem_free(path);
  return valid;
}

/**************** pagedir_save ****************/
/*
 * see pagedir.h for description
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) 
{
    char* pathname;
    int required_length = snprintf(NULL, 0, "%s/%d", pageDirectory, docID);
    pathname = mem_malloc_assert(required_length + 1, "Memory is full");
    sprintf(pathname, "%s/%d", pageDirectory, docID);
    FILE* file;
    // check if can read file and run fprintf into file the URL, depth, and HTML if so
    if ((file = fopen(pathname, "w")) != NULL) {
        fprintf(file,"%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
        fclose(file);
    } else {
        fprintf(stderr, "Error: Unable to write for file %p", (void *)file);
    }
    free(pathname);
}

/**************** getWebpageFromDirectoryID ****************/
/*
 * see pagedir.h for description
*/
webpage_t* getWebpageFromDirectoryID(char* pageDirectory, const int docID) 
{
    char* pathname;
    int required_length = snprintf(NULL, 0, "%s/%d", pageDirectory, docID);
    pathname = mem_malloc_assert(required_length + 1, "Memory is full");
    sprintf(pathname, "%s/%d", pageDirectory, docID);
    FILE* file;
    file = fopen(pathname, "r");
    mem_free(pathname);
    if (file != NULL) {
        webpage_t* webpage;
        char* URL = file_readLine(file);
        char* depth_char = file_readLine(file);
        char* html = file_readFile(file);
        // Convert the string for depth to an integer base 10 and check for error
        char* endptr;
        int depth = strtol(depth_char, &endptr, 10); 
        if (*endptr == '\0') {
            webpage = webpage_new(URL,depth,html);
        } else {
            webpage = NULL;
        }
        free(depth_char);
        fclose(file);
        return webpage;
    }
    return NULL;
}
