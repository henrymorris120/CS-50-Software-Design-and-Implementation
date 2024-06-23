#!/bin/bash                                                                     

# Lab 4 TSE - testing.sh for crawler                                                         
# Purpose: tests that crawler is working correctly
# Henry Morris, Fall 2023    

mkdir directory;

# test cases where there are errors
# not enough arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory
# invalid URL
./crawler http://www.google.com directory 5
# invalid directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html fkefjdjfdn 7
# depth too high
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 20
# depth too low 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory -30

# we now test the cases asked by us in the implementation

# letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 1

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 2

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 3

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 10

# scrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html directory 0

# run with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html directory 1


# wikipedia
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ directory 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ directory 1
