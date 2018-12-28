#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/// TEST INTERFACE ///
#define RUN_TESTS(name, func) { \
	printf("Testing " name "...\n"); \
	func(&success, &total); \
	printf("%lu/%lu tests passed.\n\n", success, total); \
	success = total = 0; \
}

#define TEST_IMPL(expr, msg, line) *total += 1; \
if (expr) \
	*success += 1; \
else \
	printf("Test %lu failed (line %d): %s\n", *total, line, msg)

#define TEST(expr, msg) TEST_IMPL(expr, msg, __LINE__)

static void test_required_interface(size_t* const success, size_t* const total);
static void test_extra_functionality(size_t* const success, size_t* const total);
static void test_iterator_interface(size_t* const success, size_t* const total);
static void test_extra_iterator_functionality(size_t* const success, size_t* const total);

/// CALLBACKS ///
static bool less_than_comparator(const value_t* left, const value_t* right);
static int less_than_comparator_qsort(const void* left, const void* right);
static void sum_list(const value_t* value);
static value_t* get_sum(bool reset);

/// DEBUGGING HELPERS ///
#ifdef DEBUG_OUTPUT
	#define PRINT_ARRAY(arr, pre) print_array(arr, sizeof(arr)/sizeof(value_t), pre, __LINE__)
	#define PRINT_LIST(list, pre) print_list(list, pre, __LINE__)
#else
	#define PRINT_ARRAY(arr, pre) (void)0
	#define PRINT_LIST(list, pre) (void)0
#endif

static void print_value(const value_t* value);
static void print_array(value_t* arr, size_t size, const char* const preMsg, int line);
static void print_list(linked_list* list, const char* const preMsg, int line);

int main(void)
{
	size_t success = 0;
	size_t total = 0;

	#ifdef TEST_REQUIRED_INTERFACE
		RUN_TESTS("Required Interface", test_required_interface);
	#endif

	#ifdef TEST_EXTRA_FUNCTIONALITY
		RUN_TESTS("Extra Functionality", test_extra_functionality);
	#endif

	#ifdef TEST_ITERATOR_INTERFACE
		RUN_TESTS("Iterator Interface", test_iterator_interface);
	#endif

	#ifdef TEST_EXTRA_ITERATOR_FUNCTIONALITY
		RUN_TESTS("Extra Iterator Functionality", test_extra_iterator_functionality);
	#endif

	printf("All tests completed.\n");

	return 0;
}

