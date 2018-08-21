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

#ifndef __C_INTERNAL_H__
#define __C_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include "c_def.h"

#define __c_static static

#ifdef __GNUC__
#define __c_inline __attribute__((always_inline)) inline
#else
#define __c_inline inline
#endif

__c_inline uint64_t __c_get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    uint64_t ret = tv.tv_usec;
    /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
    ret /= 1000;

    /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
    ret += (tv.tv_sec * 1000);

    return ret;
}

#define C_MEASURE

#ifndef C_MEASURE
#define __c_measure(expr) expr
#else
#define __c_measure(expr) \
    do { \
        uint64_t __start = __c_get_time_ms(); \
        expr; \
        uint64_t __finish = __c_get_time_ms(); \
        printf("%s takes %llu ms to finish\n", #expr, __finish - __start); \
    } while (0)
#endif

__c_inline void __c_iter_copy_or_assign(c_iterator_t** dst, c_iterator_t* src)
{
    if (dst) {
        if (*dst == 0)
            C_ITER_COPY(dst, src);
        else
            C_ITER_ASSIGN(*dst, src);
    }
}

__c_inline c_iterator_t* __c_iter_copy_and_move(c_iterator_t** dst, c_iterator_t* src, ptrdiff_t n)
{
    __c_iter_copy_or_assign(dst, src);
    C_ITER_ADVANCE(*dst, n);
    return *dst;
}

__c_inline c_ref_t __c_identity(c_ref_t value)
{
    return value;
}

__c_inline c_ref_t __c_select1st(c_pair_t* pair)
{
    return pair->first;
}

__c_inline c_ref_t __c_select2nd(c_pair_t* pair)
{
    return pair->second;
}

// general macro
#define __c_free(x) \
    do { \
        if ((x)) { \
            free((x)); \
            (x) = 0; \
        } \
    } while (0)

#define __c_unuse(x) (void)(x)

#ifdef NDEBUG
#define __c_assert(cond, msg) { (void)(cond); (void)(msg); }
#else
#define __c_assert(cond, msg) assert((cond) && (msg))
#endif // NDEBUG
#define __c_static_assert(cond) { char static_assertion[(cond) ? 1 : -1]; (void)static_assertion; }
#define __array_length(__array) sizeof(__array) / sizeof(__array[0])
#define __array_foreach(__array, __index) for (unsigned int __index = 0; __index < __array_length(__array); ++__index)

// algorithm macro
#define __c_iter_get_shadow(x) \
    c_iterator_t* __##x = 0; \
    C_ITER_COPY(&__##x, x); \

#define __c_iter_put_shadow(x) \
    __c_free(__##x); \

#define __C_ALGO_BEGIN_1(x) \
    __c_iter_get_shadow(x)

#define __C_ALGO_END_1(x) \
    __c_iter_put_shadow(x)

#define __C_ALGO_BEGIN_2(x, y) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \

#define __C_ALGO_END_2(x, y) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \

#define __C_ALGO_BEGIN_3(x, y, z) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \

#define __C_ALGO_END_3(x, y, z) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \

#define __C_ALGO_BEGIN_4(x, y, z, u) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \

#define __C_ALGO_END_4(x, y, z, u) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \

#define __C_ALGO_BEGIN_5(x, y, z, u, v) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \
    __c_iter_get_shadow(v) \

#define __C_ALGO_END_5(x, y, z, u, v) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \
    __c_iter_put_shadow(v) \

#define __C_ALGO_BEGIN_6(x, y, z, u, v, w) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \
    __c_iter_get_shadow(v) \
    __c_iter_get_shadow(w) \

#define __C_ALGO_END_6(x, y, z, u, v, w) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \
    __c_iter_put_shadow(v) \
    __c_iter_put_shadow(w) \

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_INTERNAL_H__
