### counters

A `counters` is a set of counters, each distinguished by an integer _key_. It's a set - each _key_ can only occur once in the set - and it tracks a _counter_ for each _key_. It starts empty. Each time `counters_add` is called on a given _key_, the corresponding _counter_ is incremented. The current _counter_ value can be retrieved by asking for the relevant _key_.

### Usage

The *counters* module, defined in `counters.h` and implemented in `counters.c`, implements a counter set of (`int* key`:`int* count`), and exports the following functions:

```c
counters_t* counters_new(void);
int counters_add(counters_t* ctrs, const int key);
int counters_get(counters_t* ctrs, const int key);
bool counters_set(counters_t* ctrs, const int key, const int count);
void counters_print(counters_t* ctrs, FILE* fp);
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count));
void counters_delete(counters_t* ctrs);
```

### Implementation

We implement this counter set as a linked list.
The *counters* itself is represented as a `struct counters` containing a pointer to the head of the list; the head pointer is NULL when the counter set is empty.

Each countpair in the list is a `struct countpair`, a type defined internally to the module.
Each countpair includes a pointer to a non negative `int* key`, a pointer to the count associated with that key `int* count`,  and a pointer to the next countpair on the list.

To insert a new countpair in the counter set we create a new countpair to hold the `key` and `count` which is set to 1, and insert it at the head of the list.

To get a count for a specific key, we iterate throuhg the linked list, checking the key held by each countpair and returning the count if the key matches.

To increment a counter by the key, we iterate throuhg the linked list, checking the key held by each countpair and adding 1 to that countpair's count. If it does not exist, we add a new countpair.

To set a counter by the key, we iterate throuhg the linked list, checking the key held by each countpair and setting whatever non-negative integer inputted to that countpair's count. If it does not exist, we add a new countpair with that count.

The `counters_iterate` method calls the `itemfunc` function on each item by scanning the linked list.

The `counters_delete` method calls the `itemdelete` function on each item by scanning the linked list, freeing bagnodes as it proceeds.
It concludes by freeing the `struct counters`.

### Assumptions

`count` and `key` are nonnegative

### Files

* `Makefile` - compilation procedure
* `counters.h` - the interface
* `counters.c` - the implementation
* `counterstest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make counters.o`.

### Testing

The `counterstest.c` program inserts key-item pairs into a set counter, then finds some pairs and prints the counter set out.
It tests a few error and edge cases as well.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run. 
Looking at `testing.out`, the results agree with the the expectations indicated in 
`counterstest.c`.

To test with valgrind, `make valgrind`.
