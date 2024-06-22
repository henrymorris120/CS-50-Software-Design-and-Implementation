#ifndef HASH_H
#define HASH_H

/*
 * hash_jenkins - Bob Jenkins' one_at_a_time hash function
 * str: char buffer to hash (non-NULL)
 * mod: desired hash modulus (>0)
 *
 * Returns hash(str) % mod. 
 */
unsigned long hash_jenkins(const char* str, const unsigned long mod);

#endif // HASH_H
