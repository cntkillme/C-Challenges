# Beginner C Challenges - Doubly-Linked List

## Summary
Your task is to implement a doubly-linked list data structure by implementing the given interface.
You will be judged based on code readability and reuse, proper memory management, implementation of the interface, and to a certain extent performance.

All additional challenges are completely optional, however it is highly recommended to complete after the required interface has been fully implemented and tested.

## Interface
**Note:** the expected runtime does not account for library function calls, assume those are O(1).

**Note:** *internal* functions shall be prefixed with an underscore (i.e. `_linked_list_new_node`)

```c
typedef struct linked_list linked_list;
typedef struct node node;
typedef double value_t;

struct linked_list {
	node* first;
	node* last;
	size_t size;
} linked_list;

struct node {
	node* prev;
	node* next;
	value_t value;
};

/**
 * Creates a new linked_list on the heap and initializes it.
 * For memory allocation errors, see Handling Errors.
 */
linked_list* linked_list_new();

/**
 * Frees a linked_list and its associated nodes from memory.
 * Expected runtime: O(n)
 * Hint: linked_list_clear may be called here.
 */
void linked_list_free(linked_list* list);

/**
 * Copies a linked_list and all of its elements.
 * Expected runtime: O(n)
 * For memory allocation errors, see Handling Errors.
 */
linked_list* linked_list_copy(const linked_list* list);

/**
 * Resizes the linked_list structure.
 * If count is greater than the current size, push additional elements initialized with the given value.
 * If count is less than the current size, pop the excess elements.
 * For memory allocation errors, see Handling Errors.
 */
void linked_list_resize(linked_list* list, size_t newSize, value_t value);

/**
 * Returns the size of a linked_list.
 * Expected runtime: O(1)
*/
size_t linked_list_size(const linked_list* list);

/**
 * Clears all elements of the linked_list.
 * Expected runtime: O(n)
 */
void linked_list_clear(linked_list* list);

/**
 * Returns the value of the first element of the linked_list.
 * Expected runtime: O(1)
 * For out-of-bounds errors, see Handling Errors.
 */
value_t linked_list_front(const linked_list* list);

/**
 * Returns the value of the last element of the linked_list.
 * Expected runtime: O(1)
 * For out-of-bounds errors, see Handling Errors.
 */
value_t linked_list_back(const linked_list* list);

/**
 * Adds an element  with the given value to the beginning of the list.
 * Expected runtime: O(1)
 * For memory allocation errors, see Handling Errors.
 */
void linked_list_push_front(linked_list* list, value_t value);

/**
  * Adds an element with the given value to the end of the linked_list.
  * Expected runtime: O(1)
 * For memory allocation errors, see Handling Errors.
  */
void linked_list_push_back(linked_list* list, value_t value);

/**
 * Removes an element from the beginning of the linked_list and returns the value.
 * Expected runtime: O(1)
 * For out-of-bounds errors, see Handling Errors.
 */
value_t linked_list_pop_front(linked_list* list);

/**
 * Removes an element from the end of the linked_list and returns the value.
 * Expected runtime: O(1)
 * For out-of-bounds errors, see Handling Errors.
 */
value_t linked_list_pop_back(linked_list* list);

/**
 * Returns the value at the given position of the linked_list.
 * Expected runtime: O(n)
 * For out-of-bounds errors, see Handling Errors.
 */
value_t linked_list_get(linked_list* list, size_t idx);

/**
 * Sets the value at the given position of the linked_list.
 * Expected runtime: O(n)
 * For out-of-bounds errors, see Handling Errors.
 */
void linked_list_set(linked_list* list, size_t idx, value_t value);
```

## C Library Reference
```c
// stdio.h //
int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);

// stdlib.h //
void* malloc(size_t size);
void free(void* chunk);
void exit(int errcode);
```

## Handling Errors
```c
// Memory Allocation Errors
fprintf(stderr, "*** memory allocation failed ***\n");
exit(1);

// Out-of-Bounds Errors
fprintf(stderr, "*** element access failed ***\n");
exit(1);
```

## Additional Challenge: Implement Iterators
An iterator is a *pointer-like* object that refers to a node in the linked_list.

**Iterator invalidation** refers to when an iterator is no longer valid. For linked_list this occurs when:
- The element associated with the given iterator has been deleted.

```c
typedef struct node* iter_t;

/**
 * Returns an iterator to the start of the linked_list.
 * If the list is empty, the end iterator is returned.
 * Expected runtime: O(1)
 */
iter_t linked_list_begin(const linked_list* list);

/**
 * Returns an iterator to the end of the linked_list (one passed the last element).
 * Expected runtime: O(1)
 */
iter_t linked_list_end(const linked_list* list);

/**
 * Advances the iterator by `i`.
 * Expected runtime: O(|i|)
 * For out-of-bounds errors, see Handling Errors.
 */
iter_t linked_list_advance(const linked_list* list, const iter_t iter, int i);

/**
 * Inserts an element before the given iterator.
 * Returns the iterator of the newly inserted element.
 * Expected runtime: O(1)
 * For memory allocation errors, see Handling Errors.
 */
iter_t linked_list_insert(linked_list* list, iter_t iter, value_t value);

/**
 * Erases the element associated with the given iterator.
 * Returns the iterator of the previous element.
 * Expected runtime: O(1)
 * Invalidates iterators to this element.
 * For out-of-bounds errors, see Handling Errors.
 */
iter_t linked_list_erase(linked_list* list, iter_t iter);
```

## Additional Challenge: Extra Functionality
You may choose to implement as many of these as you'd like. If you have decided to do the iterator additional challenge, 

```c
typedef int (*comparator_t)(value_t, value_t);
typedef void (*callback_t)(value_t);

/**
 * Reverses a linked_list.
 * Expected runtime: O(n)
 */
void linked_list_reverse(linked_list* list);

/**
 * Sorts a linked_list in the order defined by the given comparator function.
 * Expected runtime: O(n^2) or better
 */
void linked_list_sort(linked_list* list, comparator_t comparator);

/**
 * Appends one linked_list to the end of another.
 * The first linked_list `list` is the destination.
 * The second linked_list `other` shall become an empty list.
 * Hint: you should not need to call `linked_list_clear`.
 * Expected runtime: O(1)
 */
void linked_list_append(linked_list* list, linked_list* other);

/**
 * Iterates over the entirety of the linked_list and invoked `callback` for each element.
 * Expected runtime: O(n) without regard to the complexity of the callback.
 */
void linked_list_foreach(const linked_list* list, callback_t callback);

/**
 * Swaps the contents of the two linked_lists.
 * Expected runtime: O(1)
 */
void linked_list_swap(linked_list* list1, linked_list* list2);
```
