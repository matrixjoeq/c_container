/**
 * MIT License
 *
 * Copyright (c) 2017-2018 MatrixJoeQ
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

#ifndef __C_PRIME_INTERNAL_H__
#define __C_PRIME_INTERNAL_H__

#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"

#define __C_TYPE_SIZE(__type, __abbr) \
__c_static __c_inline size_t __c_##__abbr##_size(void) \
{ \
    return sizeof(__type); \
}

#define __C_TYPE_ALLOCATE(__type, __abbr) \
__c_static __c_inline c_ref_t __c_##__abbr##_allocate(void) \
{ \
    return (c_ref_t)malloc(__c_##__abbr##_size()); \
}

#define __C_TYPE_CREATE(__type, __abbr, __value) \
__c_static __c_inline void __c_##__abbr##_create(c_ref_t obj) \
{ \
    __c_assert(obj, "Object must be allocated before create."); \
    *((__type*)obj) = __value; \
}

#define __C_TYPE_COPY(__type, __abbr) \
__c_static __c_inline void __c_##__abbr##_copy(c_ref_t dst, const c_ref_t src) \
{ \
    __c_assert(dst, "Target object must be allocated before copy."); \
    __c_assert(dst, "Source object must be created before copy."); \
    *((__type*)dst) = *((__type*)src); \
}

#define __C_TYPE_DESTROY(__abbr) \
__c_static __c_inline void __c_##__abbr##_destroy(c_ref_t obj) \
{ \
    __c_assert(obj, "Object must be created before destory."); \
    __c_unuse(obj); \
}

#define __C_TYPE_DEALLOCATE(__abbr) \
__c_static __c_inline void __c_##__abbr##_deallocate(c_ref_t obj) \
{ \
    __c_assert(obj, "Object must be allocated before deallocate."); \
    free(obj); \
}

#define __C_TYPE_ASSIGN(__type, __abbr) \
__c_static __c_inline c_ref_t __c_##__abbr##_assign(c_ref_t dst, const c_ref_t src) \
{ \
    __c_assert(dst, "Target object must be allocated before assign."); \
    __c_assert(dst, "Source object must be created before assign."); \
    if (dst != src) { \
        *((__type*)dst) = *((__type*)src); \
    } \
    return dst; \
}

#define __C_TYPE_LESS(__type, __abbr) \
__c_static __c_inline bool __c_##__abbr##_less(const c_ref_t lhs, const c_ref_t rhs) \
{ \
    __c_assert(lhs, "Left hand object must be created before less compare."); \
    __c_assert(rhs, "Right hand object must be created before less compare."); \
    return (*((__type*)lhs) < *((__type*)rhs)); \
}

#define __C_TYPE_EQUAL(__type, __abbr) \
__c_static __c_inline bool __c_##__abbr##_equal(const c_ref_t lhs, const c_ref_t rhs) \
{ \
    __c_assert(lhs, "Left hand object must be created before equal compare."); \
    __c_assert(rhs, "Right hand object must be created before equal compare."); \
    return (*((__type*)lhs) == *((__type*)rhs)); \
}

#define __C_TYPE_OPERATIONS(__type, __abbr, __value) \
__C_TYPE_SIZE(__type, __abbr) \
__C_TYPE_ALLOCATE(__type, __abbr) \
__C_TYPE_CREATE(__type, __abbr, (__value)) \
__C_TYPE_COPY(__type, __abbr) \
__C_TYPE_DESTROY(__abbr) \
__C_TYPE_DEALLOCATE(__abbr) \
__C_TYPE_ASSIGN(__type, __abbr) \
__C_TYPE_LESS(__type, __abbr) \
__C_TYPE_EQUAL(__type, __abbr)

#define __C_GET_TYPE_INFO(__abbr) \
c_type_info_t* c_get_##__abbr##_type_info(void) \
{ \
    static c_type_info_t type_info = { \
        .size = __c_##__abbr##_size, \
        .allocate = __c_##__abbr##_allocate, \
        .create = __c_##__abbr##_create, \
        .copy = __c_##__abbr##_copy, \
        .destroy = __c_##__abbr##_destroy, \
        .deallocate = __c_##__abbr##_deallocate, \
        .assign = __c_##__abbr##_assign, \
        .less = __c_##__abbr##_less, \
        .equal = __c_##__abbr##_equal \
    }; \
    return &type_info; \
}

#endif  // __C_PRIME_INTERNAL_H__
