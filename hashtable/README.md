### hashtable

A `hashtable` is a set of (_key_,_item_) pairs. It acts just like a _set_, but is far more efficient for large collections.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements a hashtable of (`char* key`:`void* item`), and exports the following functions:

```c
hashtable_t* hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t* ht, const char* key, void* item);
void* hashtable_find(hashtable_t* ht, const char* key);
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) );
```

### Implementation

We implement this as an array of set pointers.
The *hashtable* itself is represented as a `struct hashtable` containing an integer `int num_slots` and a pointer to an array, of the size of that interger, of set pointers.

See the `set` module for more information about how sets work.

To insert a new key-item pair in the hashtable we hash the key to determine which slot
it goes in, and then add the key-item pair into that slot's set

To find an item from the hashtable we go to the correct slot and then look in that set

The `hashtable_print` method, for each slot which gets its own line, calls on that line the `itemprint` function on each item by scanning the linked list.

The `hashtable_iterate` method calls the `itemfunc` function on ever pair in the hashtable.

The `hashtable_delete` method calls the `itemdelete` function on each pair, freeing as it proceeds.
It concludes by freeing the `struct hashtable`.

### Assumptions
`item` and `key` inserted are not NULL, and the number of slots is a positive integer

### Files

* `Makefile` - compilation procedure
* `hashtable.h` - the interface
* `hashtable.c` - the implementation
* `hashtabletest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make hashtable.o`.

### Testing

The `hashtabletest.c` program inserts key-item pairs into a hashtable, then finds some pairs and prints the hashtable out.
It tests a few error and edge cases as well.



To test, simply `make test`.
See `testing.out` for details of testing and an example test run.
Looking at `testing.out`, the results agree with the the expectations indicated in 
`hashtabletest.c`.

To test with valgrind, `make valgrind`.
