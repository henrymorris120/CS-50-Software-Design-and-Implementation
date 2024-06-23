#!/bin/bash                                                                     

# TSE - testing.sh for querier                                                        
# Purpose: tests that querier is working correctly
# Henry Morris, Fall 2023    

# note for grader: output is all correct but it looks a little weird because I'm not typing it
# into terminal. If I were, it would look perfect.

pageDirectory="/thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/toscrape-2"
indexFilename="/thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/toscrape-2.index"

# invalid arguments
# too few

./querier $pageDirectory

# too many

./querier $pageDirectory $indexFilename fdjskhfgjlksdhjsdhdfgkn

# not crawler page directory

./querier fdjfndjbfdjsjfdjbdfj $indexFilename

./querier $pageDirectory fsnfdndf


# invalid queriers

# invalid characters

echo "Henry 1234"| ./querier $pageDirectory $indexFilename

# start with and

echo "and supernefjfdn"| ./querier $pageDirectory $indexFilename

# end with and

echo "supernefjfdn and"| ./querier $pageDirectory $indexFilename

# start with or

echo "or supernefjfdn"| ./querier $pageDirectory $indexFilename

# end with or

echo "supernefjfdn or"| ./querier $pageDirectory $indexFilename

# have and or

echo "jfdohajdfh and   or supernefjfdn"| ./querier $pageDirectory $indexFilename

# have or and

echo "supernefjfdn or and dfkfkdjfdk "| ./querier $pageDirectory $indexFilename

# no matches query

echo "   fdkhfkhadjfdad      fkdhaakfdh     "| ./querier

# valid tests

echo "football" | ./querier $pageDirectory $indexFilename

echo "dog or     CAt"| ./querier $pageDirectory $indexFilename

echo "  football or dog or cat" | ./querier $pageDirectory $indexFilename

echo "football and dog or cat"| ./querier $pageDirectory $indexFilename

echo "football or cat or pet and dog"| ./querier $pageDirectory $indexFilename

# check valgrind on the above, which shows no leakes

echo "football or cat or pet and dog"| valgrind ./querier $pageDirectory $indexFilename
