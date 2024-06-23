# Henry Morris

### crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a "seed" URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given "depth".

### usage
The *crawler* module use two data structures: a 'bag' of pages that need to be crawled, and a 'hashtable' of URLs that we have seen during our crawl. Both start empty. The size of the hashtable (slots) is impossible to determine in advance, so we use 200.



```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```

### Implementation
'main' works by simply calling parseArgs and crawl, and then exitting zero
If a non-zero integer is exitted from the program "parseArgs", then we have an error , we know we have an erro.
'crawl' actually just crawls  from  seedURL to maxDepth, saving pages in the pageDirectory.
Lastly, pageScan scans a webpage for internal links we habe not seen before


### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `testing.sh` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make crawler.o`.

### Testing

The `testing.sh` adds a valid internal link.
It tests a few errors as well.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run. 
Looking at `testing.out`, the results agree with the the expectations indicated in 
`testing.sh`.

To test with valgrind, `make valgrind`.
