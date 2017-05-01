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

#ifndef __C_DEQUE_H__
#define __C_DEQUE_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_deque;
struct __c_backend_deque;

typedef struct __c_deque c_deque_t;
typedef struct __c_backend_deque c_backend_deque_t;

typedef struct __c_deque_iterator {
    c_iterator_t base_iter;
    c_ref_t pos;
} c_deque_iterator_t;

/**
 * constructor/destructor
 */
c_deque_t* c_deque_create(c_containable_t* type_info);
c_deque_t* c_deque_create_from(c_containable_t* type_info, c_ref_t values, size_t length);
c_deque_t* c_deque_copy(c_deque_t* other);
c_deque_t* c_deque_assign(c_deque_t* self, c_deque_t* other);
void c_deque_destroy(c_deque_t* deque);

/**
 * element access
 */
c_ref_t c_deque_at(c_deque_t* deque, size_t pos);
c_ref_t c_deque_front(c_deque_t* deque);
c_ref_t c_deque_back(c_deque_t* deque);

/**
 * iterators
 */
c_deque_iterator_t c_deque_begin(c_deque_t* deque);
c_deque_iterator_t c_deque_rbegin(c_deque_t* deque);
c_deque_iterator_t c_deque_end(c_deque_t* deque);
c_deque_iterator_t c_deque_rend(c_deque_t* deque);

/**
 * capacity
 */
bool c_deque_empty(c_deque_t* deque);
size_t c_deque_size(c_deque_t* deque);
size_t c_deque_max_size(void);
void c_deque_shrink_to_fit(c_deque_t* deque);

/**
 * modifiers
 */
void c_deque_clear(c_deque_t* deque);
c_deque_iterator_t c_deque_insert(c_deque_t* deque, c_deque_iterator_t pos, c_ref_t value);
c_deque_iterator_t c_deque_insert_n(c_deque_t* deque, c_deque_iterator_t pos, size_t count, c_ref_t value);
c_deque_iterator_t c_deque_insert_range(c_deque_t* deque, c_deque_iterator_t pos, c_iterator_t first, c_iterator_t last);
c_deque_iterator_t c_deque_erase(c_deque_t* deque, c_deque_iterator_t pos);
c_deque_iterator_t c_deque_erase_range(c_deque_t* deque, c_deque_iterator_t first, c_deque_iterator_t last);
void c_deque_push_back(c_deque_t* deque, c_ref_t value);
void c_deque_pop_back(c_deque_t* deque);
void c_deque_push_front(c_deque_t* deque, c_ref_t value);
void c_deque_pop_front(c_deque_t* deque);
void c_deque_resize(c_deque_t* deque, size_t count);
void c_deque_resize_with_value(c_deque_t* deque, size_t count, c_ref_t value);
void c_deque_swap(c_deque_t* deque, c_deque_t* other);

/**
 * backend
 */
c_backend_container_t* c_deque_create_backend(c_containable_t* type_info);

/**
 * helpers
 */
#define C_DEQUE_INT     c_deque_create(c_get_int_type_info())
#define C_DEQUE_SINT    c_deque_create(c_get_sint_type_info())
#define C_DEQUE_UINT    c_deque_create(c_get_uint_type_info())
#define C_DEQUE_SHORT   c_deque_create(c_get_short_type_info())
#define C_DEQUE_SSHORT  c_deque_create(c_get_sshort_type_info())
#define C_DEQUE_USHORT  c_deque_create(c_get_ushort_type_info())
#define C_DEQUE_LONG    c_deque_create(c_get_long_type_info())
#define C_DEQUE_SLONG   c_deque_create(c_get_slong_type_info())
#define C_DEQUE_ULONG   c_deque_create(c_get_ulong_type_info())
#define C_DEQUE_CHAR    c_deque_create(c_get_char_type_info())
#define C_DEQUE_SCHAR   c_deque_create(c_get_schar_type_info())
#define C_DEQUE_UCHAR   c_deque_create(c_get_uchar_type_info())
#define C_DEQUE_FLOAT   c_deque_create(c_get_float_type_info())
#define C_DEQUE_DOUBLE  c_deque_create(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_DEQUE_H__
