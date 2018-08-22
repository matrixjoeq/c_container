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

#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"
#if 0
__c_static __c_inline size_t __lg(size_t n)
{
    size_t k = 0;
    for (k = 0; n > 1; n >>= 1) ++k;
    return k;
}

__c_static __c_inline
c_ref_t __median_of_three_by(c_ref_t x, c_ref_t y, c_ref_t z, c_compare comp)
{
    if (comp(x, y)) {
        if (comp(y, z)) // x < y < z
            return y;
        else if (comp(z, x)) // z < x < y
            return x;
        else // x <= z <= y
            return z;
    }
    else {
        if (comp(x, z)) // y <= x < z
            return x;
        else if (comp(z, y)) // z < y <= x
            return y;
        else // y <= z <= x
            return z;
    }
}

__c_static __c_inline
void __partition_by(c_iterator_t* __c_random_iterator first,
                    c_iterator_t* __c_random_iterator last,
                    c_iterator_t** __c_random_iterator second_first,
                    c_ref_t pivot,
                    c_compare comp)
{
    __C_ALGO_BEGIN_2(first, last)

    while (true) {
        while (comp(C_ITER_DEREF(__first), pivot)) C_ITER_INC(__first);
        C_ITER_DEC(__last);
        while (comp(pivot, C_ITER_DEREF(__last))) C_ITER_DEC(__last);
        if (!C_ITER_LESS(__first, __last)) {
            __c_iter_copy_or_assign(second_first, __first);
            break;
        }
        algo_iter_swap(__first, __last);
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)
}

__c_static void __introspective_sort_by(c_iterator_t* __c_random_iterator first,
                                        c_iterator_t* __c_random_iterator last,
                                        size_t depth_limit,
                                        c_compare comp)
{
    static const int __s_threshold = 16;
    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __middle = 0;
    c_iterator_t* __last_prev = 0;
    c_iterator_t* __part = 0;

    while (C_ITER_DISTANCE(__first, __last) > __s_threshold) { // leave small set unsorted for insertion sort
        if (depth_limit == 0) { // partition is turning bad, prevent it from going to O(N^2)
            algo_partial_sort_by(__first, __last, __last, comp);
            break;
        }

        --depth_limit;
        __c_iter_copy_and_move(&__middle, __first, C_ITER_DISTANCE(__first, __last) / 2);
        __c_iter_copy_and_move(&__last_prev, __last, -1);

        // __pivot is a reference to the median value, DO NOT free it
        c_ref_t __pivot = __median_of_three_by(C_ITER_DEREF(__first),
                                               C_ITER_DEREF(__middle),
                                               C_ITER_DEREF(__last_prev),
                                               comp);

        __partition_by(__first, __last, &__part, __pivot, comp);
        __introspective_sort_by(__part, __last, depth_limit, comp); // introsort second half
        C_ITER_ASSIGN(__last, __part); // introsort first half
    }

    __c_free(__part);
    __c_free(__last_prev);
    __c_free(__middle);

    __C_ALGO_END_2(first, last)
}

__c_static __c_inline
void __insertion_sort_by(c_iterator_t* __c_random_iterator first,
                         c_iterator_t* __c_random_iterator last,
                         c_compare comp)
{
    if (C_ITER_EQ(first, last)) return;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __i = 0;
    c_iterator_t* __i_next = 0;
    c_iterator_t* __inner_last = 0;
    c_ref_t __last_value = malloc(__first->value_type->size());

    __c_iter_copy_and_move(&__i, __first, 1);
    C_ITER_COPY(&__i_next, __i);
    C_ITER_COPY(&__inner_last, __i);
    __first->value_type->create(__last_value);

    while (C_ITER_NE(__i, __last)) { // outer loop
        C_ITER_V_ASSIGN_DEREF(__last_value, __i);
        if (comp(__last_value, C_ITER_DEREF(__first))) {
            __c_iter_copy_and_move(&__i_next, __i, 1);
            algo_copy_backward(__first, __i, __i_next, C_IGNORED);
            C_ITER_DEREF_ASSIGN_V(__first, __last_value);
        }
        else {
            C_ITER_ASSIGN(__inner_last, __i);
            __c_iter_copy_and_move(&__i_next, __i, -1);
            while (comp(__last_value, C_ITER_DEREF(__i_next))) { // inner loop
                C_ITER_DEREF_ASSIGN(__inner_last, __i_next);
                C_ITER_ASSIGN(__inner_last, __i_next);
                C_ITER_DEC(__i_next);
            }
            C_ITER_DEREF_ASSIGN_V(__inner_last, __last_value);
        }
        C_ITER_INC(__i);
    }

    __c_free(__last_value);
    __c_free(__inner_last);
    __c_free(__i_next);
    __c_free(__i);

    __C_ALGO_END_2(first, last)
}
#endif
bool algo_is_sorted_by(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __until = 0;

    algo_is_sorted_until_by(__first, __last, &__until, comp);
    bool is_sorted = C_ITER_EQ(__until, __last);

    __c_free(__until);

    __C_ALGO_END_2(first, last)

    return is_sorted;
}

