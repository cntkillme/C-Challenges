# Beginner C Challenges - Doubly-Linked List

## Summary
Your task is to implement a doubly-linked list data structure by implementing the given interface.
You will be judged based on code readability and reuse, proper memory management, implementation of the interface, and to a certain extent, performance.

All **additional challenges** are completely optional, however it is highly recommended you complete them after the required interface has been fully implemented and tested.

## Interface
**Note:** the expected runtime does not account for library function calls, assume those are O(1).  
**Note:** *internal* functions are fine, however it is your job to test them if needed.

```c
struct linked_list {
	struct node* first;
	struct node* last;
	size_t size;
} linked_list;

struct node {
	struct node* prev;
	struct node* next;
	value_t value;
};

typedef struct linked_list linked_list;
typedef struct node node;
typedef double value_t;

/**
 * Creates a new linked_list on the heap and initializes it.
 * For memory allocation errors, see Handling Errors.
 * Expected runtime: O(1)
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
 * Expected runtime: O(n)
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

## Handling Errors
Typically, I don't care how you handle errors provided you spit out a somewhat meaningful error message and exit the program. Here are some templates:

```c
// Memory Allocation Errors
fprintf(stderr, "*** memory allocation failed ***\n");
exit(1);

// Out-of-Bounds Errors
fprintf(stderr, "*** element access failed ***\n");
exit(1);
```

---

## Additional Challenge: Extra Functionality
You may choose to implement as many of these as you'd like.

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
 * If the iterator challenge is planned/completed, sort must not invalidate iterators.
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

---

## Additional Challenge: Iterators
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
iter_t linked_list_advance(const linked_list* list, iter_t iter, int i);

/**
 * Inserts an element before the given iterator.
 * Returns the iterator of the newly inserted element.
 * Expected runtime: O(1)
 * For memory allocation errors, see Handling Errors.
 */
iter_t linked_list_insert(linked_list* list, iter_t iter, value_t value);

/**
 * Erases the element associated with the given iterator.
 * Returns the iterator following the erased element.
 * Expected runtime: O(1)
 * Invalidates iterators to this element.
 * For out-of-bounds errors, see Handling Errors.
 */
iter_t linked_list_erase(linked_list* list, iter_t iter);
```

---

## Test Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "linked_list.h"

// assuming value_t is double

#define ASSERT(c, e) assert(c, e, __LINE__)

void test_additional();
void test_iterator();

void assert(int cond, const char* err, int line)
{
	if (!cond) {
		fprintf(stderr, "Error (line %d): %s\n", line, err);
		exit(1);
	}
}

int main()
{
	linked_list* list1 = linked_list_new();
	linked_list* list2;

	linked_list_resize(list1, 4, 1.0); // test grow
	ASSERT(linked_list_size(list1) == 4, "resize or size failed");
	ASSERT(linked_list_front(list1) == 1.0, "front failed");
	ASSERT(linked_list_back(list1) == 1.0, "back failed");

	list2 = linked_list_copy(list1);
	linked_list_resize(list2, 2, 0.0); // test shrink

	linked_list_clear(list1); // test clear
	ASSERT(linked_list_size(list1) == 0, "clear or size failed");

	// test pushing
	linked_list_push_front(list1, 1.0); // 1.0
	linked_list_push_back(list1, 2.0); // 1.0 2.0
	linked_list_push_front(list1, 0.0); // 0.0 1.0 2.0
	ASSERT(linked_list_size(list1) == 3, "push_front or push_back or size failed");

	// test popping
	linked_list_free(list2);
	list2 = linked_list_copy(list1);

	ASSERT(linked_list_pop_front(list2) == 0.0, "pop_front failed");
	ASSERT(linked_list_pop_back(list2) == 2.0, "pop_back failed");
	ASSERT(linked_list_pop_front(list2) == 1.0, "pop_front failed");
	ASSERT(linked_list_size(list2) == 0, "pop_front or pop_back or size failed");

	ASSERT(linked_list_size(list1) == 3, "mutation of list2 affected list1");

	// test get
	linked_list_free(list2);

	ASSERT(linked_list_get(list1, 0) == 0.0, "get failed");
	ASSERT(linked_list_get(list1, 1) == 1.0, "get failed");
	ASSERT(linked_list_get(list1, 2) == 2.0, "get failed");

	// test set
	linked_list_set(list1, 0, -1.0);
	linked_list_set(list1, 1, -5.0);
	linked_list_set(list1, 2, -10.0);
	ASSERT(linked_list_get(list1, 0) == -1.0, "set or get failed");
	ASSERT(linked_list_get(list1, 1) == -5.0, "set or get failed");
	ASSERT(linked_list_get(list1, 2) == -10.0, "set or get failed");

	linked_list_free(list1);

	test_additional();
	test_iterator();

	printf("Tests completed!\n");
	return 0;
}