void test_required_interface(size_t* const success, size_t* const total)
{
	linked_list _list1, _list2;

	linked_list* list1 = &_list1;
	linked_list* list2 = &_list2;

	linked_list_init(list1);
	linked_list_init(list2);

	TEST(linked_list_size(list1) == 0, "new linked_list size NOT 0");

	linked_list_resize(list1, 12, 5.0); // 5.0  ...
	PRINT_LIST(list1, "list1: ");

	TEST(linked_list_size(list1) == 12, "resized list size (to 12) is NOT 12");
	TEST(linked_list_front(list1) == 5.0, "resized list front element NOT 5.0");
	TEST(linked_list_back(list1) == 5.0, "resized list back element NOT 5.0");

	linked_list_copy(list2, list1);
	PRINT_LIST(list1, "list2: ");
	TEST(linked_list_size(list2) == 12, "copied list size is NOT 12");
	TEST(linked_list_front(list2) == 5.0, "copied list front element NOT 5.0");
	TEST(linked_list_back(list2) == 5.0, "copied list back element NOT 5.0");

	linked_list_resize(list2, 4, 0.0);
	PRINT_LIST(list1, "list2: ");
	TEST(linked_list_size(list2) == 4, "list size after down-size is NOT 4");
	TEST(linked_list_front(list2) == 5.0, "list front after down-size is NOT 5.0");
	TEST(linked_list_back(list2) == 5.0, "list back after down-size is NOT 5.0");

	linked_list_clear(list1);
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 0, "cleared list size is NOT 0");
	TEST(linked_list_size(list2) == 4, "copied list is NOT independent");

	linked_list_push_front(list1, 123.0); // 123.0
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 1, "empty list size after push_front is NOT 1");
	TEST(linked_list_front(list1) == 123.0, "empty list front after push_front is NOT 123.0");
	TEST(linked_list_back(list1) == 123.0, "empty list back after push_fron is NOT 123.0");

	linked_list_push_front(list1, 456.0); // 456.0, 123.0
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 2, "list size after second push_front is NOT 2");
	TEST(linked_list_front(list1) == 456.0, "list front after second push_front is NOT 456.0");
	TEST(linked_list_back(list1) == 123.0, "list back after second push_front is NOT 123.0");

	linked_list_push_back(list1, 999.0); // 456.0, 123.0, 999.0
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 3, "list size after three pushes is NOT 3");
	TEST(linked_list_front(list1) == 456.0, "list front is NOT 456.0");
	TEST(linked_list_back(list1) == 999.0, "list back is NOT 999.0");

	// 123.0, 999.0
	TEST(linked_list_pop_front(list1) == 456.0, "value popped from front is NOT 456.0");
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 2, "list size after pop is NOT 2");
	TEST(linked_list_front(list1) == 123.0, "list front after pop is NOT 123.0");
	TEST(linked_list_back(list1) == 999.0, "list back after pop is NOT 999.0");

	// 123.0
	TEST(linked_list_pop_back(list1) == 999.0, "value popped from back is NOT 999.0");
	PRINT_LIST(list1, "list1: ");
	TEST(linked_list_size(list1) == 1, "list size after pop is NOT 2");
	TEST(linked_list_front(list1) == 123.0, "list front after pop is NOT 123.0");
	TEST(linked_list_back(list1) == 123.0, "list back after pop is NOT 123.0");

	TEST(linked_list_get(list1, 0) == 123.0, "value at index 0 is NOT 123.0");
	TEST(linked_list_set(list1, 0, 456.0) == 123.0, "value returned after set at index 0 is NOT 123.0");
	TEST(linked_list_get(list1, 0) == 456.0, "value at index 0 is NOT 456.0");
	TEST(linked_list_front(list1) == 456.0, "value at front is NOT 456.0");
	TEST(linked_list_back(list1) == 456.0, "value at back is NOT 456.0");

	linked_list_clear(list1);
	linked_list_clear(list2);
	PRINT_LIST(list1, "list1: ");
	PRINT_LIST(list2, "list2: ");

	{
		bool setsValid = true;
		bool getsValid = true;

		linked_list_resize(list1, 16, 555.0);
		PRINT_LIST(list1, "list1: ");

		for (size_t idx = 0; idx < 16; idx++) {
			value_t value = (value_t)(rand()%100);
			setsValid = setsValid && linked_list_set(list1, idx, value) == 555.0;
			getsValid = getsValid && linked_list_get(list1, idx) == value;
		}
		PRINT_LIST(list1, "list1: ");

		TEST(setsValid, "value(s) returned after set on 555.0-initialized list is NOT 555.0");
		TEST(getsValid, "value(s) returned from get on list is NOT identical");
	}
}

