#!/bin/bash                                                                     

# TSE - testing.sh for indexer                                                         
# Purpose: tests that indexer is working correctly
# Henry Morris, Fall 2023    

# incorrect number of arguments
./indexer /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/letters-3 
./indexer directory/2
./indexer 
./indexer directory/2 fdjfkdhjfd njfdjwfkjdf knfdknfkd kfdkjnfdkd
# invalid page directory (not exist or not crawler directory)
./indexer madeupshit directory/2
./indexer directory directory/2
# invalid file (not exist or read only)
./indexer /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/letters-3 /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/letters-3.index


# successful runs with correct number of arguments and valid files/directories
valgrind --leak-check=full --show-leak-kinds=all ./indextest /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/letters-3.index directory/1
valgrind --leak-check=full --show-leak-kinds=all ./indexer /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/letters-3 directory/2
# check if correct by comparing output files of index test and indexer
~/cs50-dev/shared/students_files/tse/indexcmp directory/2 directory/1

# run indexer on some other page directories
./indexer /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/toscrape-1 directory/3
./indexer /thayerfs/home/f0056n5/cs50-dev/shared/students_files/tse/output/wikipedia-1 directory/4
