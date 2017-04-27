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

__c_static __c_inline size_t c_unsigned_char_size(void)
{
    return sizeof(unsigned char);
}

__c_static __c_inline void c_unsigned_char_create(c_ref_t obj)
{
    *((unsigned char*)obj) = 0u;
}

__c_static __c_inline void c_unsigned_char_copy(c_ref_t dst, const c_ref_t src)
{
    *((unsigned char*)dst) = *((unsigned char*)src);
}

__c_static __c_inline void c_unsigned_char_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_unsigned_char_assign(c_ref_t dst, const c_ref_t src)
{
    *((unsigned char*)dst) = *((unsigned char*)src);
    return dst;
}

__c_static __c_inline bool c_unsigned_char_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(unsigned char*)lhs) < *((unsigned char*)rhs);
}

__c_static __c_inline bool c_unsigned_char_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(unsigned char*)lhs) == (*(unsigned char*)rhs);
}

c_containable_t* c_get_uchar_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_unsigned_char_size,
        .create = c_unsigned_char_create,
        .copy = c_unsigned_char_copy,
        .destroy = c_unsigned_char_destroy,
        .assign = c_unsigned_char_assign,
        .less = c_unsigned_char_less,
        .equal = c_unsigned_char_equal
    };

    return &type_info;
}