void test_extra_functionality(size_t* const success, size_t* const total)
{
	linked_list _list1;
	linked_list _list2;
	linked_list _list3;
	linked_list* list1 = &_list1;
	linked_list* list2 = &_list2;
	linked_list* list3 = &_list3;
	value_t values[32];

	linked_list_init(list1);
	linked_list_init(list2);
	linked_list_init(list3);

	for (size_t idx = 0; idx < 32; idx++) {
		value_t value = (value_t)(rand()%100);
		values[idx] = value;
		linked_list_push_back(list1, value);
	}
	linked_list_copy(list2, list1); // list1 - even
	linked_list_pop_back(list2); // list2 - odd (values[31] popped)
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");

	// test reverse, sort for even-list
	{
		linked_list_reverse(list1);
		PRINT_LIST(list1, " list1: ");
		TEST(linked_list_size(list1) == 32, "reversed even list size is NOT 32");
		TEST(linked_list_front(list1) == values[31], "reversed even list front is NOT values[31]");
		TEST(linked_list_back(list1) == values[0], "reversed even list back is NOT values[0]");
		TEST(linked_list_get(list1, 15) == values[16], "reversed even list center-left is NOT values[16]");
		TEST(linked_list_get(list1, 16) == values[15], "reversed even list center-right is NOT values[15]");

		{
			bool allEqual = true;
			PRINT_LIST(list1, " list1: ");
			PRINT_ARRAY(values, "values: ");

			for (size_t idx = 0; idx < 32 && allEqual; idx++)
				if (linked_list_get(list1, idx) != values[31 - idx])
					allEqual = false;

			TEST(allEqual, "reversed even list not equal to reversed values array");
		}

		linked_list_sort(list1, less_than_comparator);
		PRINT_LIST(list1, " list1: ");

		{
			bool allEqual = true;
			value_t sortedValues[32];

			memcpy(sortedValues, values, 32*sizeof(value_t));
			qsort(sortedValues, 32, sizeof(value_t), less_than_comparator_qsort);
			PRINT_LIST(list1,         " list1: ");
			PRINT_ARRAY(sortedValues, "values: ");

			for (size_t idx = 0; idx < 32 && allEqual; idx++)
				if (linked_list_get(list1, idx) != sortedValues[idx])
					allEqual = false;

			TEST(allEqual, "reversed sorted even list not equal to reversed sorted values array");
		}
	}

	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	linked_list_swap(list1, list2);
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	TEST(linked_list_size(list1) == 31, "list1 after swap size is NOT 31");
	TEST(linked_list_size(list2) == 32, "list2 after swap size is NOT 32");

	// test swap, reverse, sort for odd-list
	{
		linked_list_reverse(list1);
		PRINT_LIST(list1, " list1: ");
		TEST(linked_list_size(list1) == 31, "reversed odd list size is NOT 31");
		TEST(linked_list_front(list1) == values[30], "reversed odd list front is NOT values[31]");
		TEST(linked_list_back(list1) == values[0], "reversed odd list back is NOT values[0]");
		TEST(linked_list_get(list1, 15) == values[15], "reversed odd list center is NOT values[15]");

		{
			bool allEqual = true;
			value_t _values[31];
			memcpy(_values, values, 31*sizeof(value_t));

			PRINT_LIST(list1, " list1: ");
			PRINT_ARRAY(_values, "values: ");

			for (size_t idx = 0; idx < 31 && allEqual; idx++)
				if (linked_list_get(list1, idx) != _values[30 - idx])
					allEqual = false;

			TEST(allEqual, "reversed odd list not equal to reversed values array");
		}


		linked_list_sort(list1, less_than_comparator);

		{
			bool allEqual = true;
			value_t sortedValues[31];

			memcpy(sortedValues, values, 31*sizeof(value_t));
			qsort(sortedValues, 31, sizeof(value_t), less_than_comparator_qsort);
			PRINT_LIST(list1, " list1: ");
			PRINT_ARRAY(sortedValues, "values: ");

			for (size_t idx = 0; idx < 31 && allEqual; idx++)
				if (linked_list_get(list1, idx) != sortedValues[idx])
					allEqual = false;

			TEST(allEqual, "reversed sorted odd list not equal to reversed sorted values array");
		}
	}

	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	linked_list_append(list1, list2);
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	TEST(linked_list_size(list1) == 63, "size of list1 after list2 append is NOT 63");
	TEST(linked_list_size(list2) == 0, "size of list2 after list2 append is NOT 0");

	linked_list_append(list1, list2);
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	TEST(linked_list_size(list1) == 63, "size of list1 after appended list2 append is NOT 63");
	TEST(linked_list_size(list2) == 0, "size of list2 after appended list2 append is NOT 0");

	linked_list_append(list2, list1);
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");
	TEST(linked_list_size(list1) == 0, "size of list1 after list1 append is NOT 0");
	TEST(linked_list_size(list2) == 63, "size of list2 after list1 append is NOT 63");

	linked_list_clear(list1);
	linked_list_clear(list2);
	PRINT_LIST(list1, " list1: ");
	PRINT_LIST(list2, " list2: ");

	{
		value_t expectedSum = 0;

		for (size_t idx = 0; idx < 10; idx++) {
			value_t value = (value_t)(rand()%100);
			expectedSum += value;
			linked_list_push_back(list1, value);
		}
		PRINT_LIST(list1, " list1: ");

		get_sum(true);
		linked_list_foreach(list1, sum_list);

		#ifdef DEBUG_OUTPUT
			printf("[Line %4d]      sum: %f\n", __LINE__, *get_sum(false));
			printf("[Line %4d] expected: %f\n", __LINE__, expectedSum);
		#endif

		TEST(*get_sum(false) == expectedSum, "sum of list is NOT expected sum");
	}
}

void test_iterator_interface(size_t* const success, size_t* const total)
{
}

void test_extra_iterator_functionality(size_t* const success, size_t* const total)
{
}

bool less_than_comparator(const value_t* left, const value_t* right)
{
	return *left <= *right;
}

int less_than_comparator_qsort(const void* left, const void* right)
{
	return (*(const value_t*)left)-(*(const value_t*)right);
}

void sum_list(const value_t* value)
{
	*get_sum(false) += *value;
}

value_t* get_sum(bool reset)
{
	static value_t sum;

	if (reset)
		sum = 0;

	return &sum;
}

void print_value(const value_t* value)
{
	printf("%3.0f ", *value);
}

void print_array(value_t* arr, size_t size, const char* const preMsg, int line)
{
	printf("[Line %4d]: %s", line, preMsg);

	for (size_t idx = 0; idx < size; idx++)
		print_value(&arr[idx]);

	printf("\n");
}

void print_list(linked_list* list, const char* const preMsg, int line)
{
	node* iter;
	size_t idx;

	printf("[Line %4d]: %s", line, preMsg);

	for (iter = list->first, idx = 0; idx < list->size && iter != NULL; idx++, iter = iter->next)
		print_value(&iter->value);

	printf("\n");
}
