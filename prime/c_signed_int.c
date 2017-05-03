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

__c_static __c_inline size_t c_signed_int_size(void)
{
    return sizeof(signed int);
}

__c_static __c_inline void c_signed_int_create(c_ref_t obj)
{
    *((signed int*)obj) = 0;
}

__c_static __c_inline void c_signed_int_copy(c_ref_t dst, const c_ref_t src)
{
    *((signed int*)dst) = *((signed int*)src);
}

__c_static __c_inline void c_signed_int_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_signed_int_assign(c_ref_t dst, const c_ref_t src)
{
    *((signed int*)dst) = *((signed int*)src);
    return dst;
}

__c_static __c_inline bool c_signed_int_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(signed int*)lhs) < *((signed int*)rhs);
}

__c_static __c_inline bool c_signed_int_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(signed int*)lhs) == (*(signed int*)rhs);
}

c_type_info_t* c_get_sint_type_info(void)
{
    static c_type_info_t type_info = {
        .size = c_signed_int_size,
        .create = c_signed_int_create,
        .copy = c_signed_int_copy,
        .destroy = c_signed_int_destroy,
        .assign = c_signed_int_assign,
        .less = c_signed_int_less,
        .equal = c_signed_int_equal
    };

    return &type_info;
}
