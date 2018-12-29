#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define RUN_TESTS(name, func) \
	printf("Testing " name "...\n"); \
	func(&success, &total); \
	printf("%zu/%zu tests passed.\n\n", success, total); \
	totalSuccess += success; \
	totalTotal += total; \
	success = total = 0

#define TEST(expr, msg) TEST_IMPL(expr, msg, __LINE__)

#define TEST_IMPL(expr, msg, line) *total += 1; \
if (expr) \
	*success += 1; \
else \
	printf("Test %zu failed (line %d): %s\n", *total, line, msg)

#ifdef DEBUG_OUTPUT
	#define DEBUG_WRITE(...) printf("[Line %4d]: ", __LINE__); printf(__VA_ARGS__)
	#define PRINT_ARRAY(arr, str) DEBUG_WRITE(str); print_array(arr, sizeof(arr)/sizeof(value_t))
	#define PRINT_LIST(list, str) DEBUG_WRITE(str); print_list(list)
	#define PRINT_VAL(val) printf("%3.0f ", val)
#else
	#define DEBUG_WRITE(...) (void)0
	#define PRINT_ARRAY(arr, str) (void)0
	#define PRINT_LIST(list, str) (void)0
	#define PRINT_VAL(val) (void)0
#endif

static void test_required_interface(size_t* const success, size_t* const total);
static void test_extra_functionality(size_t* const success, size_t* const total);
static void test_iterator_interface(size_t* const success, size_t* const total);
static void test_extra_iterator_functionality(size_t* const success, size_t* const total);

static void print_array(const value_t* arr, size_t size);
static void print_list(const linked_list* list);
static bool less_than_comparator(const value_t* left, const value_t* right);
static int less_than_comparator_qsort(const void* left, const void* right);
static void sum_list(const value_t* value);
static value_t* get_sum(bool reset);

int main(void)
{
	size_t success = 0;
	size_t total = 0;
	size_t totalSuccess = 0;
	size_t totalTotal = 0;

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

	printf("All tests completed, summary: %lu/%lu tests passed.\n", totalSuccess, totalTotal);

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
	TEST(linked_list_back(list1) == 123.0, "empty list back after push_front is NOT 123.0");

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

		DEBUG_WRITE("      sum: %.0f\n", *get_sum(false));
		DEBUG_WRITE(" expected: %.0f\n", expectedSum);

		TEST(*get_sum(false) == expectedSum, "sum of list is NOT expected sum");

		linked_list_clear(list1);
	}
}

