/* 
 * querier.c - querier module
 * implementation follows the Requirement and Design Specs, and follows
 * the Implementation Spec
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../common/index.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include"../common/word.h"
#include "../libcs50/counters.h"
#include "../common/pagedir.h"

/**************** file-local global variables ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
static void parseArgs(const int argc, char* argv[], char** indexFilename, char** pageDirectory);
static void query(index_t* index, char* pageDirectory);
static int wordCounter(char* input);
static bool allLetters(char* input);
static char** tokenize(char* input, int numberOfWords);
static bool equalAnd(char* word);
static bool equalOr(char* word);
static counters_t* getScores(int numberOfWords, index_t* index, char** words, int wordsIndex);
static counters_t* addWordAnd(counters_t* ctr, counters_t* counter_new);
static void addWordAndHelper(void* arg, const int key, const int item);
static void addWordOrHelper(void* arg, const int key, const int item);
static void itemcount(void* arg, const int key, const int count);
static void printRankedScores(counters_t* score, char* pageDirectory, int matches);
static void createDocIDsArray(void* arg, const int key, const int item);
static int scoreDescending(const void *a, const void *b);
static char* getLink(char* pageDirectory, int docID);


/********* local structs **********/
/*struct that holds 2 pointers to counters*/
typedef struct twoCtr {
    counters_t* ctr1; 
    counters_t* ctr2;
} twoCtr_t;
/*struct that holds a key item pair*/
typedef struct pair {
    int key; 
    int item;
} pair_t;
/*struct that holds 2 pointers to counters*/
typedef struct indexArray {
    pair_t* IDs; 
    int index;
} indexArray_t;


/*
 * Inputs to main funciton are pointers to pageDirectory and indexFilename
 * Parses args and returns error if invalid arguments
 * if no error lets user query
*/
int main(const int argc, char* argv[]) 
{
     // create pointer to values of the arguments and store in memory
    char** pageDirectory = mem_malloc_assert(sizeof(char*), "Memory is full");
    char** indexFilename = mem_malloc_assert(sizeof(char*), "Memory is full");
    // parse arguments
    parseArgs(argc, argv, indexFilename, pageDirectory); 
    // make index
    FILE* fp = fopen(*indexFilename, "r");
    index_t* index = index_read(fp);
    fclose(fp);
    query(index, *pageDirectory);
    index_delete(index);
    mem_free(pageDirectory);
    mem_free(indexFilename);
    return 0; 
}

/*
 * Given arguments from the command line, extract them into the function parameters; 
 * return only if successful.
 * for pageDirectory, make sure that the directory is accessible and is the pathname for a directory produced by the Crawler
 * for indexFilename, make sure that the file exists and can be read
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
    if ((check = fopen(*indexFilename, "r")) == NULL) {
        fprintf(stderr, "Error: invalid index filename\n");
        exit(3);
    } else {
        fclose(check);
    }
}

/*
 * Performs query following implementation details in requirements.md
*/
static void query(index_t* index, char* pageDirectory)
{
    printf("Query? ");
    char* input;
    // continue loop until the user exits
    while ((input=file_readLine(stdin)) != NULL) {
        // check if input is all valid characters
        if (allLetters(input)) {
            int numberOfWords = wordCounter(input);
            char** words;
            // check if valid tokeization
            if ((words = tokenize(input,numberOfWords)) != NULL) {
              counters_t* score = getScores(numberOfWords, index, words, 0);
              // calculate number of matches
              int matches = 0;
              counters_iterate(score, &matches, itemcount);
              if (matches == 0) {
                printf("No documents match.\n");
              } else {
                printf("Matches %d documents (ranked):\n", matches);
                printRankedScores(score, pageDirectory, matches);
              }
              printf("-----------------------------------------------\n");
              printf("Query? ");
              mem_free(words);
              counters_delete(score);
            } else {
                printf("Query? ");
            }
        } else {
            printf("Query? ");
        }
        mem_free(input);
    }
}

/*
 * returns the number of words in the string that the char pointer input points to
*/
static int wordCounter(char* input) 
{
    int numberOfWords = 0;
    //Loop through string, adding 1 to numberOfWords at the start of each new word
    char* p = input;
    while (*p != '\0') {
        //Check if letter
        if (isalpha(*p)) {
            numberOfWords++;
            p++;
            //Wait for the next space or end of line
            while ((*p != '\0') && (isalpha(*p))) {
                p++;
            }
        } else {
            p++;
        }
    }
    return numberOfWords;
}

/*
 * Check if the string is equal to "or"
 * returns true or false depending on if equal
 */
