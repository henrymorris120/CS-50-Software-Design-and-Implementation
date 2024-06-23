# Henry Morris
## CS50 Fall 2023, Lab 6

GitHub username: henrymorris120
use late day if needed

### Querier
TSE Querier is a standalone program that reads the document files produced by the TSE crawler and the corresponding index files produced by TSE indexer, takes in queries from stdin, and prints out documents that satisfy the query in descending order ranked by their score (how many times they satisfy the query). 

### usage
The *querier* module uses 'index' whose description you can see in its module in common. It start empty. The size of the hashtable(slots) in index is impossible to determine in advance, so we use 500.It also create 3 local structures whose description you can see in the querier module.



```c
int main(const int argc, char* argv[]);
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

### Implementation
'main' works by simply calling parseArgs and then query, and then exitting zero
If a non-zero integer is exitted from the program "parseArgs", we know we have an error.


### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation
* `testing.sh` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make querier.o`.

### Testing

The `testing.sh` adds a valid internal link.
It tests a few errors as well.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run. 
Looking at `testing.out`, the results agree with the the expectations indicated in 
`testing.sh`.

To test with valgrind, `make valgrind`.
