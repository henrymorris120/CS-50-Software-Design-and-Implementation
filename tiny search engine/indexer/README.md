# Henry Morris

### indexer
The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file

### usage
The *indexer* module uses 1 data structure: an 'index' whose description you can see in its module in common. It start empty. The size of the hashtable(slots) in index is impossible to determine in advance, so we use 500.



```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** indexFilename, char** pageDirectory);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* webpage, int docID);
```

### Implementation
'main' works by simply calling parseArgs and index_build, and then exitting zero
If a non-zero integer is exitted from the program "parseArgs", we know we have an error.
'indexer' just builds an in-memory index from webpage files it finds in the pageDirectory
and then writes/saves this indexer it just built onto the file given as an argument


### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `testing.sh` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make indexer.o`.

### Testing

The `testing.sh` adds a valid internal link.
It tests a few errors as well.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run. 
Looking at `testing.out`, the results agree with the the expectations indicated in 
`testing.sh`.

To test with valgrind, `make valgrind`.

Note for reader, the actual of the tests are in the folder directory. In testing.out, we just print the valgrind results which say no errors