static bool equalOr(char* word) {
  if (strcmp(word, "or") == 0) {
    return true;
  } else {
    return false;
  }
}

/*
 * Check if the string is equal to "and"
 * return true or false depending on if equal
 */
static bool equalAnd(char* word) {
  if (strcmp(word, "and") == 0) {
    return true;
  } else {
    return false;
  }
}


/*
 * Returns true if the string that input points to contains only letter
 * and has at least one letter, else returns false
*/
static bool allLetters(char* input)
{
    bool empty = true;
    while (*input) {
        if (isalpha(*input) || isspace(*input)) {
            if (isalpha(*input)) {
                empty = false;
            }
            input++;
        }
        else {
            fprintf(stderr, "Error: bad character '%c' in query\n", *input);
            return false;
        }
    }
    if (empty) {
        printf("\n");
        return false;
    }
    return true;
}

/*
 * Inputs are the inputted user line and the number of words in that string
 * Converts a string to an array of words
 * If "and" or "or" are mispalced in the input, print error to
 * stderr and return NULL, else return the pointer to this array of words
 */
static char** tokenize(char* input, int numberOfWords) 
{
  // create words with memory needed
  char** words = mem_malloc_assert(numberOfWords * sizeof(char*), "Memory is full");

  //add pointers to each word in the input, which has been normalized
  char* p = normalizeWord(input);
  int pos = 0;
  while (*p != '\0') {
    // cheack if pointing at a letter (instead of a space)
    if (isalpha(*p)) {
      words[pos] = p;
      pos++;
      //Wait for the next space or end of line
      while ((isalpha(*p)) && (*p != '\0')) {
          p++;
      }
      // set the first space after word to an end of line if exists
      if ((*p != '\0') && (isspace(*p))){ 
        *p = '\0';
        p++;
      }
    } else {
      p++;
    }
  }
  // print the query in more formal manner (i.e normalize and no extra spaces)
  printf("Query: ");
  for (int i= 0; i<numberOfWords; i++) {
    printf("%s ", words[i]);
  }
  printf("\n");

  // check if no improper "and" "or" uses in input 
  // check beginning
  if (equalAnd(words[0])) {
    mem_free(words);
    fprintf(stderr, "'and' cannot be first\n");
    return NULL;
  }
  if (equalOr(words[0])) {
    mem_free(words);
    fprintf(stderr, "'or' cannot be first\n");
    return NULL;
  } 
  // check end
  if (equalAnd(words[numberOfWords-1])) {
    mem_free(words);
    fprintf(stderr, "'and' cannot be last\n");
    return NULL;
  }
  if (equalOr(words[numberOfWords-1])) {
    mem_free(words);
    fprintf(stderr, "'or' cannot be last\n");
    return NULL;
  }
  // check if any pairing of connectors are next to each other
  pos = 0;
  while (pos < numberOfWords -1) {
    char* next = words[pos+1];
    char* curr = words[pos];
    if (equalOr(next) && equalOr(curr)) {
      mem_free(words);
      fprintf(stderr, "'or' and 'or' cannot be adjacent\n");
      return NULL;
    }
    if (equalAnd(next) && equalAnd(curr)) {
      mem_free(words);
      fprintf(stderr, "'and' and 'and' cannot be adjacent\n");
      return NULL;
    }
    if (equalAnd(next) && equalOr(curr)) {
      mem_free(words);
      fprintf(stderr, "'and' and 'or' cannot be adjacent\n");
      return NULL;
    }
    if (equalOr(next) && equalAnd(curr)) {
      mem_free(words);
      fprintf(stderr, "'or' and 'and' cannot be adjacent\n");
      return NULL;
    }
    pos++;
  }
  return words;
}


/*
 * inputs are the array of words, index made from indexFilename, number of words in thw array
 * and the index in words which we start looking at
 * Returns a counter of docIDs and their scores
*/
static counters_t* getScores(int numberOfWords, index_t* index, char** words, int wordsIndex)
{
  // create main counter which will be returned
  counters_t* ctr = counters_new();
  //Add the first word to main counter if it exists
  counters_t* newCounter = index_find(index, words[wordsIndex]);
  if (newCounter != NULL) {
    counters_iterate(newCounter, ctr, (*addWordOrHelper));
  }
  wordsIndex++;
  for (int i = wordsIndex; i < numberOfWords; i++) {
    if (equalAnd(words[i])){   
      // do nothing
    } else if (equalOr(words[i])) {
      // check if word is or, and employ recusrion if so
      counters_t* remaining = getScores(numberOfWords,index, words,i+1);
      counters_iterate(remaining, ctr, (*addWordOrHelper));
      counters_delete(remaining);
      break;
    } else { 
      // check if can find word and if so adjust our main counter
      //if cannot find, return empty counter 
      newCounter = index_find(index, words[i]);
      if(newCounter == NULL) {
        counters_delete(ctr);
        ctr = counters_new();
      } else {
        ctr = addWordAnd(ctr, newCounter);
      }
    }
  }
  return ctr;
}

