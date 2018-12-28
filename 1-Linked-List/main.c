#include <stdio.h>
#include "linked_list.h"

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

	TEST(linked_list_size(list1) == 12, "resized list size (to 12) is NOT 12");
	TEST(linked_list_front(list1) == 5.0, "resized list front element NOT 5.0");
	TEST(linked_list_back(list1) == 5.0, "resized list back element NOT 5.0");

	linked_list_copy(list2, list1);
	TEST(linked_list_size(list2) == 12, "copied list size is NOT 12");
	TEST(linked_list_front(list2) == 5.0, "copied list front element NOT 5.0");
	TEST(linked_list_back(list2) == 5.0, "copied list back element NOT 5.0");

	linked_list_resize(list2, 4, 0.0);
	TEST(linked_list_size(list2) == 4, "list size after down-size is NOT 4");
	TEST(linked_list_front(list2) == 5.0, "list front after down-size is NOT 5.0");
	TEST(linked_list_back(list2) == 5.0, "list back after down-size is NOT 5.0");

	linked_list_clear(list1);
	TEST(linked_list_size(list1) == 0, "cleared list size is NOT 0");
	TEST(linked_list_size(list2) == 4, "copied list is NOT independent");

	linked_list_push_front(list1, 123.0); // 123.0
	TEST(linked_list_size(list1) == 1, "empty list size after push_front is NOT 1");
	TEST(linked_list_front(list1) == 123.0, "empty list front after push_front is NOT 123.0");
	TEST(linked_list_back(list1) == 123.0, "empty list back after push_front is NOT 123.0");

	linked_list_push_front(list1, 456.0); // 456.0, 123.0
	TEST(linked_list_size(list1) == 2, "list size after second push_front is NOT 2");
	TEST(linked_list_front(list1) == 456.0, "list front after second push_front is NOT 456.0");
	TEST(linked_list_back(list1) == 123.0, "list back after second push_front is NOT 123.0");

	linked_list_push_back(list1, 999.0); // 456.0, 123.0, 999.0
	TEST(linked_list_size(list1) == 3, "list size after three pushes is NOT 3");
	TEST(linked_list_front(list1) == 456.0, "list front is NOT 456.0");
	TEST(linked_list_back(list1) == 999.0, "list back is NOT 999.0");

	// 123.0, 999.0
	TEST(linked_list_pop_front(list1) == 456.0, "value popped from front is NOT 456.0");
	TEST(linked_list_size(list1) == 2, "list size after pop is NOT 2");
	TEST(linked_list_front(list1) == 123.0, "list front after pop is NOT 123.0");
	TEST(linked_list_back(list1) == 999.0, "list back after pop is NOT 999.0");

	// 123.0
	TEST(linked_list_pop_back(list1) == 999.0, "value popped from back is NOT 999.0");
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
}

void test_extra_functionality(size_t* const success, size_t* const total)
{
}

void test_iterator_interface(size_t* const success, size_t* const total)
{
}

void test_extra_iterator_functionality(size_t* const success, size_t* const total)
{
}
