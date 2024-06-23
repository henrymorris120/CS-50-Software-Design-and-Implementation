# CS50 TSE Querier
## Design Spec

According to the Querier Requirements Spec, the TSE *Querier* is a standalone program that reads the document files produced by the TSE crawler and the corresponding index files produced by 
TSE indexer, takes in queries from stdin, and prints out documents that satisfy the query in descending order ranked by their score (how many times they satisfy the query). 

### User interface

The querier's first interface with the user is on the command line; it must always have two arguments.

```
querier pageDirectory indexFilename
```

The querier's second inteface with the user is in stdin; there  must be only one query per line and querier will keep on reading lines of queries form stdin until EOF (when press control d). When the program detects stdin is a terminal, it will prompt the user to enter a query with the message "Query?".

```
Query? userInputtedQuery
```

### Inputs and output

**Input**: the querier reads the index file by using the indexFilename from the parameters as the path.
The indexer reads files from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID.
The indexer reads queries from stdin by reading each line into one string.

**Output**: We print all documents satisfying the query in descending order ranked by score with one document per line. 
If no documents satisfy the query we print `No documents match.`

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules;
 2. *query*, which perfomrs the query, initializing other functions;
 3. *tokenize*, which indexes the the words from input;
 4. *getScore*, which from the tokenized input creates the score counters object
 4. *printRankedScores*, which prints out the counters we created from user input in ranked order;

And some helper modules that provide data structures:

 1. *pagedir*, a module providing functions to check if pageDirectory is a crawler produced directory;
 2. *word*, a module providing a function to normalize a word (convert all letters in query to lowercase);
 3. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    create an index from indexFileName
    call query
    free all memory

where *query:*
  while user input is not EOF 
    check if input is all alphabet letters
      tokenize input which will remove all extra spaces
        check if correct usage of connectors (and, or)
          call getScores
          call printRankedScore
        else print error connector message
    else print error character message


where *getScore:*

    create new counters object for given word
    create new counters object for results
    loop through each (key count) pair in current counters
      if the key also exists in our created counters
        add key and smaller of the two counts in current counters and our created counters

where *printRankedScore*:

    loops through each (key count) pair in score and builds array of counterpair ranked in order of score using qsort
    Iterate through array 




### Major data structures

Global data types:
 We also create three local data structures that are helpful in the implementation
 1. *twoCtr*, struct that holds 2 pointers to counters;
 2. *pair*, struct that holds a key item pair;
 3. *indexArray*, struct that holds 2 pointers to counters.

Helper modules provided data structures:
- *counters* of (key, count) pairs that represent andsequences
- *index* of word, docID and frequencies that uses a *hashtable* keyed by String words with *counters* as the items

### Testing plan

1. Test `querier` with various invalid arguments.
    1. too few arguments
    2. too many arguments
    3. invalid `pageDirectory` (non-existent path) and `indexFilename`
2. Test `querier` with various invalid queries
    1. begins with end
    2. begins with or
    3. ends with and
    4. ends with or
    5. adjacent connectors
    6. contains non letter and non space characters
3. Test accuracy of query by comparing output to manually created expected result (with unclean inputs)
4. Run *valgrind* on *querier* to ensure no memory leaks or errors
        
    
