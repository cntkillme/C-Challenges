#include <stdio.h>
#include <stdlib.h>
#include "table.h"

#define TEST(expr) TEST_IMPL(expr, #expr, __LINE__)

#define TEST_IMPL(expr, msg, line) total += 1; \
if (expr) \
	success += 1; \
else \
	printf("Test %zu failed (line %d): %s\n", total, line, msg)

#ifdef DEBUG_OUTPUT
	#define DEBUG_WRITE(...) printf("[Line %4d]: ", __LINE__); printf(__VA_ARGS__)
#else
	#define DEBUG_WRITE(...) (void)0
#endif

static table_key_t* key_new(table_key_t key);
static table_value_t* value_new(table_value_t value);

int main(void)
{
	size_t success = 0;
	size_t total = 0;
	table_t _table;
	table_t* table = &_table;

	table_init(table);

	TEST(table_size(table) == 0);

	table_clear(table);
	TEST(table_size(table) == 0);

	TEST(table_begin(table) == table_begin_mut(table));
	TEST(table_end(table) == table_begin(table));

	{
		table_iter_t iter;
		table_const_iter_t constiter;

		table_key_t* k = key_new(5);
		table_value_t* v = value_new(100);

		iter = table_insert(table, k, v, TRANSFER, TRANSFER);
		TEST(iter != table_end(table));
		TEST(table_key(table, iter) == k);
		TEST(table_value(table, iter) == v);

		iter = table_insert(table, k, v, TRANSFER, TRANSFER); // dangerous but should fail below
		TEST(iter == table_end(table));

		TEST(table_erase(table, iter) == table_end(table));

		k = key_new(10);
		v = value_new(200);

		iter = table_insert(table, k, v, TRANSFER, TRANSIENT);
		TEST(iter != table_end(table));
		TEST(table_key(table, iter) == k);
		TEST(table_value(table, iter) != v);

		free(v); // k is no longer user-managed

		constiter = table_find(table, k);
		TEST(iter == constiter);

		v = value_new(250);
		iter = table_find_mut(table, k);
		TEST(iter == constiter);
		iter = table_assign(table, iter, v, STATIC);
		TEST(table_value(table, constiter) == v);
		iter = table_assign(table, iter, value_new(300), TRANSFER);
		TEST(iter == constiter);

		free(v);

		TEST(table_erase(table, iter));
	}

	printf("All tests completed, summary: %lu/%lu tests passed.\n", success, total);
	return 0;
}

static table_key_t* key_new(table_key_t key)
{
	table_key_t* k = (table_key_t*)malloc(sizeof(table_key_t));
	*k = key;

	return k;
}

static table_value_t* value_new(table_value_t value)
{
	table_value_t* v = (table_value_t*)malloc(sizeof(table_key_t));
	*v = value;

	return v;
}
