/**
 * MIT License
 *
 * Copyright (c) 2017 MatrixJoeQ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __C_MAP_H__
#define __C_MAP_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_tree.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* map */
typedef c_tree_t c_map_t;
typedef c_tree_iterator_t c_map_iterator_t;

/**
 * constructor/destructor
 */
c_map_t* c_map_create(c_type_info_t* key_type, c_type_info_t* value_type, c_compare key_comp);
void c_map_destroy(c_map_t* map);

/**
 * iterators
 */
c_map_iterator_t c_map_begin(c_map_t* map);
c_map_iterator_t c_map_rbegin(c_map_t* map);
c_map_iterator_t c_map_end(c_map_t* map);
c_map_iterator_t c_map_rend(c_map_t* map);

/**
 * capacity
 */
bool c_map_empty(c_map_t* map);
size_t c_map_size(c_map_t* map);
size_t c_map_max_size(void);

/**
 * modifiers
 */
void c_map_clear(c_map_t* map);
c_map_iterator_t c_map_insert_value(c_map_t* map, c_ref_t value);
c_map_iterator_t c_map_insert(c_map_t* map, c_map_iterator_t hint, c_ref_t value);
void c_map_insert_range(c_map_t* map, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_map_insert_from(c_map_t* map, c_ref_t first_value, c_ref_t last_value);
c_map_iterator_t c_map_erase(c_map_t* map, c_map_iterator_t pos);
size_t c_map_erase_key(c_map_t* map, c_ref_t key);
void c_map_erase_range(c_map_t* map, c_map_iterator_t first, c_map_iterator_t last);
void c_map_erase_from(c_map_t* map, c_ref_t first_key, c_ref_t last_key);
void c_map_swap(c_map_t* map, c_map_t* other);

/**
 * operations
 */
c_map_iterator_t c_map_find(c_map_t* map, c_ref_t key);
size_t c_map_count(c_map_t* map, c_ref_t key);
c_map_iterator_t c_map_lower_bound(c_map_t* map, c_ref_t key);
c_map_iterator_t c_map_upper_bound(c_map_t* map, c_ref_t key);
void c_map_equal_range(c_map_t* map, c_ref_t key, c_map_iterator_t** lower, c_map_iterator_t** upper);

/**
 * helpers
 */
#define C_MAP(k, v)    c_map_create((k), (v), (k)->less)

/* multimap */
typedef c_tree_t c_multimap_t;
typedef c_tree_iterator_t c_multimap_iterator_t;

/**
 * constructor/destructor
 */
c_multimap_t* c_multimap_create(c_type_info_t* key_type, c_type_info_t* value_type, c_compare key_comp);
void c_multimap_destroy(c_multimap_t* multimap);

/**
 * iterators
 */
c_multimap_iterator_t c_multimap_begin(c_multimap_t* multimap);
c_multimap_iterator_t c_multimap_rbegin(c_multimap_t* multimap);
c_multimap_iterator_t c_multimap_end(c_multimap_t* multimap);
c_multimap_iterator_t c_multimap_rend(c_multimap_t* multimap);

/**
 * capacity
 */
bool c_multimap_empty(c_multimap_t* multimap);
size_t c_multimap_size(c_multimap_t* multimap);
size_t c_multimap_max_size(void);

/**
 * modifiers
 */
void c_multimap_clear(c_multimap_t* multimap);
c_multimap_iterator_t c_multimap_insert_value(c_multimap_t* multimap, c_ref_t value);
c_multimap_iterator_t c_multimap_insert(c_multimap_t* multimap, c_multimap_iterator_t hint, c_ref_t value);
void c_multimap_insert_range(c_multimap_t* multimap, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_multimap_insert_from(c_multimap_t* multimap, c_ref_t first_value, c_ref_t last_value);
c_multimap_iterator_t c_multimap_erase(c_multimap_t* multimap, c_multimap_iterator_t pos);
size_t c_multimap_erase_key(c_multimap_t* multimap, c_ref_t key);
void c_multimap_erase_range(c_multimap_t* multimap, c_multimap_iterator_t first, c_multimap_iterator_t last);
void c_multimap_erase_from(c_multimap_t* multimap, c_ref_t first_key, c_ref_t last_key);
void c_multimap_swap(c_multimap_t* multimap, c_multimap_t* other);

/**
 * operations
 */
c_multimap_iterator_t c_multimap_find(c_multimap_t* multimap, c_ref_t key);
size_t c_multimap_count(c_multimap_t* multimap, c_ref_t key);
c_multimap_iterator_t c_multimap_lower_bound(c_multimap_t* multimap, c_ref_t key);
c_multimap_iterator_t c_multimap_upper_bound(c_multimap_t* multimap, c_ref_t key);
void c_multimap_equal_range(c_multimap_t* multimap, c_ref_t key, c_multimap_iterator_t** lower, c_multimap_iterator_t** upper);

/**
 * helpers
 */
#define C_MULTIMAP(k, v)    c_multimap_create((k), (v), (k)->less)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __C_MAP_H__
