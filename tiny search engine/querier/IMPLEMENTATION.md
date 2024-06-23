# CS50 TSE Querier
## Implementation Spec

In this document we reference the design and requirements specification and focus on the implementation-specific decisions. 
The knowledge unit noted that an [implementation spec](https://github.com/cs50fall2023/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data Structures

We use the `index` structure whose description you can see in that model.  We also create three local data structures that are helpful in the implementation
 1. *twoCtr*, struct that holds 2 pointers to counters;
 2. *pair*, struct that holds a key item pair;
 3. *indexArray*, struct that holds 2 pointers to counters.

## Control flow

The Querier is implemented in one file `querier.c`, with many helper functionss.

### main

The `main` function calls `parseArgs` and `query` then returns 0.

## Other modules



### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Pseudocode for `pagedir_init`:

	construct the pathname for the .crawler file in that directory
	open the file for writing; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_save`:

	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file


### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so our crawler need not implement that part of the spec.


### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
```


### index

We create a re-usable module `index.c` as the key data structure _index_ mentionned in the design. 
The _index_ is a _hashtable_ keyed by word and storing _counters_ as items. The _counters_ is keyed by _docID_ and stores a count of the number of occurrences of that word in the document with that ID.
The module also implements functions to write an index to a file and to create an _index_ from reading an indexFile.
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate an _index_, and how to write and read indexFiles, in one place... anticipating future use by the Querier.

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.


### word

We create a re-usable module `word.c` to handle normalizing a word as mentionned in the design. 
We chose to write this as a separate module, in `../common` anticipating future use by the Querier.

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.

### libcs50

We leverage the modules of libcs50, most notably `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for words;

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
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
```



## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if the given page directory is not a Crawler produced directory, allowing the querier to decide what to do; the querier checks if an indexFile cannot be written to and for what reason (does not exist, read only, etc.).


### Integration/system testing

See the DESIGN.md for details about how testing works

