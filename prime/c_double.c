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

#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_double_size(void)
{
    return sizeof(double);
}

__c_static __c_inline void c_double_create(c_ref_t obj)
{
    *((double*)obj) = 0.0;
}

__c_static __c_inline void c_double_copy(c_ref_t dst, const c_ref_t src)
{
    *((double*)dst) = *((double*)src);
}

__c_static __c_inline void c_double_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_double_assign(c_ref_t dst, const c_ref_t src)
{
    *((double*)dst) = *((double*)src);
    return dst;
}

__c_static __c_inline bool c_double_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(double*)lhs) < *((double*)rhs);
}

__c_static __c_inline bool c_double_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(double*)lhs) == (*(double*)rhs);
}

c_containable_t* c_get_double_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_double_size,
        .create = c_double_create,
        .copy = c_double_copy,
        .destroy = c_double_destroy,
        .assign = c_double_assign,
        .less = c_double_less,
        .equal = c_double_equal
    };

    return &type_info;
}
