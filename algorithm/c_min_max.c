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

c_ref_t algo_max_by(c_containable_t* value_type,
                    c_ref_t x,
                    c_ref_t y,
                    c_compare comp)
{
    if (!value_type || !x || !y || !comp) return 0;

    c_ref_t r = malloc(value_type->size());
    if (!r) return 0;

    value_type->copy(r, comp(x, y) ? y : x);

    return r;
}

c_ref_t algo_min_by(c_containable_t* value_type,
                    c_ref_t x,
                    c_ref_t y,
                    c_compare comp)
{
    if (!value_type || !x || !y || !comp) return 0;

    c_ref_t r = malloc(value_type->size());
    if (!r) return 0;

    value_type->copy(r, comp(x, y) ? x : y);

    return r;
}

void algo_max_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator max,
                         c_compare comp)
{
    if (!first || !last || !max || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __max = 0;
    C_ITER_COPY(&__max, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__max), C_ITER_DEREF(__first))) {
                C_ITER_ASSIGN(__max, __first);
            }
        }
    }

    if (*max == 0)
        C_ITER_COPY(max, __max);
    else {
        assert(C_ITER_AT_LEAST(*max, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*max, __max);
    }

    __c_free(__max);
    __C_ALGO_END_2(first, last)
}

void algo_min_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator min,
                         c_compare comp)
{
    if (!first || !last || !min || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __min = 0;
    C_ITER_COPY(&__min, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__first), C_ITER_DEREF(__min))) {
                C_ITER_ASSIGN(__min, __first);
            }
        }
    }

    if (*min == 0)
        C_ITER_COPY(min, __min);
    else {
        assert(C_ITER_AT_LEAST(*min, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*min, __min);
    }

    __c_free(__min);
    __C_ALGO_END_2(first, last)
}

void algo_minmax_element_by(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t** __c_forward_iterator min,
                            c_iterator_t** __c_forward_iterator max,
                            c_compare comp)
{
    if (!first || !last || !min || !max || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __min = 0;
    c_iterator_t* __max = 0;
    C_ITER_COPY(&__min, __first);
    C_ITER_COPY(&__max, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__first), C_ITER_DEREF(__min))) {
                C_ITER_ASSIGN(__min, __first);
            }

            if (comp(C_ITER_DEREF(__max), C_ITER_DEREF(__first))) {
                C_ITER_ASSIGN(__max, __first);
            }
        }
    }

    if (*min == 0)
        C_ITER_COPY(min, __min);
    else {
        assert(C_ITER_AT_LEAST(*min, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*min, __min);
    }

    if (*max == 0)
        C_ITER_COPY(max, __max);
    else {
        assert(C_ITER_AT_LEAST(*max, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*max, __max);
    }

    __c_free(__min);
    __c_free(__max);
    __C_ALGO_END_2(first, last)
}

c_ref_t algo_clamp_by(c_containable_t* value_type,
                      c_ref_t v,
                      c_ref_t lo,
                      c_ref_t hi,
                      c_compare comp)
{
    if (!value_type || !v || !lo || !hi || !comp) return 0;
    assert(!comp(hi, lo));

    return comp(v, hi) ? algo_max_by(value_type, v, lo, comp)
                       : algo_min_by(value_type, v, hi, comp);
}

bool algo_lexicographical_compare_by(c_iterator_t* __c_forward_iterator first1,
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

    bool ret = false;

    __C_ALGO_BEGIN_4(first1, last1, first2, last2)

    while (C_ITER_NE(__first1, __last1) && C_ITER_NE(__first2, __last2)) {
        if (comp(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
            ret = true;
            break;
        }
        if (comp(C_ITER_DEREF(__first2), C_ITER_DEREF(__first1))) {
            ret = false;
            break;
        }
        C_ITER_INC(__first1);
        C_ITER_INC(__first2);
    }

    if (C_ITER_EQ(__first1, __last1) && C_ITER_NE(__first2, __last2))
        ret = true;

    __C_ALGO_END_4(first1, last1, first2, last2)

    return ret;
}
