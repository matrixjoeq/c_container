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

#ifndef __C_SET_H__
#define __C_SET_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_tree.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef c_tree_t c_set_t;
typedef c_tree_iterator_t c_set_iterator_t;

/**
 * constructor/destructor
 */
c_set_t* c_set_create(c_containable_t* key_type, c_compare key_comp);
void c_set_destroy(c_set_t* set);

/**
 * iterators
 */
c_set_iterator_t c_set_begin(c_set_t* set);
c_set_iterator_t c_set_rbegin(c_set_t* set);
c_set_iterator_t c_set_end(c_set_t* set);
c_set_iterator_t c_set_rend(c_set_t* set);

/**
 * capacity
 */
bool c_set_empty(c_set_t* set);
size_t c_set_size(c_set_t* set);
size_t c_set_max_size(void);

/**
 * modifiers
 */
void c_set_clear(c_set_t* set);
c_set_iterator_t c_set_insert_value(c_set_t* set, c_ref_t value);
c_set_iterator_t c_set_insert(c_set_t* set, c_set_iterator_t hint, c_ref_t value);
void c_set_insert_range(c_set_t* set, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_set_insert_from(c_set_t* set, c_ref_t first_value, c_ref_t last_value);
c_set_iterator_t c_set_erase(c_set_t* set, c_set_iterator_t pos);
size_t c_set_erase_key(c_set_t* set, c_ref_t key);
void c_set_erase_range(c_set_t* set, c_set_iterator_t first, c_set_iterator_t last);
void c_set_erase_from(c_set_t* set, c_ref_t first_key, c_ref_t last_key);
void c_set_swap(c_set_t* set, c_set_t* other);

/**
 * operations
 */
c_set_iterator_t c_set_find(c_set_t* set, c_ref_t key);
size_t c_set_count(c_set_t* set, c_ref_t key);
c_set_iterator_t c_set_lower_bound(c_set_t* set, c_ref_t key);
c_set_iterator_t c_set_upper_bound(c_set_t* set, c_ref_t key);
void c_set_equal_range(c_set_t* set, c_ref_t key, c_set_iterator_t** lower, c_set_iterator_t** upper);

/**
 * helpers
 */
#define C_SET(t)       c_set_create((t), (t)->less)
#define C_SET_INT      C_SET(c_get_int_type_info())
#define C_SET_SINT     C_SET(c_get_sint_type_info())
#define C_SET_UINT     C_SET(c_get_uint_type_info())
#define C_SET_SHORT    C_SET(c_get_short_type_info())
#define C_SET_SSHORT   C_SET(c_get_sshort_type_info())
#define C_SET_USHORT   C_SET(c_get_ushort_type_info())
#define C_SET_LONG     C_SET(c_get_long_type_info())
#define C_SET_SLONG    C_SET(c_get_slong_type_info())
#define C_SET_ULONG    C_SET(c_get_ulong_type_info())
#define C_SET_CHAR     C_SET(c_get_char_type_info())
#define C_SET_SCHAR    C_SET(c_get_schar_type_info())
#define C_SET_UCHAR    C_SET(c_get_uchar_type_info())
#define C_SET_FLOAT    C_SET(c_get_float_type_info())
#define C_SET_DOUBLE   C_SET(c_get_double_type_info())


typedef c_tree_t c_multiset_t;
typedef c_tree_iterator_t c_multiset_iterator_t;

/**
 * constructor/destructor
 */
c_multiset_t* c_multiset_create(c_containable_t* key_type, c_compare key_comp);
void c_multiset_destroy(c_multiset_t* multiset);

/**
 * iterators
 */
c_multiset_iterator_t c_multiset_begin(c_multiset_t* multiset);
c_multiset_iterator_t c_multiset_rbegin(c_multiset_t* multiset);
c_multiset_iterator_t c_multiset_end(c_multiset_t* multiset);
c_multiset_iterator_t c_multiset_rend(c_multiset_t* multiset);

/**
 * capacity
 */
bool c_multiset_empty(c_multiset_t* multiset);
size_t c_multiset_size(c_multiset_t* multiset);
size_t c_multiset_max_size(void);

/**
 * modifiers
 */
void c_multiset_clear(c_multiset_t* multiset);
c_multiset_iterator_t c_multiset_insert_value(c_multiset_t* multiset, c_ref_t value);
c_multiset_iterator_t c_multiset_insert(c_multiset_t* multiset, c_multiset_iterator_t hint, c_ref_t value);
void c_multiset_insert_range(c_multiset_t* multiset, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_multiset_insert_from(c_multiset_t* multiset, c_ref_t first_value, c_ref_t last_value);
c_multiset_iterator_t c_multiset_erase(c_multiset_t* multiset, c_multiset_iterator_t pos);
size_t c_multiset_erase_key(c_multiset_t* multiset, c_ref_t key);
void c_multiset_erase_range(c_multiset_t* multiset, c_multiset_iterator_t first, c_multiset_iterator_t last);
void c_multiset_erase_from(c_multiset_t* multiset, c_ref_t first_key, c_ref_t last_key);
void c_multiset_swap(c_multiset_t* multiset, c_multiset_t* other);

/**
 * operations
 */
c_multiset_iterator_t c_multiset_find(c_multiset_t* multiset, c_ref_t key);
size_t c_multiset_count(c_multiset_t* multiset, c_ref_t key);
c_multiset_iterator_t c_multiset_lower_bound(c_multiset_t* multiset, c_ref_t key);
c_multiset_iterator_t c_multiset_upper_bound(c_multiset_t* multiset, c_ref_t key);
void c_multiset_equal_range(c_multiset_t* multiset, c_ref_t key, c_multiset_iterator_t** lower, c_multiset_iterator_t** upper);

/**
 * helpers
 */
#define C_MULTISET(t)       c_multiset_create((t), (t)->less)
#define C_MULTISET_INT      C_MULTISET(c_get_int_type_info())
#define C_MULTISET_SINT     C_MULTISET(c_get_sint_type_info())
#define C_MULTISET_UINT     C_MULTISET(c_get_uint_type_info())
#define C_MULTISET_SHORT    C_MULTISET(c_get_short_type_info())
#define C_MULTISET_SSHORT   C_MULTISET(c_get_sshort_type_info())
#define C_MULTISET_USHORT   C_MULTISET(c_get_ushort_type_info())
#define C_MULTISET_LONG     C_MULTISET(c_get_long_type_info())
#define C_MULTISET_SLONG    C_MULTISET(c_get_slong_type_info())
#define C_MULTISET_ULONG    C_MULTISET(c_get_ulong_type_info())
#define C_MULTISET_CHAR     C_MULTISET(c_get_char_type_info())
#define C_MULTISET_SCHAR    C_MULTISET(c_get_schar_type_info())
#define C_MULTISET_UCHAR    C_MULTISET(c_get_uchar_type_info())
#define C_MULTISET_FLOAT    C_MULTISET(c_get_float_type_info())
#define C_MULTISET_DOUBLE   C_MULTISET(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __C_SET_H__
