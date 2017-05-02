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

size_t algo_merge_by(c_iterator_t* __c_forward_iterator first1,
                     c_iterator_t* __c_forward_iterator last1,
                     c_iterator_t* __c_forward_iterator first2,
                     c_iterator_t* __c_forward_iterator last2,
                     c_iterator_t* __c_forward_iterator d_first,
                     c_iterator_t** __c_forward_iterator d_last,
                     c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !d_first || !d_last || !comp)
        return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t n = 0;
    __C_ALGO_BEGIN_5(first1, last1, first2, last2, d_first)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__first1);
        }
        else {
            C_ITER_DEREF_ASSIGN(__d_first, __first2);
            C_ITER_INC(__first2);
        }
        ++n;
        C_ITER_INC(__d_first);
    }

    n += c_algo_copy(__first1, __last1, __d_first, &__d_first);
    n += c_algo_copy(__first2, __last2, __d_first, &__d_first);

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_5(first1, last1, first2, last2, d_first)

    return n;
}

bool algo_includes_by(c_iterator_t* __c_forward_iterator first1,
                      c_iterator_t* __c_forward_iterator last1,
                      c_iterator_t* __c_forward_iterator first2,
                      c_iterator_t* __c_forward_iterator last2,
                      c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !comp) return false;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));

    if (C_ITER_EQ(first2, last2)) return true;

    bool is_included = true;
    __C_ALGO_BEGIN_4(first1, last1, first2, last2)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first2), C_ITER_DEREF(__first1)))
            break;
        else if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2)))
            C_ITER_INC(__first1);
        else {
            C_ITER_INC(__first1);
            C_ITER_INC(__first2);
        }
    }

    if (C_ITER_NE(__first2, __last2)) is_included = false;

    __C_ALGO_END_4(first1, last1, first2, last2)

    return is_included;
}

size_t algo_set_difference_by(c_iterator_t* __c_forward_iterator first1,
                              c_iterator_t* __c_forward_iterator last1,
                              c_iterator_t* __c_forward_iterator first2,
                              c_iterator_t* __c_forward_iterator last2,
                              c_iterator_t* __c_forward_iterator d_first,
                              c_iterator_t** __c_forward_iterator d_last,
                              c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !d_first || !d_last || !comp)
        return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t n = 0;

    __C_ALGO_BEGIN_5(first1, last1, first2, last2, d_first)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__first1);
            C_ITER_INC(__d_first);
            ++n;
        }
        else {
            C_ITER_INC(__first1);
            C_ITER_INC(__first2);
        }
    }

    n += c_algo_copy(__first1, __last1, __d_first, &__d_first);

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_5(first1, last1, first2, last2, d_first)

    return n;
}

size_t algo_set_intersection_by(c_iterator_t* __c_forward_iterator first1,
                                c_iterator_t* __c_forward_iterator last1,
                                c_iterator_t* __c_forward_iterator first2,
                                c_iterator_t* __c_forward_iterator last2,
                                c_iterator_t* __c_forward_iterator d_first,
                                c_iterator_t** __c_forward_iterator d_last,
                                c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !d_first || !d_last || !comp)
        return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t n = 0;

    __C_ALGO_BEGIN_5(first1, last1, first2, last2, d_first)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2)))
            C_ITER_INC(__first1);
        else if (comp(C_ITER_DEREF(__first2), C_ITER_DEREF(__first1)))
            C_ITER_INC(__first2);
        else {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first1);
            C_ITER_INC(__first2);
            ++n;
        }
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_5(first1, last1, first2, last2, d_first)

    return n;
}

size_t algo_set_symmetric_difference_by(c_iterator_t* __c_forward_iterator first1,
                                        c_iterator_t* __c_forward_iterator last1,
                                        c_iterator_t* __c_forward_iterator first2,
                                        c_iterator_t* __c_forward_iterator last2,
                                        c_iterator_t* __c_forward_iterator d_first,
                                        c_iterator_t** __c_forward_iterator d_last,
                                        c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !d_first || !d_last || !comp)
        return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t n = 0;

    __C_ALGO_BEGIN_5(first1, last1, first2, last2, d_first)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first1);
            ++n;
        }
        else if (comp(C_ITER_DEREF(__first2), C_ITER_DEREF(__first1))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first2);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first2);
            ++n;
        }
        else {
            C_ITER_INC(__first1);
            C_ITER_INC(__first2);
        }
    }

    n += c_algo_copy(__first1, __last1, __d_first, &__d_first);
    n += c_algo_copy(__first2, __last2, __d_first, &__d_first);

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_5(first1, last1, first2, last2, d_first)

    return n;
}

size_t algo_set_union_by(c_iterator_t* __c_forward_iterator first1,
                         c_iterator_t* __c_forward_iterator last1,
                         c_iterator_t* __c_forward_iterator first2,
                         c_iterator_t* __c_forward_iterator last2,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_iterator_t** __c_forward_iterator d_last,
                         c_compare comp)
{
    if (!first1 || !last1 || !first2 || !last2 || !d_first || !d_last || !comp)
        return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last2, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t n = 0;

    __C_ALGO_BEGIN_5(first1, last1, first2, last2, d_first)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first1);
            ++n;
        }
        else if (comp(C_ITER_DEREF(__first2), C_ITER_DEREF(__first1))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first2);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first2);
            ++n;
        }
        else {
            C_ITER_DEREF_ASSIGN(__d_first, __first1);
            C_ITER_INC(__d_first);
            C_ITER_INC(__first1);
            C_ITER_INC(__first2);
            ++n;
        }
    }

    n += c_algo_copy(__first1, __last1, __d_first, &__d_first);
    n += c_algo_copy(__first2, __last2, __d_first, &__d_first);

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_5(first1, last1, first2, last2, d_first)

    return n;
}