/**************** addWordOrHelper() ****************/
/* 
 * adds the count for a specific word from the main and new counter
 * and sets the sum in the main counter
*/
static void addWordOrHelper(void* arg, const int key, const int item)
{
    counters_t* ctr = arg;
    int countFromMain = counters_get(ctr,key);
    counters_set(ctr,key,countFromMain + item);
}

/**************** addWordAndHelper() ****************/
/* 
 * The function inputted in addWordAnd() that is used at every 
 * iteration so that an "and" connector in query is treated accordingly
 * to the rules
*/
static void addWordAndHelper(void* arg, const int key, const int item)
{
    counters_t* andCounter = ((twoCtr_t*)arg)->ctr1;
    counters_t* ctr = ((twoCtr_t*)arg)->ctr2;
    // check if key in ctr, add minimum to andCounter if so
    int num;
    if ((num = counters_get(ctr,key)) != 0) {
        if (num < item) {
          counters_set(andCounter,key,num);
        } else {
          counters_set(andCounter,key,item);
        }
    }
}

/**************** addWordAnd() ****************/
/* 
 * creates new counter and uses iteration of the new counter
 * to fill this new counter only if key is in both the main and new counter
*/
static counters_t* addWordAnd(counters_t* ctr, counters_t* counter_new)
{
  // create new counter which will end up being returned
  counters_t* andCounter = counters_new();
  // use local struct that hold 2 counters_t*
  // note: did this as counters_iterates only expect 3 arguments
  twoCtr_t twoCtrs = {andCounter, ctr};
  counters_iterate(counter_new, &twoCtrs, (*addWordAndHelper));
  counters_delete(ctr);
  return andCounter;
}

// count the counter pairs
static void itemcount(void* arg, const int key, const int count)
{
  int* pairs = arg;
  if (pairs != NULL && key >=0 && count >= 0) {
    (*pairs)++;
  }
}

// print the ranked scores based on the counter scores 
// in format specified in design.md
static void printRankedScores(counters_t* score, char* pageDirectory, int matches)
{
  // sort in order from largest score to smallest score
  pair_t* docIDs = (pair_t*)mem_malloc_assert(matches * sizeof(pair_t), "memory is full");
  // fill in docIDs array with all the docIDs
  int pos = 0;
  indexArray_t indexArray = {docIDs, pos};
  counters_iterate(score, &indexArray, createDocIDsArray);
  // sort
  qsort(docIDs, matches, sizeof(pair_t), scoreDescending);
  for (int i =0; i<matches;i++) {
    char* URL = getLink(pageDirectory, docIDs[i].key);
    printf("score   %d doc  %d: %s\n", docIDs[i].item, docIDs[i].key, URL);
    mem_free(URL);
  }
  mem_free(docIDs);
}

/**************** createDocIDsArray() ****************/
/* 
 * creates an array of all the docIDs that we matched with
*/
static void createDocIDsArray(void* arg, const int key, const int item)
{
    pair_t* docIDs = ((indexArray_t*)arg)->IDs;
    int pos = ((indexArray_t*)arg)->index;
    pair_t pairing= {key,item};
    docIDs[pos] = pairing;
    pos++;
    (((indexArray_t*)arg)->index) = pos;
}


// sort in descending order the scores of the ids
int scoreDescending(const void *a, const void *b) {
    int int_a = ((pair_t *)a)->item;
    int int_b = ((pair_t *)b)->item;
    if (int_a > int_b) return -1;
    if (int_a < int_b) return 1;
    return 0;
}

// return URl of webpage with docID
static char* getLink(char* pageDirectory, int docID)
{
  //construct filepath
  char* pathname = mem_malloc_assert(strlen(pageDirectory) + sizeof(int) + 2, "Memory is full");
  sprintf(pathname, "%s/%d", pageDirectory, docID);
  
  //open the file for reading (checked earlier it can be read)
  FILE* file = fopen(pathname, "r"); 
  // get url on first line
  char* url = file_readLine(file);
  fclose(file); 
  mem_free(pathname);
  return url;
}
