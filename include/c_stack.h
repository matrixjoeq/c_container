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

#ifndef __C_STACK_H__
#define __C_STACK_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"
#include "c_deque.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_stack;
typedef struct __c_stack c_stack_t;

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(c_containable_t* type_info, BackendContainerCreator creator);
void c_stack_destroy(c_stack_t* stack);

/**
 * element access
 */
c_ref_t c_stack_top(c_stack_t* stack);

/**
 * capacity
 */
bool c_stack_empty(c_stack_t* stack);
size_t c_stack_max_size(c_stack_t* stack);

/**
 * modifiers
 */
void c_stack_push(c_stack_t* stack, c_ref_t value);
void c_stack_pop(c_stack_t* stack);
void c_stack_swap(c_stack_t* stack, c_stack_t* other);

/**
 * helpers
 */
#define C_STACK_BASE(t, b)      c_stack_create((t), (b))
#define C_STACK(t)              C_STACK_BASE((t), c_deque_create_backend)

#define C_STACK_INT     C_STACK(c_get_int_type_info())
#define C_STACK_SINT    C_STACK(c_get_sint_type_info())
#define C_STACK_UINT    C_STACK(c_get_uint_type_info())
#define C_STACK_SHORT   C_STACK(c_get_short_type_info())
#define C_STACK_SSHORT  C_STACK(c_get_sshort_type_info())
#define C_STACK_USHORT  C_STACK(c_get_ushort_type_info())
#define C_STACK_LONG    C_STACK(c_get_long_type_info())
#define C_STACK_SLONG   C_STACK(c_get_slong_type_info())
#define C_STACK_ULONG   C_STACK(c_get_ulong_type_info())
#define C_STACK_CHAR    C_STACK(c_get_char_type_info())
#define C_STACK_SCHAR   C_STACK(c_get_schar_type_info())
#define C_STACK_UCHAR   C_STACK(c_get_uchar_type_info())
#define C_STACK_FLOAT   C_STACK(c_get_float_type_info())
#define C_STACK_DOUBLE  C_STACK(c_get_double_type_info())

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_STACK_H__