void algo_is_sorted_until_by(c_iterator_t* __c_forward_iterator first,
                             c_iterator_t* __c_forward_iterator last,
                             c_iterator_t** __c_forward_iterator until,
                             c_compare comp)
{
    if (!first || !last || !until || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(*until == 0 || C_ITER_AT_LEAST(*until, C_ITER_CATE_FORWARD));

    if (C_ITER_EQ(first, last)) {
        __c_iter_copy_or_assign(until, last);
        return;
    }

    __C_ALGO_BEGIN_2(first, last)

    bool __is_sorted = true;
    c_iterator_t* __next = 0;

    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);

    while (C_ITER_NE(__next, __last)) {
        if (comp(C_ITER_DEREF(__next), C_ITER_DEREF(__first))) {
            __is_sorted = false;
            break;
        }
        C_ITER_INC(__first);
        C_ITER_INC(__next);
    }

    __c_iter_copy_or_assign(until, __is_sorted ? __last : __first);

    __c_free(__next);

    __C_ALGO_END_2(first, last)
}

void algo_sort_by(c_iterator_t* __c_random_iterator first,
                  c_iterator_t* __c_random_iterator last,
                  c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
    assert(C_ITER_MUTABLE(first));

    if (C_ITER_EQ(first, last)) return;

    __C_ALGO_BEGIN_2(first, last)

    //__c_measure(__introspective_sort_by(__first, __last, __lg(C_ITER_DISTANCE(__first, __last)) * 2, comp));
    //__c_measure(__insertion_sort_by(__first, __last, comp));
    algo_partial_sort_by(__first, __last, __last, comp);

    __C_ALGO_END_2(first, last)
}

void algo_partial_sort_by(c_iterator_t* __c_random_iterator first,
                          c_iterator_t* __c_random_iterator middle,
                          c_iterator_t* __c_random_iterator last,
                          c_compare comp)
{
    if (!first || !middle || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(middle, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
    assert(C_ITER_MUTABLE(first));
    assert(C_ITER_MUTABLE(middle));

    if (C_ITER_EQ(first, middle)) return;

    __C_ALGO_BEGIN_3(first, middle, last)

    c_iterator_t* __middle_prev = 0;
    c_iterator_t* __i = 0;

    C_ITER_COPY(&__i, __middle);
    __c_iter_copy_and_move(&__middle_prev, __middle, -1);

    c_algo_make_heap_by(__first, __middle, comp);
    while (C_ITER_NE(__i, __last)) {
        if (comp(C_ITER_DEREF(__i), C_ITER_DEREF(__first))) {
            c_algo_pop_heap_by(__first, __middle, comp);
            c_algo_iter_swap(__middle_prev, __i);
            c_algo_push_heap_by(__first, __middle, comp);
        }
        C_ITER_INC(__i);
    }

    c_algo_sort_heap_by(__first, __middle, comp);

    __c_free(__i);
    __c_free(__middle_prev);

    __C_ALGO_END_3(first, middle, last)
}

size_t algo_partial_sort_copy_by(c_iterator_t* __c_forward_iterator first,
                                 c_iterator_t* __c_forward_iterator last,
                                 c_iterator_t* __c_random_iterator d_first,
                                 c_iterator_t* __c_random_iterator d_last,
                                 c_iterator_t** __c_random_iterator d_upper,
                                 c_compare comp)
{
    if (!first || !last || !d_first || !d_last || !comp) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_EXACT(d_first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(d_last, C_ITER_CATE_RANDOM));
    assert(d_upper == 0 || *d_upper == 0 || C_ITER_EXACT(*d_upper, C_ITER_CATE_RANDOM));
    assert(C_ITER_MUTABLE(d_first));

    if (C_ITER_EQ(first, last) || C_ITER_EQ(d_first, d_last)) {
        __c_iter_copy_or_assign(d_upper, d_first);
        return 0;
    }

    size_t n = 0;

    __C_ALGO_BEGIN_4(first, last, d_first, d_last)

    c_iterator_t* __i = 0;
    c_iterator_t* __d = 0;
    c_iterator_t* __d_prev = 0;

    C_ITER_COPY(&__i, __first);
    C_ITER_COPY(&__d, __d_first);

    while (C_ITER_NE(__i, __last) && C_ITER_NE(__d, __d_last)) {
        C_ITER_DEREF_ASSIGN(__d, __i);
        C_ITER_INC(__d);
        C_ITER_INC(__i);
        ++n;
    }

    __c_iter_copy_and_move(&__d_prev, __d, -1);

    c_algo_make_heap_by(__d_first, __d, comp);
    while (C_ITER_NE(__i, __last)) {
        if (comp(C_ITER_DEREF(__i), C_ITER_DEREF(__d_first))) {
            c_algo_pop_heap_by(__d_first, __d, comp);
            C_ITER_DEREF_ASSIGN(__d_prev, __i);
            c_algo_push_heap_by(__d_first, __d, comp);
        }
        C_ITER_INC(__i);
    }

    c_algo_sort_heap_by(__d_first, __d, comp);

    __c_iter_copy_or_assign(d_upper, __d);

    __c_free(__d_prev);
    __c_free(__d);
    __c_free(__i);

    __C_ALGO_END_4(first, last, d_first, d_last)

    return n;
}
