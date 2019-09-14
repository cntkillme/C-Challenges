#include <stdlib.h>
#include "table.h"

/* Standard implementations for table_key_t: ArithmeticType, value_key_t: ArithmeticType. */

size_t key_hasher(const table_key_t* key)
{
	return *key;
}

void key_duplicator(table_key_t* target, const table_key_t* key)
{
	*target = *key;
}

void key_destructor(table_key_t* key)
{
	free(key);
}

void value_duplicator(table_value_t* target, const table_value_t* value)
{
	*target = *value;
}

void value_destructor(table_value_t* value)
{
	free(value);
}


/* Table interface implementation */

void table_init(table_t* table);

void table_clear(table_t* table);

size_t table_size(const table_t* table);

table_iter_t table_insert(table_t* table, table_key_t* key, table_value_t* value,
	storage_mode key_storage_mode, storage_mode value_storage_mode);

table_iter_t table_erase(table_t* table, table_iter_t iter);

const table_value_t* table_key(const table_t* table, table_const_iter_t iter);

const table_value_t* table_value(const table_t* table, table_const_iter_t iter);

table_iter_t table_assign(table_t* table, table_iter_t iter, table_value_t* value, storage_mode value_storage_mode);

table_const_iter_t table_find(const table_t* table, const table_key_t* key);
table_iter_t table_find_mut(table_t* table, const table_key_t* key);

table_const_iter_t table_begin(const table_t* table);
table_iter_t table_begin_mut(table_t* table);

table_const_iter_t table_end(const table_t* table);
table_iter_t table_end_mut(table_t* table);

table_const_iter_t table_next(const table_t* table, table_const_iter_t iter);
table_iter_t table_next_mut(const table_t* table, table_iter_t iter);
