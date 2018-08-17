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

#ifndef __C_FORWARD_LIST_H__
#define __C_FORWARD_LIST_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_slist;
struct __c_slist_node;

typedef struct __c_slist c_slist_t;

typedef struct __c_slist_iterator {
    c_iterator_t base_iter;
    struct __c_slist_node* node;
} c_slist_iterator_t;

/**
 * constructor/destructor
 */
c_slist_t* c_slist_create(const c_type_info_t* type_info);
c_slist_t* c_slist_create_from(const c_type_info_t* type_info, c_ref_t values, size_t length);
c_slist_t* c_slist_copy(c_slist_t* other);
c_slist_t* c_slist_assign(c_slist_t* self, c_slist_t* other);
void c_slist_destroy(c_slist_t* list);

/**
 * element access
 */
c_ref_t c_slist_front(c_slist_t* list);

/**
 * iterators
 */
c_slist_iterator_t c_slist_before_begin(c_slist_t* list);
c_slist_iterator_t c_slist_begin(c_slist_t* list);
c_slist_iterator_t c_slist_end(c_slist_t* list);

/**
 * capacity
 */
bool c_slist_empty(c_slist_t* list);
size_t c_slist_max_size(void);

/**
 * modifiers
 */
void c_slist_clear(c_slist_t* list);
c_slist_iterator_t c_slist_insert_after(c_slist_t* list, c_slist_iterator_t pos, c_ref_t value);
c_slist_iterator_t c_slist_erase_after(c_slist_t* list, c_slist_iterator_t pos);
c_slist_iterator_t c_slist_erase_after_range(c_slist_t* list, c_slist_iterator_t first, c_slist_iterator_t last);
void c_slist_push_front(c_slist_t* list, c_ref_t value);
void c_slist_pop_front(c_slist_t* list);
void c_slist_resize(c_slist_t* list, size_t count);
void c_slist_resize_with_value(c_slist_t* list, size_t count, c_ref_t value);
void c_slist_swap(c_slist_t* list, c_slist_t* other);

/**
 * operations
 */
void c_slist_merge(c_slist_t* list, c_slist_t* other);
void c_slist_merge_by(c_slist_t* list, c_slist_t* other, c_compare comp);
void c_slist_splice_after(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other);
void c_slist_splice_after_from(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t from);
void c_slist_splice_after_range(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t first, c_slist_iterator_t last);
void c_slist_remove(c_slist_t* list, c_ref_t value);
void c_slist_remove_if(c_slist_t* list, c_unary_predicate pred);
void c_slist_sort(c_slist_t* list);
void c_slist_sort_by(c_slist_t* list, c_compare comp);
void c_slist_reverse(c_slist_t* list);
void c_slist_unique(c_slist_t* list);
void c_slist_unique_if(c_slist_t* list, c_binary_predicate pred);

/**
 * helpers
 */
#define C_SLIST_INT     c_slist_create(c_get_int_type_info())
#define C_SLIST_SINT    c_slist_create(c_get_sint_type_info())
#define C_SLIST_UINT    c_slist_create(c_get_uint_type_info())
#define C_SLIST_SHORT   c_slist_create(c_get_short_type_info())
#define C_SLIST_SSHORT  c_slist_create(c_get_sshort_type_info())
#define C_SLIST_USHORT  c_slist_create(c_get_ushort_type_info())
#define C_SLIST_LONG    c_slist_create(c_get_long_type_info())
#define C_SLIST_SLONG   c_slist_create(c_get_slong_type_info())
#define C_SLIST_ULONG   c_slist_create(c_get_ulong_type_info())
#define C_SLIST_CHAR    c_slist_create(c_get_char_type_info())
#define C_SLIST_SCHAR   c_slist_create(c_get_schar_type_info())
#define C_SLIST_UCHAR   c_slist_create(c_get_uchar_type_info())
#define C_SLIST_FLOAT   c_slist_create(c_get_float_type_info())
#define C_SLIST_DOUBLE  c_slist_create(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_FORWARD_LIST_H__
