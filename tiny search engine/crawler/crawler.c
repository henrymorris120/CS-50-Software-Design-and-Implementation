/* 
 * crawler.c - crawler module
 * implementation follows the Requirement and Design Specs, and follows
 * the Implementation Spec
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include"../common/pagedir.h"
#include "../libcs50/mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local functions ****************/
/* not visible outside this file */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/*
 * the main function simply calls parseArgs and crawl, then exits zero
*/
int main(const int argc, char* argv[]) 
{
    // create pointer to values of the arguments and store in memory
    char** seedURL = mem_malloc_assert(sizeof(char*), "Memory is full");
    char** pageDirectory = mem_malloc_assert(sizeof(char*), "Memory is full");
    int* maxDepth = mem_malloc_assert(sizeof(int*), "Memory is full");
    parseArgs(argc, argv, seedURL, pageDirectory, maxDepth);
    crawl(*seedURL, *pageDirectory, *maxDepth);
    // free memory
    mem_free(seedURL);
    mem_free(pageDirectory);
    mem_free(maxDepth);
    return 0;
}

/*
 * Given arguments from the command line, extract them into the function parameters; 
 * return only if successful.
 * for seedURL, normalize the URL and validate it is an internal URL
 * for pageDirectory, call pagedir_init()
 * for maxDepth, ensure it is an integer in specified range
 * if any trouble is found, print an error to stderr and exit non-zero.
*/

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
    // check if correct number of arguments
    if (argc != 4) {
        fprintf(stderr, "Error: incorrect number of arguments\n");
        exit(1);
    }
    
    // check if valid internal URL
    *seedURL = normalizeURL(argv[1]);
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "Error: invalid internal URL\n");
        exit(2);
    }

    // check if valid page directory
    *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: invalid page directory\n");
        exit(3);
    }

    if (sscanf(argv[3], "%d", maxDepth) != 1 || *maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: Max depth is not an integer in range [0,10]\n");
        exit(4);
    }
}


/*
 * this function does the real work of crawling from seedURL to maxDepth and 
 * saving pages in pageDirectory
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    // initialize the hashtable and add the seedURL
    hashtable_t* pagesSeen = hashtable_new(200);     // initialize to 200 slots
    hashtable_insert(pagesSeen, seedURL, "");
   
    // initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* pagesToCrawl = bag_new();                         
    int depth = 0;
    webpage_t* page = webpage_new(seedURL,depth,NULL);
    bag_insert(pagesToCrawl, page);

    int docID = 1;
    webpage_t* extracted;
    while ((extracted = bag_extract(pagesToCrawl)) != NULL) {
        // fetch HTML from webpage
        if (webpage_fetch(extracted)) {
            fprintf(stdout, "%d \t Fetched: %s\n", webpage_getDepth(extracted), webpage_getURL(extracted));
            // save website and keep going if not at maxDepth
            pagedir_save(extracted, pageDirectory, docID);
            docID++;        // increase docID by 1
            if (webpage_getDepth(extracted) < maxDepth) {
                fprintf(stdout, "%d \t Scanning: %s\n", webpage_getDepth(extracted), webpage_getURL(extracted));
                pageScan(extracted, pagesToCrawl, pagesSeen);
            }
        }
        webpage_delete(extracted);
    }
    bag_delete(pagesToCrawl, NULL);
    hashtable_delete(pagesSeen, NULL);
   
}

/*
 * implements the pagescanner mentioned in the design. 
 * Given a webpage, scans the given page to extract any links (URLs), ignoring non-internal URLs; 
 * for any URL not already seen before (i.e., not in the hashtable), adds the URL to both 
 * the hashtable pages_seen and to the bag pages_to_crawl
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    char* URL;
    int pos = 0;
    int depth = webpage_getDepth(page) +1;
    while((URL = webpage_getNextURL(page, &pos)) != NULL) {
         // check if valid internal URL
        char* new = normalizeURL(URL);
        fprintf(stdout,"%d \t Found: %s\n", webpage_getDepth(page), new);
        mem_free(URL);
        if (isInternalURL(new)) {
            // insert webpage into hashtable
            if (hashtable_insert(pagesSeen, new, "")) {
                // create webpage and insert into bag
                char* new_page_copy = mem_malloc_assert(1 + strlen(new), "Memory is full");
                strcpy(new_page_copy, new);
                webpage_t* new_web = webpage_new(new_page_copy, depth, NULL);
                bag_insert(pagesToCrawl, new_web);
                fprintf(stdout,"%d \t Added: %s\n", webpage_getDepth(page), webpage_getURL(new_web));

            } else {
                 fprintf(stdout,"%d \t IgnDupl: %s\n", webpage_getDepth(page), new);
            }
        } else {
            fprintf(stdout,"%d \t IgnExtrn: %s\n", webpage_getDepth(page), new);

        }
        mem_free(new);
    }
}
