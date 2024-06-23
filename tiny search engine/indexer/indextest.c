/* 
 * indextest.c module CS50
 * load the index from the oldIndexFilename into an inverted-index data structure
 * create a file newIndexFilename and write the index to that file, in the format described below.
 *
 * Henry Morris Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"


int main(int argc, char *argv[])
{
    // check correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Error: incorrect number of arguments\n");
        exit(1);
    }
    
    FILE* new_file;
    FILE* old_file;

    // Check if the old file name can be read
    if ((old_file = fopen(argv[1], "r"))== NULL) {
        fprintf(stderr, "Indexer file could not be read.\n");
        exit(2);
    }

    // Check if the new file name can be written to
    if ((new_file = fopen(argv[2], "w"))== NULL) {
        fprintf(stderr, "Indexer file could not be read.\n");
        fclose(old_file);
        exit(3);
    }
    index_t* index = index_read(old_file);
    index_write(index,new_file);
    index_delete(index);
    fclose(old_file);
    fclose(new_file);
    return 0;
}
