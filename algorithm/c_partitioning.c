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


bool algo_is_partitioned(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    if (C_ITER_EQ(first, last)) return true;

    bool is_partitioned = true;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            is_partitioned = false;
            break;
        }
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return is_partitioned;
}

void algo_partition(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t** __c_forward_iterator second_first,
                    c_unary_predicate pred)
{
    if (!first || !last || !second_first || !pred) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    __C_ALGO_BEGIN_2(first, last)

    algo_find_if_not(__first, __last, &__first, pred);

    if (C_ITER_NE(__first, __last)) {
        c_iterator_t* __next = 0;
        C_ITER_COPY(&__next, __first);
        C_ITER_INC(__next);
        while (C_ITER_NE(__next, __last)) {
            if (pred(C_ITER_DEREF(__next))) {
                algo_iter_swap(__first, __next);
                C_ITER_INC(__first);
            }
            C_ITER_INC(__next);
        }
        __c_free(__next);
    }

    if (*second_first == 0)
        C_ITER_COPY(second_first, __first);
    else {
        assert(C_ITER_AT_LEAST(*second_first, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*second_first, __first);
    }

    __C_ALGO_END_2(first, last)
}

size_t algo_partition_copy(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first_true,
                           c_iterator_t* __c_forward_iterator d_first_false,
                           c_iterator_t** __c_forward_iterator d_last_true,
                           c_iterator_t** __c_forward_iterator d_last_false,
                           c_unary_predicate pred)
{
    if (!first || !last || !d_first_true || !d_first_false ||
        !d_last_true || !d_last_false || !pred)
        return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first_true, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first_false, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first_true));
    assert(C_ITER_MODIFIABLE(d_first_false));

    size_t copied = 0;

    __C_ALGO_BEGIN_4(first, last, d_first_true, d_first_false)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN(__d_first_true, __first);
            C_ITER_INC(__d_first_true);
        }
        else {
            C_ITER_DEREF_ASSIGN(__d_first_false, __first);
            C_ITER_INC(__d_first_false);
        }
        C_ITER_INC(__first);
        ++copied;
    }

    if (*d_last_true == 0)
        C_ITER_COPY(d_last_true, __d_first_true);
    else {
        assert(C_ITER_AT_LEAST(*d_last_true, C_ITER_CATE_FORWARD));
        C_ITER_COPY(*d_last_true, __d_first_true);
    }

    if (*d_last_false == 0)
        C_ITER_COPY(d_last_false, __d_first_false);
    else {
        assert(C_ITER_AT_LEAST(*d_last_false, C_ITER_CATE_FORWARD));
        C_ITER_COPY(*d_last_false, __d_first_false);
    }

    __C_ALGO_END_4(first, last, d_first_true, d_first_false)

    return copied;
}

void algo_partition_point(c_iterator_t* __c_forward_iterator first,
                          c_iterator_t* __c_forward_iterator last,
                          c_iterator_t** __c_forward_iterator first_last,
                          c_unary_predicate pred)
{
    if (!first || !last || !first_last || !pred) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);

    if (*first_last == 0)
        C_ITER_COPY(first_last, __first);
    else {
        assert(C_ITER_AT_LEAST(*first_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*first_last, __first);
    }

    __C_ALGO_END_2(first, last)
}
