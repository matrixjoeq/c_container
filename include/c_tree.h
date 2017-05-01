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

#ifndef __C_TREE_H__
#define __C_TREE_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_tree;
struct __c_tree_node;
struct __c_backend_tree;

typedef struct __c_tree c_tree_t;
typedef struct __c_tree_node c_tree_node_t;
typedef struct __c_backend_tree c_backend_tree_t;

typedef struct __c_tree_iterator {
    c_iterator_t base_iter;
    c_tree_node_t* node;
} c_tree_iterator_t;

/**
 * constructor/destructor
 */
c_tree_t* c_tree_create(c_containable_t* key_type,
                        c_containable_t* value_type,
                        c_key_of_value key_of_value,
                        c_compare key_comp);
void c_tree_destroy(c_tree_t* tree);

/**
 * iterators
 */
c_tree_iterator_t c_tree_begin(c_tree_t* tree);
c_tree_iterator_t c_tree_rbegin(c_tree_t* tree);
c_tree_iterator_t c_tree_end(c_tree_t* tree);
c_tree_iterator_t c_tree_rend(c_tree_t* tree);

/**
 * capacity
 */
bool c_tree_empty(c_tree_t* tree);
size_t c_tree_size(c_tree_t* tree);
size_t c_tree_max_size(void);

/**
 * modifiers
 */
void c_tree_clear(c_tree_t* tree);
c_tree_iterator_t c_tree_insert_unique_value(c_tree_t* tree, c_ref_t value);
c_tree_iterator_t c_tree_insert_unique(c_tree_t* tree, c_tree_iterator_t hint, c_ref_t value);
void c_tree_insert_unique_range(c_tree_t* tree, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_tree_insert_unique_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value);
c_tree_iterator_t c_tree_insert_equal_value(c_tree_t* tree, c_ref_t value);
c_tree_iterator_t c_tree_insert_equal(c_tree_t* tree, c_tree_iterator_t hint, c_ref_t value);
void c_tree_insert_equal_range(c_tree_t* tree, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last);
void c_tree_insert_equal_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value);
c_tree_iterator_t c_tree_erase(c_tree_t* tree, c_tree_iterator_t pos);
size_t c_tree_erase_key(c_tree_t* tree, c_ref_t key);
void c_tree_erase_range(c_tree_t* tree, c_tree_iterator_t first, c_tree_iterator_t last);
void c_tree_erase_from(c_tree_t* tree, c_ref_t first_key, c_ref_t last_key);
void c_tree_swap(c_tree_t* tree, c_tree_t* other);

/**
 * operations
 */
c_tree_iterator_t c_tree_find(c_tree_t* tree, c_ref_t key);
size_t c_tree_count(c_tree_t* tree, c_ref_t key);
c_tree_iterator_t c_tree_lower_bound(c_tree_t* tree, c_ref_t key);
c_tree_iterator_t c_tree_upper_bound(c_tree_t* tree, c_ref_t key);
void c_tree_equal_range(c_tree_t* tree, c_ref_t key, c_tree_iterator_t** lower, c_tree_iterator_t** upper);

/**
 * debugging
 */
bool c_tree_rb_verify(c_tree_t* tree);

/**
 * helpers
 */
#define C_TREE_BASE(k, v, kov, c)   c_tree_create((k), (v), (kov), (c))

#define C_TREE(t)       C_TREE_BASE((t), (t), __c_identity, (t)->less)
#define C_TREE_INT      C_TREE(c_get_int_type_info())
#define C_TREE_SINT     C_TREE(c_get_sint_type_info())
#define C_TREE_UINT     C_TREE(c_get_uint_type_info())
#define C_TREE_SHORT    C_TREE(c_get_short_type_info())
#define C_TREE_SSHORT   C_TREE(c_get_sshort_type_info())
#define C_TREE_USHORT   C_TREE(c_get_ushort_type_info())
#define C_TREE_LONG     C_TREE(c_get_long_type_info())
#define C_TREE_SLONG    C_TREE(c_get_slong_type_info())
#define C_TREE_ULONG    C_TREE(c_get_ulong_type_info())
#define C_TREE_CHAR     C_TREE(c_get_char_type_info())
#define C_TREE_SCHAR    C_TREE(c_get_schar_type_info())
#define C_TREE_UCHAR    C_TREE(c_get_uchar_type_info())
#define C_TREE_FLOAT    C_TREE(c_get_float_type_info())
#define C_TREE_DOUBLE   C_TREE(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __C_TREE_H__