void write_f(double b) {
	printf("%02.0f ", b);
}

int comparator(double a, double b) {
	return a <= b;
}

void test_additional()
{
	linked_list* list = linked_list_new();
	linked_list* sorted;
	linked_list* reversed;

	srand((unsigned int)time(NULL));

	for (int i = 0; i <= 5 + rand()%50; i++)
		linked_list_push_back(list, rand() % 100);

	sorted = linked_list_copy(list);
	linked_list_sort(sorted, comparator);
	reversed = linked_list_copy(sorted);
	linked_list_reverse(reversed);

	// test sort
	{
		int is_sorted = 1;

		for (size_t i = 0; i < linked_list_size(sorted) - 1; i++) {
			value_t a = linked_list_get(sorted, i);
			value_t b = linked_list_get(sorted, i + 1);

			if (!comparator(a, b)) {
				is_sorted = 0;
				break;
			}
		}

		ASSERT(is_sorted, "sort failed");
	}

	// test reverse
	{
		int is_reversed = 1;

		ASSERT(linked_list_size(sorted) == linked_list_size(reversed), "reverse failed");

		for (size_t i = 0; i < linked_list_size(reversed); i--) {
			value_t a = linked_list_get(reversed, i);
			value_t b = linked_list_get(sorted, linked_list_size(reversed) - i - 1);

			if (a != b) {
				is_reversed = 0;
				break;
			}
		}

		ASSERT(is_reversed, "reverse failed");
	}

	// test append
	{
		linked_list* appended = linked_list_copy(sorted);
		linked_list* reversedClone = linked_list_copy(reversed);
		int is_appended = 1;

		linked_list_append(appended, reversedClone);
		
		ASSERT(linked_list_size(reversedClone) == 0, "append failed");
		ASSERT(linked_list_size(appended) == linked_list_size(sorted) + linked_list_size(reversed), "append failed");

		for (size_t i = 0; i < linked_list_size(sorted); i++) {
			value_t a = linked_list_get(appended, i);
			value_t b = linked_list_get(sorted, i);

			if (a != b) {
				is_appended = 0;
				break;
			}
		}

		for (size_t i = 0; i < linked_list_size(reversed); i++) {
			value_t a = linked_list_get(appended, linked_list_size(sorted) + i);
			value_t b = linked_list_get(reversed, i);

			if (a != b) {
				is_appended = 0;
				break;
			}
		}

		ASSERT(is_appended, "append failed");

		linked_list_free(appended);
		linked_list_free(reversedClone);
	}

	linked_list_free(list);
	linked_list_free(sorted);
	linked_list_free(reversed);
}

void test_iterator()
{
	linked_list* list = linked_list_new();
	double values[] = { 10, 15, 20, 25, 30 };
	size_t len = sizeof(values)/sizeof(double);

	for (size_t i = 0; i < len; i++)
		linked_list_push_back(list, values[i]);

	// test begin, end, advance
	{
		iter_t iter = linked_list_begin(list);
		size_t idx = 0;
		int correct = 1;

		while (iter != linked_list_end(list)) {
			if (iter->value != values[idx]) {
				correct = 0;
				break;
			}
			iter = linked_list_advance(list, iter, 1);
			idx += 1;
		}

		ASSERT(correct, "begin/end/advance failed");
		ASSERT(iter == linked_list_end(list), "begin/end/advance failed");
		ASSERT(idx == len, "begin/end/advance failed");
	}

	// test insert
	linked_list_insert(list, linked_list_begin(list), 5);
	linked_list_insert(list, linked_list_end(list), 35);
	linked_list_insert(list, linked_list_advance(list, linked_list_begin(list), 2), 12);
	linked_list_insert(list, linked_list_advance(list, linked_list_end(list), -5), 13); // 5 10 12 13 15 20 25 30 35

	ASSERT(linked_list_size(list) == 9, "insert failed");
	ASSERT(linked_list_get(list, 1) == 10.0f, "insert failed");
	ASSERT(linked_list_get(list, 2) == 12.0f, "insert failed");
	ASSERT(linked_list_get(list, 4) == 15.0f, "insert failed");
	ASSERT(linked_list_get(list, 6) == 25.0f, "insert failed");

	// test erase
	{
		iter_t third = linked_list_advance(list, linked_list_begin(list), 2);

		while (third != linked_list_end(list))
			third = linked_list_erase(list, third); // 5 10

		ASSERT(linked_list_size(list) == 2, "erase failed");

		third = linked_list_advance(list, third, -1);
		ASSERT(third->value == 10.0f, "erase/advance failed");
		third = linked_list_advance(list, third, -1);
		ASSERT(third->value == 5.0f, "erase/advance failed");

		ASSERT(third == linked_list_begin(list), "erase/advance failed");
	}

	linked_list_free(list);
}
```
