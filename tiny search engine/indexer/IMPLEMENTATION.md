# CS50 TSE Indexer
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

We create a structure called 'index' containing words, docIDs and counts in order to keep track of the number of occurences of each word in each document. 
The 'index' starts empty. 
We implement the structure by using a _hashtable_ with a noramlized string _word_ as the key and _counters_ as the item. 
Each _counters_ contains pairs of _counter_ (docID, count) where count is the number of occurencs of a word in the corresponding document. This is done using a set
The size of the index is impossible to determine in advance unless we have already read the readfile, so we use 500.

## Control flow

The Indexer is implemented in one file `indexer.c`, with five functions.

### main

The `main` function calls `parseArgs`,`indexBuild`, and `index_write` then returns 0.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* validate that there are 2 arguments given
* for `pageDirectory` make sure that the directory is the pathname for a directory produced by the Crawler
* for `indexFileName` make sure it is the pathname of a file that can be written
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

This function implements the _indexBuild_ mentioned in the design. It build an in-memory _index_ from  from webpage files it finds in the pageDirectory
Pseudocode:

```

    creates a new 'index' object
    loops over document ID numbers, counting from 1
        loads a webpage from the document file 'pageDirectory/id'
        if successful, 
          returns the 'index'
        else 
          print error and return null

```

### indexPage

This function implements the _indexPage_ mentioned in the design. It scans a webpage document to add its words to the index.
Pseudocode:

```

 steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID

```

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
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(webpage_t* webpage, int docID, index_t* index);
```



## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if the given page directory is not a Crawler produced directory, allowing the Indexer to decide what to do; the indexer checks if an indexFile cannot be written to and for what reason (does not exist, read only, etc.).

### Unit testing

There are four units (indexer, pagedir, index, word).
The indexer represents the whole system and is covered below.
The pagedir unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.
The index unit can be tested using the the unit test drive `indextest.c`.
The word unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.

### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a call to index a given directory.
Third, run indexer on a variety of pageDirectories and use indextest as one means of validating the resulting index.
Run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
Verify correct behavior by studying the output, and by sampling the index files created in the respective directories.

