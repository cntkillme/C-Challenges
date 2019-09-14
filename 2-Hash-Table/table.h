#pragma once

#include <stddef.h>

/**
 * The table_t type implements a hash table.
 *
 * Implementation-defined specification:
 *   table_key_t: Type definition for keys in a table.
 *   value_type_t: Type definition for values in a table.
 *   key_hasher: Function that maps an instance of table_key_t to an unsigned integral.
 *   key_duplicator: Function that duplicates an instance of table_key_t.
 *   key_destructor: Function that frees all memory associated with an instance of table_key_t.
 *   value_duplicator: Function that duplicates an instance of table_value_t.
 *   value_destructor: Function that frees all memory associated with an instance of table_value_t.
 *   table_t: Hash table structure type definition.
 *
 * Expectations:
 *   The memory belonging to the keys and values of a table may be marked as TRANSIENT or STATIC.
 *    TRANSIENT specifies that a copy of the key or value will be made and its memory will be managed by the table.
 *    STATIC specifies that no copy of the key or value will be made and its memory must be managed by the user.
 *    TRANSFER specifies that no copy of the key or value will be made and its memory will be managed by the table.
 *   The public interface ought not be mutated! The implementation may define more functions but must not alter the
 *    signature of any functions beloning to the public interface.
 *   Table and table nodes are not to be accessed or manipulated outside the interface.
 *
 * There is no expectation for any particular hash table implementation to handle collisions, however collisions
 *  MUST be handled by the implementation.
 *
 * All functions aside from table_init expect an initialized table, providing an uninitialized table shall be undefined
 *  behavior.
 * The function table_init expects an uninitialized table, providing an initialized table shall be undefined behavior.
 * Passing iterators to functions that are invalidated (from removing, rehashing, etc.) shall be undefined behavior.
 */

typedef int table_key_t;
typedef int table_value_t;

typedef size_t(table_key_hasher)(const table_key_t* key);
typedef void(table_key_duplicator_t)(table_key_t* target, const table_key_t* key);
typedef void(table_key_destructor_t)(table_key_t* key);
typedef void(table_value_duplicator_t)(table_value_t* target, const table_value_t* value);
typedef void(table_value_destructor_t)(table_value_t* value);

extern size_t key_hasher(const table_key_t* key);
extern void key_duplicator(table_key_t* target, const table_key_t* key);
extern void key_destructor(table_key_t* key);
extern void value_duplicator(table_value_t* target, const table_value_t* value);
extern void value_destructor(table_value_t* value);

typedef enum storage_mode
{
	TRANSIENT, STATIC, TRANSFER
} storage_mode;

typedef struct table_node_t
{
	const table_key_t* key;
	const storage_mode key_storage_type;

	table_value_t* value;
	storage_mode value_storage_type;
} table_node_t;

typedef table_node_t* table_iter_t;
typedef const table_node_t* table_const_iter_t;

typedef struct table_t
{
	/* implementation defined, use table_node_t for entries */
} table_t;

/**
 * Initializes the given table.
 * @param table A pointer to an uninitialized table.
 */
void table_init(table_t* table);

/**
 * Clears all entries of the given table.
 * @param table A pointer to an initialized table.
 */
void table_clear(table_t* table);

/**
 * Returns the number of entries in the given table.
 * @param table A pointer to an initialized table.
 * @return The number of entries (key-value pairs) residing in the table.
 */
size_t table_size(const table_t* table);

/**
 * Inserts a key-value pair into the table.
 * @param table A pointer to an initialized table.
 * @param key A pointer to the key.
 * @param value A pointer to the value.
 * @param key_storage_mode The storage mode of the key.
 * @param value_storage_mode The storage mode of the value.
 * @return The iterator of the newly inserted entry, table_end(table) on failure.
 */
table_iter_t table_insert(table_t* table, table_key_t* key, table_value_t* value,
	storage_mode key_storage_mode, storage_mode value_storage_mode);

/**
 * Erases a key-value pair from the table.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry to remove.
 * @return The iterator of the next entry, table_end(table) if at end.
 */
table_iter_t table_erase(table_t* table, table_iter_t iter);

/**
 * Returns the key associated with an iterator.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry to access.
 * @return A pointer to the key associated with the iterator.
 */
const table_value_t* table_key(const table_t* table, table_const_iter_t iter);

/**
 * Returns the value associated with an iterator.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry to access.
 * @return A pointer to the value associated with the iterator.
 */
const table_value_t* table_value(const table_t* table, table_const_iter_t iter);

/**
 * Assigns a new value to the iterator.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry to mutate.
 * @param value A pointer to the new value.
 * @param value_storage_mode The storage mode of the new value.
 * @return The iterator passed to the function.
 */
table_iter_t table_assign(table_t* table, table_iter_t iter, table_value_t* value, storage_mode value_storage_mode);

/**
 * Returns the iterator associated to an entry.
 * @param table A pointer to an initialized table.
 * @param key A pointer to the key.
 * @return The iterator of the entry if found, table_end(table) on failure.
 */
table_const_iter_t table_find(const table_t* table, const table_key_t* key);

/**
 * Returns the mutable iterator associated to an entry.
 * @param table A pointer to an initialized table.
 * @param key A pointer to the key.
 * @return The iterator of the entry if found, table_end(table) on failure.
 */
table_iter_t table_find_mut(table_t* table, const table_key_t* key);

/**
 * Returns the iterator associated to the first entry in a table.
 * @param table A pointer to an initialized table.
 * @return The iterator of the first entry, table_end(table) on empty table.
 */
table_const_iter_t table_begin(const table_t* table);

/**
 * Returns the mutable iterator associated to the first entry in a table.
 * @param table A pointer to an initialized table.
 * @return The iterator of the first entry, table_end(table) on empty table.
 */
table_iter_t table_begin_mut(table_t* table);

/**
 * Returns the iterator associated to the end of a table.
 * @param table A pointer to an initialized table.
 * @return <implementation-defined>
 */
table_const_iter_t table_end(const table_t* table);

/**
 * Returns the iterator proceeding a given iterator.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry.
 * @return The iterator proceeding the given iterator, table_end(table) on end reached.
 */
table_const_iter_t table_next(const table_t* table, table_const_iter_t iter);

/**
 * Returns the mutable iterator proceeding a given iterator.
 * @param table A pointer to an initialized table.
 * @param iter A valid iterator excluding table_end(table) to the entry.
 * @return The iterator proceeding the given iterator, table_end(table) on end reached.
 */
table_iter_t table_next_mut(const table_t* table, table_iter_t iter);