void test_iterator_interface(size_t* const success, size_t* const total)
{
	linked_list _list;
	linked_list* list = &_list;

	linked_list_init(list);

	TEST(linked_list_begin(list) == linked_list_end(list), "empty list begin is NOT end");

	linked_list_push_back(list, 5.0);
	PRINT_LIST(list, "list: ");
	TEST(linked_list_begin(list) != linked_list_end(list), "non-empty list begin IS end");
	TEST(linked_list_read(list, linked_list_begin(list)) == 5.0, "value at index 0 is NOT 5.0");
	TEST(linked_list_write(list, linked_list_begin(list), 10.0) == 5.0, "value returned after write at index 0 is NOT 5.0");
	PRINT_LIST(list, "list: ");
	TEST(linked_list_read(list, linked_list_begin(list)) == 10.0, "value at index 0 is NOT 10.0");

	linked_list_push_back(list, 20.0); linked_list_push_back(list, 30.0);
	PRINT_LIST(list, "list: ");
	{
		iter_t iter;

		iter = linked_list_insert(list, linked_list_begin(list), 0.0);
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 4, "size of list is NOT 4");
		TEST(linked_list_front(list) == 0.0, "value at front is NOT 0.0");
		TEST(iter == linked_list_begin(list), "returned iter from insert is NOT first");

		iter = linked_list_insert(list, linked_list_end(list), 50.0);
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 5, "size of list is NOT 5");
		TEST(linked_list_back(list) == 50.0, "value at back is NOT 50.0");

		iter = linked_list_insert(list, iter, 40.0);
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 6, "size of list is NOT 6");
		TEST(linked_list_back(list) == 50.0, "value at back is NOT 50.0");

		linked_list_erase(list, linked_list_begin(list));
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 5, "size of list is NOT 5");
		TEST(linked_list_front(list) == 10.0, "value at front is NOT 10.0");

		iter = linked_list_erase(list, iter);
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 4, "size of list is NOT 4");

		linked_list_erase(list, iter);
		PRINT_LIST(list, "list: ");
		TEST(linked_list_size(list) == 3, "size of list is NOT 3");
		TEST(linked_list_back(list) == 30.0, "value at back is NOT 30.0");
	}

	TEST(linked_list_advance(list,
		linked_list_begin(list), 0) == linked_list_begin(list), "begin + 0 is NOT begin");
	TEST(linked_list_advance(list,
		linked_list_begin(list), 3) == linked_list_end(list), "begin + 3 is NOT end");
	TEST(linked_list_advance(list,
		linked_list_end(list), 0) == linked_list_end(list), "end + 0 is NOT end");
	TEST(linked_list_advance(list,
		linked_list_end(list), -3) == linked_list_begin(list), "end - 3 is NOT begin");
	TEST(linked_list_read(list, linked_list_advance(list,
		linked_list_begin(list), 0)) == 10.0, "begin + 0 value is NOT 10.0");
	TEST(linked_list_read(list, linked_list_advance(list,
		linked_list_begin(list), 1)) == 20.0, "begin + 1 value is NOT 20.0");
	TEST(linked_list_read(list, linked_list_advance(list,
		linked_list_begin(list), 2)) == 30.0, "begin + 2 value is NOT 30.0");
	TEST(linked_list_read(list, linked_list_advance(list,
		linked_list_end(list), -1)) == 30.0, "end - 1 value is NOT 30.0");
	TEST(linked_list_read(list, linked_list_advance(list,
		linked_list_end(list), -2)) == 20.0, "end - 2 value is NOT 20.0");

	DEBUG_WRITE("dist(begin, begin): %td \n", linked_list_dist(list, linked_list_begin(list),
		linked_list_begin(list)));
	TEST(linked_list_dist(list, linked_list_begin(list),
		linked_list_advance(list,
			linked_list_begin(list), 0)) == 0, "dist(begin, begin) is NOT 0");

	DEBUG_WRITE("dist(begin, begin + 1): %td \n", linked_list_dist(list, linked_list_begin(list),
		linked_list_advance(list,
			linked_list_begin(list), 1)));
	TEST(linked_list_dist(list, linked_list_begin(list),
		linked_list_advance(list,
			linked_list_begin(list), 1)) == 1, "dist(begin, begin + 1) is NOT 1");

	DEBUG_WRITE("dist(begin, end): %td \n", linked_list_dist(list, linked_list_begin(list),
		linked_list_end(list)));
	TEST(linked_list_dist(list, linked_list_begin(list),
		linked_list_end(list)) == 3, "dist(begin, end) is NOT 3");

	DEBUG_WRITE("dist(end, end): %td \n", linked_list_dist(list, linked_list_end(list),
		linked_list_end(list)));
	TEST(linked_list_dist(list, linked_list_end(list),
		linked_list_advance(list,
			linked_list_end(list), 0)) == 0, "dist(end, end) is NOT 0");

	DEBUG_WRITE("dist(end, end - 1): %td \n", linked_list_dist(list, linked_list_end(list),
		linked_list_advance(list,
			linked_list_end(list), -1)));
	TEST(linked_list_dist(list, linked_list_end(list),
		linked_list_advance(list,
			linked_list_end(list), -1)) == -1, "dist(end, end - 1) is NOT -1");

	DEBUG_WRITE("dist(end, begin): %td \n", linked_list_dist(list, linked_list_end(list),
		linked_list_begin(list)));
	TEST(linked_list_dist(list, linked_list_end(list),
		linked_list_begin(list)) == -3, "dist(end, begin) is NOT -3");

	DEBUG_WRITE("dist(begin + 1, begin + 2): %td \n", linked_list_dist(list,
		linked_list_advance(list, linked_list_begin(list), 1),
		linked_list_advance(list, linked_list_begin(list), 2)));
	TEST(linked_list_dist(list,
		linked_list_advance(list, linked_list_begin(list), 1),
		linked_list_advance(list, linked_list_begin(list), 2)) == 1, "dist(begin + 1, begin + 2) is NOT 1");

	DEBUG_WRITE("dist(begin + 2, begin + 1): %td \n", linked_list_dist(list,
		linked_list_advance(list, linked_list_begin(list), 2),
		linked_list_advance(list, linked_list_begin(list), 1)));
	TEST(linked_list_dist(list,
		linked_list_advance(list, linked_list_begin(list), 2),
		linked_list_advance(list, linked_list_begin(list), 1)) == -1, "dist(begin + 2, begin + 1) is NOT -1");

	DEBUG_WRITE("dist(end - 2, end - 1): %td \n", linked_list_dist(list,
		linked_list_advance(list, linked_list_end(list), -2),
		linked_list_advance(list, linked_list_end(list), -1)));
	TEST(linked_list_dist(list,
		linked_list_advance(list, linked_list_end(list), -2),
		linked_list_advance(list, linked_list_end(list), -1)) == 1, "dist(end - 2, end - 1) is NOT 1");

	DEBUG_WRITE("dist(end - 1, end - 2): %td \n", linked_list_dist(list,
		linked_list_advance(list, linked_list_end(list), -1),
		linked_list_advance(list, linked_list_end(list), -2)));
	TEST(linked_list_dist(list,
		linked_list_advance(list, linked_list_end(list), -1),
		linked_list_advance(list, linked_list_end(list), -2)) == -1, "dist(end - 1, end - 2) is NOT -1");

	linked_list_clear(list);
}

void test_extra_iterator_functionality(size_t* const success, size_t* const total)
{
}

void print_array(const value_t* arr, size_t size)
{
	for (size_t idx = 0; idx < size; idx++)
		PRINT_VAL(arr[idx]);
	printf("\n");
}

void print_list(const linked_list* list)
{
	node* iter;
	size_t idx;

	for (iter = list->first, idx = 0; idx < list->size && iter != NULL; idx++, iter = iter->next)
		PRINT_VAL(iter->value);
	printf("\n");
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
