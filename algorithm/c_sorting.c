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

    if (C_ITER_EQ(first, last)) {
        if (*until == 0)
            C_ITER_COPY(until, last);
        else {
            assert(C_ITER_AT_LEAST(*until, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*until, last);
        }
        return;
    }

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __next = 0;
    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);

    bool is_sorted = true;
    while (C_ITER_NE(__next, __last)) {
        if (!comp(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
            is_sorted = false;
            break;
        }
        C_ITER_INC(__first);
        C_ITER_INC(__next);
    }

    if (*until == 0)
        C_ITER_COPY(until, is_sorted ? __last : __first);
    else {
        assert(C_ITER_AT_LEAST(*until, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*until, is_sorted ? __last : __first);
    }

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
    assert(C_ITER_MODIFIABLE(first));
    assert(C_ITER_MODIFIABLE(middle));

    if (C_ITER_EQ(first, middle)) return;

    __C_ALGO_BEGIN_3(first, middle, last)

    c_iterator_t* __middle_prev = 0;
    c_iterator_t* __i = 0;

    C_ITER_COPY(&__middle_prev, __middle);
    C_ITER_COPY(&__i, __middle);

    C_ITER_DEC(__middle_prev);

    c_algo_make_heap_by(__first, __middle, comp);
    while (C_ITER_NE(__i, __last)) {
        if (comp(C_ITER_DEREF(__i), C_ITER_DEREF(__first))) {
            c_algo_pop_heap_by(__first, __middle, comp);
            c_algo_iter_swap(__middle_prev, __i);
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
    if (!first || !last || !d_first || !d_last || !d_upper || !comp) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_EXACT(d_first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(d_last, C_ITER_CATE_RANDOM));
    assert(C_ITER_MODIFIABLE(d_first));

    if (C_ITER_EQ(first, last) || C_ITER_EQ(d_first, d_last)) {
        if (*d_upper == 0)
            C_ITER_COPY(d_upper, d_first);
        else {
            assert(C_ITER_EXACT(*d_upper, C_ITER_CATE_RANDOM));
            C_ITER_ASSIGN(*d_upper, d_first);
        }

        return 0;
    }

    size_t n = 0;

    __C_ALGO_BEGIN_4(first, last, d_first, d_last)

    c_iterator_t* __i = 0;
    c_iterator_t* __d = 0;
    C_ITER_COPY(&__i, __first);
    C_ITER_COPY(&__d, __d_first);

    while (C_ITER_NE(__i, __last) && C_ITER_NE(__d, __d_last)) {
        C_ITER_DEREF_ASSIGN(__d, __i);
        C_ITER_INC(__d);
        C_ITER_INC(__i);
        ++n;
    }

    c_iterator_t* __d_prev = 0;
    C_ITER_COPY(&__d_prev, __d);
    C_ITER_DEC(__d_prev);

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

    if (*d_upper == 0)
        C_ITER_COPY(d_upper, __d);
    else {
        assert(C_ITER_EXACT(*d_upper, C_ITER_CATE_RANDOM));
        C_ITER_ASSIGN(*d_upper, __d);
    }

    __c_free(__d_prev);
    __c_free(__d);
    __c_free(__i);

    __C_ALGO_END_4(first, last, d_first, d_last)

    return n;
}
