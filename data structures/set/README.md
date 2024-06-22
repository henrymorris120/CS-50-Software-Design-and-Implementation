# Henry Morris
## CS50 Fall 2023

### set

A `set` maintains an unordered collection of (_key_,_item_) pairs; any given _key_ can only occur in the set once. It starts out empty and grows as the caller inserts new (_key_,_item_) pairs. The caller can retrieve _items_ by asking for their _key_, but cannot remove or update pairs. Items are distinguished by their _key_.

### Usage

The *set* module, defined in `set.h` and implemented in `set.c`, implements a set of (`char* key`:`void* item`) and exports the following functions:

```c
set_t* set_new(void);
bool set_insert(set_t* set, const char* key, void* item);
void* set_find(set_t* set, const char* key);
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) );
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void set_delete(set_t* set, void (*itemdelete)(void* item) );
```

### Implementation

We implement this bag as a linked list.
The *set* itself is represented as a `struct set` containing a pointer to the head of the list; the head pointer is NULL when the set is empty.

Each setpair in the list is a `struct setpair`, a type defined internally to the module.
Each setpair includes a pointer to the `void* item`, a pointer to the key in this setpair 
`char* key`, and a pointer to the next setpair on the list.

To insert a new item in the set we create a new setpair to hold the `item` and `key`, and insert it at the head of the list.

The `set_print` method prints a little syntax around the list, and between items, but mostly calls the `itemprint` function on each setpair by scanning the linked list.

The `set_iterate` method calls the `itemfunc` function on each setpair by scanning the linked list.

The `set_delete` method calls the `itemdelete` function on each setpair by scanning the linked list, freeing everything as it proceeds.
It concludes by freeing the `struct set`.

### Assumptions

`item` and `key` inserted are not NULL

### Files

* `Makefile` - compilation procedure
* `set.h` - the interface
* `set.c` - the implementation
* `settest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make set.o`.

### Testing

The `settest.c` program inserts key-item pairs into a set, then finds some set pairs and prints the set out.
It tests a few error and edge cases as well.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.
Looking at `testing.out`, the results agree with the the expectations indicated in 
`settest.c`.

To test with valgrind, `make valgrind`.
