#include <stddef.h>

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

typedef int (*comparator_t)(value_t, value_t);
typedef void (*callback_t)(value_t);
typedef node* iter_t;
typedef const node* const_iter_t;

/**
 * Initialized a linked_list object.
 */
void linked_list_init(linked_list* list);

/**
 * Copies a linked_list and all of its elements.
 * The two lists should be fully independent of each other.
 * Assume the destination list is empty.
 */
void linked_list_copy(linked_list* dest, const linked_list* src);

/**
* Clears a linked_list of all its elements.
*/
void linked_list_clear(linked_list* list);

/**
* Resizes a linked_list to the given size. For newly created nodes, initialize
* them with the given value.
*/
void linked_list_resize(linked_list* list, size_t newSize, value_t value);

/**
* Returns the size (number of elements) of a linked_list.
*/
size_t linked_list_size(const linked_list* list);

/**
* Returns the first element of a linked_list
*/
value_t linked_list_front(const linked_list* list);

/**
* Returns the last element of a linked_list
*/
value_t linked_list_back(const linked_list* list);

/**
* Adds an element with the given value to the end of a linked_list.
*/
void linked_list_push_front(linked_list* list, value_t value);

/**
* Adds an element with the given value to the beginning of a linked_list.
*/
void linked_list_push_back(linked_list* list, value_t value);

/**
* Removes the element at the beginning of a linked_list and returns it.
*/
value_t linked_list_pop_front(linked_list* list);

/**
* Removes the element at the end of a linked_list and returns it.
*/
value_t linked_list_pop_back(linked_list* list);

/**
* Returns the element at the given index of a linked_list.
* Assume idx is in the range [0, size)
*/
value_t linked_list_get(const linked_list* list, size_t idx);

/**
* Alters the element at the given index of a linked_list and
* returns the old value.
* Assume idx is in the range [0, size)
*/
value_t linked_list_set(linked_list* list, size_t idx, value_t newValue);

/**
* Reverses the elements of a linked_list.
*/
void linked_list_reverse(linked_list* list);

/**
* Sorts the elements of a linked_list in the order defined by a comparator.
*/
void linked_list_sort(linked_list* list, comparator_t comparator);

/**
* Appends one linked_list to the end of another.
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
