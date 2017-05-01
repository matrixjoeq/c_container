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

#ifndef __C_LIST_H__
#define __C_LIST_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_list;
struct __c_list_node;
struct __c_backend_list;

typedef struct __c_list c_list_t;
typedef struct __c_backend_list c_backend_list_t;

typedef struct __c_list_iterator {
    c_iterator_t base_iter;
    struct __c_list_node* node;
} c_list_iterator_t;

/**
 * constructor/destructor
 */
c_list_t* c_list_create(c_containable_t* type_info);
c_list_t* c_list_create_from(c_containable_t* type_info, c_ref_t values, size_t length);
c_list_t* c_list_copy(c_list_t* other);
c_list_t* c_list_assign(c_list_t* self, c_list_t* other);
void c_list_destroy(c_list_t* list);

/**
 * element access
 */
c_ref_t c_list_front(c_list_t* list);
c_ref_t c_list_back(c_list_t* list);

/**
 * iterators
 */
c_list_iterator_t c_list_begin(c_list_t* list);
c_list_iterator_t c_list_rbegin(c_list_t* list);
c_list_iterator_t c_list_end(c_list_t* list);
c_list_iterator_t c_list_rend(c_list_t* list);

/**
 * capacity
 */
bool c_list_empty(c_list_t* list);
size_t c_list_size(c_list_t* list);
size_t c_list_max_size(void);

/**
 * modifiers
 */
void c_list_clear(c_list_t* list);
c_list_iterator_t c_list_insert(c_list_t* list, c_list_iterator_t pos, c_ref_t value);
c_list_iterator_t c_list_erase(c_list_t* list, c_list_iterator_t pos);
c_list_iterator_t c_list_erase_range(c_list_t* list, c_list_iterator_t first, c_list_iterator_t last);
void c_list_push_back(c_list_t* list, c_ref_t value);
void c_list_pop_back(c_list_t* list);
void c_list_push_front(c_list_t* list, c_ref_t value);
void c_list_pop_front(c_list_t* list);
void c_list_resize(c_list_t* list, size_t count);
void c_list_resize_with_value(c_list_t* list, size_t count, c_ref_t value);
void c_list_swap(c_list_t* list, c_list_t* other);

/**
 * operations
 */
void c_list_merge(c_list_t* list, c_list_t* other);
void c_list_merge_by(c_list_t* list, c_list_t* other, c_compare comp);
void c_list_splice(c_list_t* list, c_list_iterator_t pos, c_list_t* other);
void c_list_splice_from(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t from);
void c_list_splice_range(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t first, c_list_iterator_t last);
void c_list_remove(c_list_t* list, c_ref_t value);
void c_list_remove_if(c_list_t* list, c_unary_predicate pred);
void c_list_sort(c_list_t* list);
void c_list_sort_by(c_list_t* list, c_compare comp);
void c_list_reverse(c_list_t* list);
void c_list_unique(c_list_t* list);
void c_list_unique_if(c_list_t* list, c_binary_predicate pred);

/**
 * backend
 */
c_backend_container_t* c_list_create_backend(c_containable_t* type_info);

/**
 * helpers
 */
#define C_LIST_INT      c_list_create(c_get_int_type_info())
#define C_LIST_SINT     c_list_create(c_get_sint_type_info())
#define C_LIST_UINT     c_list_create(c_get_uint_type_info())
#define C_LIST_SHORT    c_list_create(c_get_short_type_info())
#define C_LIST_SSHORT   c_list_create(c_get_sshort_type_info())
#define C_LIST_USHORT   c_list_create(c_get_ushort_type_info())
#define C_LIST_LONG     c_list_create(c_get_long_type_info())
#define C_LIST_SLONG    c_list_create(c_get_slong_type_info())
#define C_LIST_ULONG    c_list_create(c_get_ulong_type_info())
#define C_LIST_CHAR     c_list_create(c_get_char_type_info())
#define C_LIST_SCHAR    c_list_create(c_get_schar_type_info())
#define C_LIST_UCHAR    c_list_create(c_get_uchar_type_info())
#define C_LIST_FLOAT    c_list_create(c_get_float_type_info())
#define C_LIST_DOUBLE   c_list_create(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_LIST_H__
