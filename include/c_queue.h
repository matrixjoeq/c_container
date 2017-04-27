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

#ifndef __C_QUEUE_H__
#define __C_QUEUE_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_queue;
typedef struct __c_queue c_queue_t;

/**
 * constructor/destructor
 */
c_queue_t* c_queue_create(c_containable_t* type_info, BackendContainerCreator creator);
void c_queue_destroy(c_queue_t* queue);

/**
 * element access
 */
c_ref_t c_queue_front(c_queue_t* queue);
c_ref_t c_queue_back(c_queue_t* queue);

/**
 * capacity
 */
bool c_queue_empty(c_queue_t* queue);
size_t c_queue_size(c_queue_t* queue);

/**
 * modifiers
 */
void c_queue_push(c_queue_t* queue, c_ref_t data);
void c_queue_pop(c_queue_t* queue);
void c_queue_swap(c_queue_t* queue, c_queue_t* other);

/**
 * helpers
 */
#define C_QUEUE_INT c_queue_create(c_get_int_type_info(), c_list_create_backend)
#define C_QUEUE_SINT c_queue_create(c_get_sint_type_info(), c_list_create_backend)
#define C_QUEUE_UINT c_queue_create(c_get_uint_type_info(), c_list_create_backend)
#define C_QUEUE_SHORT c_queue_create(c_get_short_type_info(), c_list_create_backend)
#define C_QUEUE_SSHORT c_queue_create(c_get_sshort_type_info(), c_list_create_backend)
#define C_QUEUE_USHORT c_queue_create(c_get_ushort_type_info(), c_list_create_backend)
#define C_QUEUE_LONG c_queue_create(c_get_long_type_info(), c_list_create_backend)
#define C_QUEUE_SLONG c_queue_create(c_get_slong_type_info(), c_list_create_backend)
#define C_QUEUE_ULONG c_queue_create(c_get_ulong_type_info(), c_list_create_backend)
#define C_QUEUE_CHAR c_queue_create(c_get_char_type_info(), c_list_create_backend)
#define C_QUEUE_SCHAR c_queue_create(c_get_schar_type_info(), c_list_create_backend)
#define C_QUEUE_UCHAR c_queue_create(c_get_uchar_type_info(), c_list_create_backend)
#define C_QUEUE_FLOAT c_queue_create(c_get_float_type_info(), c_list_create_backend)
#define C_QUEUE_DOUBLE c_queue_create(c_get_double_type_info(), c_list_create_backend)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_QUEUE_H__
