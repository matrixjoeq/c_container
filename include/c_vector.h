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

#ifndef __C_VECTOR_H__
#define __C_VECTOR_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_vector;
struct __c_backend_vector;

typedef struct __c_vector c_vector_t;
typedef struct __c_backend_vector c_backend_vector_t;

typedef struct __c_vector_iterator {
    c_iterator_t base_iter;
    c_ref_t pos;
} c_vector_iterator_t;

/**
 * constructor/destructor
 */
c_vector_t* c_vector_create(c_type_info_t* type_info);
c_vector_t* c_vector_create_from(c_type_info_t* type_info, c_ref_t values, size_t length);
c_vector_t* c_vector_copy(c_vector_t* other);
c_vector_t* c_vector_assign(c_vector_t* self, c_vector_t* other);
void c_vector_destroy(c_vector_t* vector);

/**
 * element access
 */
c_ref_t c_vector_at(c_vector_t* vector, size_t pos);
c_ref_t c_vector_front(c_vector_t* vector);
c_ref_t c_vector_back(c_vector_t* vector);
c_ref_t c_vector_data(c_vector_t* vector);

/**
 * iterators
 */
c_vector_iterator_t c_vector_begin(c_vector_t* vector);
c_vector_iterator_t c_vector_rbegin(c_vector_t* vector);
c_vector_iterator_t c_vector_end(c_vector_t* vector);
c_vector_iterator_t c_vector_rend(c_vector_t* vector);

/**
 * capacity
 */
bool c_vector_empty(c_vector_t* vector);
size_t c_vector_size(c_vector_t* vector);
size_t c_vector_max_size(void);
void c_vector_reserve(c_vector_t* vector, size_t new_cap);
size_t c_vector_capacity(c_vector_t* vector);
void c_vector_shrink_to_fit(c_vector_t* vector);

/**
 * modifiers
 */
void c_vector_clear(c_vector_t* vector);
c_vector_iterator_t c_vector_insert(c_vector_t* vector, c_vector_iterator_t pos, c_ref_t value);
c_vector_iterator_t c_vector_insert_n(c_vector_t* vector, c_vector_iterator_t pos, size_t count, c_ref_t value);
c_vector_iterator_t c_vector_insert_range(c_vector_t* vector, c_vector_iterator_t pos, c_iterator_t first, c_iterator_t last);
c_vector_iterator_t c_vector_erase(c_vector_t* vector, c_vector_iterator_t pos);
c_vector_iterator_t c_vector_erase_range(c_vector_t* vector, c_vector_iterator_t first, c_vector_iterator_t last);
void c_vector_push_back(c_vector_t* vector, c_ref_t value);
void c_vector_pop_back(c_vector_t* vector);
void c_vector_resize(c_vector_t* vector, size_t count);
void c_vector_resize_with_value(c_vector_t* vector, size_t count, c_ref_t value);
void c_vector_swap(c_vector_t* vector, c_vector_t* other);

/**
 * backend
 */
c_backend_container_t* c_vector_create_backend(c_type_info_t* type_info);

/**
 * helpers
 */
#define C_VECTOR_INT    c_vector_create(c_get_int_type_info())
#define C_VECTOR_SINT   c_vector_create(c_get_sint_type_info())
#define C_VECTOR_UINT   c_vector_create(c_get_uint_type_info())
#define C_VECTOR_SHORT  c_vector_create(c_get_short_type_info())
#define C_VECTOR_SSHORT c_vector_create(c_get_sshort_type_info())
#define C_VECTOR_USHORT c_vector_create(c_get_ushort_type_info())
#define C_VECTOR_LONG   c_vector_create(c_get_long_type_info())
#define C_VECTOR_SLONG  c_vector_create(c_get_slong_type_info())
#define C_VECTOR_ULONG  c_vector_create(c_get_ulong_type_info())
#define C_VECTOR_CHAR   c_vector_create(c_get_char_type_info())
#define C_VECTOR_SCHAR  c_vector_create(c_get_schar_type_info())
#define C_VECTOR_UCHAR  c_vector_create(c_get_uchar_type_info())
#define C_VECTOR_FLOAT  c_vector_create(c_get_float_type_info())
#define C_VECTOR_DOUBLE c_vector_create(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_VECTOR_H__
