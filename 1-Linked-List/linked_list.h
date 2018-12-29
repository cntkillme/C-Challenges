#include <stdbool.h>
#include <stddef.h>
 // stdbool.h for bool
 // stddef.h for size_t, ptrdiff_t

struct linked_list;
struct node;
typedef double value_t;

typedef struct linked_list
{
	struct node* first;
	struct node* last;
	size_t size;
} linked_list;

typedef struct node
{
	struct node* prev;
	struct node* next;
	value_t value;
} node;

typedef bool (*comparator_t)(const value_t*, const value_t*);
typedef void (*callback_t)(const value_t*);
typedef node* iter_t;
typedef const node* const_iter_t;

/**
 * Initializes a linked_list object.
 */
void linked_list_init(linked_list* list);

/**
 * Copies a linked_list and all of its elements. The two lists should be fully independent of each other.
 * Assume the destination list is empty.
 */
void linked_list_copy(linked_list* dest, const linked_list* src);

/**
 * Clears a linked_list of all its elements.
 */
void linked_list_clear(linked_list* list);

/**
 * Resizes a linked_list to the given size. For newly created nodes, initialize them with the given value.
 */
void linked_list_resize(linked_list* list, size_t newSize, value_t value);

/**
 * Returns the size (number of elements) of a linked_list.
 */
size_t linked_list_size(const linked_list* list);

/**
 * Returns the first element of a linked_list.
 * Assume the list is not empty.
 */
value_t linked_list_front(const linked_list* list);
/**
 * Returns the last element of a linked_list.
 * Assume the list is not empty.
 */
value_t linked_list_back(const linked_list* list);

/**
 * Adds an element with the given value to the beginning of a linked_list.
 */
void linked_list_push_front(linked_list* list, value_t value);

/**
 * Adds an element with the given value to the end of a linked_list.
 */
void linked_list_push_back(linked_list* list, value_t value);

/**
 * Removes the element at the beginning of a linked_list and returns it.
 * Assume the list is not empty.
 */
value_t linked_list_pop_front(linked_list* list);

/**
 * Removes the element at the end of a linked_list and returns it.
 * Assume the list is not empty.
 */
value_t linked_list_pop_back(linked_list* list);

/**
 * Returns the element at the given index of a linked_list.
 * Assume idx is in the range [0, size)
 */
value_t linked_list_get(const linked_list* list, size_t idx);

/**
 * Alters the element at the given index of a linked_list and returns the old value.
 * Assume idx is in the range [0, size)
 */
value_t linked_list_set(linked_list* list, size_t idx, value_t newValue);

/**
 * Reverses the elements of a linked_list.
 */
void linked_list_reverse(linked_list* list);

/**
 * Sorts the elements of a linked_list in the order defined by the comparator.
 */
void linked_list_sort(linked_list* list, comparator_t comparator);

/**
* Appends one linked_list to the end of another. The source list should be an empty list.
* The source linked_list should become an empty linked list.
*/
void linked_list_append(linked_list* dest, linked_list* src);

/**
 * Iterates over a linked_list and invokes a callback for each element.
 */
void linked_list_foreach(const linked_list* list, callback_t callback);

/**
 * Swaps the elements of two linked_lists.
 */
void linked_list_swap(linked_list* list1, linked_list* link2);

/**
 * Returns an iterator to the first element of a linked_list. If the list is empty, the end iterator is returned.
 */
iter_t linked_list_begin(linked_list* list);

/**
 * Returns an iterator to one after the last element of a linked_list.
 */
iter_t linked_list_end(linked_list* list);

/**
 * Returns the element associated with an iterator.
 * Assume iter is in the range [begin, end).
 */
value_t linked_list_read(const linked_list* list, const_iter_t iter);

/**
 * Alters the element associated with an iterator and returns the old value.
 * Assume iter is in the range [begin, end).
 */
value_t linked_list_write(linked_list* list, iter_t iter, value_t value);

/**
 * Advances an iterator by a number of steps, a negative step indicates advancing backwards.
 * Assume iter + steps will be in the range [begin, end].
 */
iter_t linked_list_advance(linked_list* list, iter_t iter, ptrdiff_t steps);

/**
 * Inserts an element before a given iterator and returns an iterator to the new element.
 */
iter_t linked_list_insert(linked_list* list, iter_t iter, value_t value);

/**
 * Erases an element at the given iterator and returns the iterator following the erased element.
 * Assume iter is in the range [begin, end) and iter != end.
 */
iter_t linked_list_erase(linked_list* list, iter_t iter);

/**
 * Returns the distance between two nodes, negative if first comes after last.
 */
ptrdiff_t linked_list_dist(linked_list* list, const_iter_t iter1, const_iter_t iter2);

/**
 * Inserts some number elements before the given iterator that are initialized with then given value.
 * Returns an iterator to the first inserted element (or begin if count = 0).
 */
iter_t linked_list_insert_many(linked_list* list, iter_t begin, size_t count, value_t value);

/**
 * Erases all elements in the range [begin, begin + count). If begin + count >= end, erase all elements after begin.
 * Assume begin != end.
 * Returns an iterator to the iterator following the last erased element (or begin if count = 0).
 */
iter_t linked_list_erase_many(linked_list* list, iter_t begin, size_t count);

/**
 * Inserts nodes in [first, last) before dest. Beware of overlapping between the nodes and the dest.
 * Assume dist(first, last) is non-negative and first != end.
 * Returns an iterator to the first inserted element (or dest if first = last).
 */
iter_t linked_list_insert_range(linked_list* list, iter_t dest, const_iter_t first, const_iter_t last);

/**
 * Erases all elements in the range [first, last)
 * Assume dist(first, last) is non-negative and first != end.
 * Returns the iterator following the last erased element (or first if first = last).
 */
iter_t linked_list_erase_range(linked_list* list, iter_t first, iter_t last);

/**
 * Swaps the nodes associated with two iterators.
 * Assume iter1, iter2 are in the range [begin, end).
 */
void linked_list_swap_nodes(linked_list* list, iter_t iter1, iter_t iter2);

/**
 * Reverses the nodes of a linked_list by their elements from [first, last).
 * Assume dist(first, last) is non-negative and first != end.
 */
void linked_list_reverse_nodes(linked_list* list, iter_t first, iter_t last);

/**
 * Sorts the nodes of a linked_list by their elements from [first, last) in the order defined by a comparator.
 * Assume dist(first, last) is non-negative, and and first != end.
 */
void linked_list_sort_nodes(linked_list* list, iter_t first, iter_t last, comparator_t comparator);
