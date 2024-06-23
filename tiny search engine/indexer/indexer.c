/* 
 * indexer.c - indexer module
 * implementation follows the Requirement and Design Specs, and follows
 * the Implementation Spec
 *
 * Henry Morris Fall 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../libcs50/webpage.h"
#include"../common/word.h"
#include "../libcs50/mem.h"
#include "../common/pagedir.h"
#include "../libcs50/counters.h"


/**************** file-local global variables ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* webpage, int docID);

/*
 * The `main` function simply calls `parseArgs`,`indexBuild`, and `index_write`.
 * It then deletes the index it built and frees pageDirectory and indexFilename
 * before then returning 0.
*/
int main(const int argc, char* argv[]) 
{
    // create pointer to values of the arguments and store in memory
    char** pageDirectory = mem_malloc_assert(sizeof(char*), "Memory is full");
    char** indexFilename = mem_malloc_assert(sizeof(char*), "Memory is full");
    // parse arguments
    parseArgs(argc, argv, indexFilename, pageDirectory);
    // build index
    index_t* index = indexBuild(*pageDirectory);
    // write index
     FILE* fp = fopen(*indexFilename, "w");

    index_write(index,fp);
    fclose(fp);
    // delete and free
    
    index_delete(index);
    mem_free(pageDirectory);
    mem_free(indexFilename);
    return 0;
}

/*
 * Given arguments from the command line, extract them into the function parameters; 
 * return only if successful.
 * for pageDirectory, make sure that the directory is accessible and is the pathname for a directory produced by the Crawler
 * for indexFilename, make sure that the file exists and can be written into
 * if any trouble is found, print an error to stderr and exit non-zero.
*/
static void parseArgs(const int argc, char* argv[], char** indexFilename, char** pageDirectory)
{
     // check if correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Error: incorrect number of arguments\n");
        exit(1);
    }

    *pageDirectory = argv[1];
    *indexFilename = argv[2];

    // check if valid page directory
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: invalid page directory\n");
        exit(2);
    }

   // check if valid filename
    FILE* check;
    if ((check = fopen(*indexFilename, "w")) == NULL) {
        fprintf(stderr, "Error: invalid index filename\n");
        exit(3);
    } else {
        fclose(check);
    }
}


/*
 * Builds an in-memory _index_ from  from webpage files it finds in the pageDirectory
 * Input is the pointer to a string that is the pageDirectory
 * Output is the index we create based on pageDirectory
*/
static index_t* indexBuild(char* pageDirectory)
{
    // create new index
    index_t* index = index_new(500);
    //loops over document ID numbers, counting from 1
    int docID = 1;
    
    webpage_t* newWebpage = getWebpageFromDirectoryID(pageDirectory, docID);
    // loop over all webpages
    while (newWebpage != NULL) {
        indexPage(index, newWebpage, docID);
        webpage_delete(newWebpage);
        docID++;
        newWebpage = getWebpageFromDirectoryID(pageDirectory, docID);
    }
    return index;
}


/*
 * scans a webpage document to add its words to the index.
 * input is index, webpage, docID
 * output is nothing
*/
static void indexPage(index_t* index, webpage_t* webpage, int docID)
{
  //retrieve words 
  int pos = 0;
  char* word;
  while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
    //skips trivial words (less than length 3)
    if (strlen(word) < 3) {
      free(word);
    } else {
        // conver to all lowercase
        normalizeWord(word);
        // insert
        index_insert(index, word, &docID);
        free(word);
    }
  }
}
