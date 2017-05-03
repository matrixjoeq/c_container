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
#include <string.h>
#include "c_internal.h"
#include "c_algorithm.h"

static c_random_int_t __uniform(c_random_int_t min, c_random_int_t max, long int seed)
{
    double t = 0;
    seed = 2045 * (seed) + 1;
    seed = seed - (seed / 1048576) * 1048576;
    t = (seed) / 1048576.0;
    t = min + (max - min) * t;
    return (c_random_int_t)t;
}

static c_random_int_t uniform(c_random_int_t min, c_random_int_t max)
{
    long int s = 0;
    c_random_int_t r = 0;

    s = rand();
    r = __uniform(min, max, s);

    return r;
}

static c_random_int_t algo_uniform(c_random_int_t n)
{
    return uniform(0, n);
}

size_t algo_copy(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator last,
                 c_iterator_t* __c_forward_iterator d_first,
                 c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEREF_ASSIGN(__d_first, __first);
        C_ITER_INC(__first);
        C_ITER_INC(__d_first);
        ++copied;
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

size_t algo_copy_if(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t* __c_forward_iterator d_first,
                    c_iterator_t** __c_forward_iterator d_last,
                    c_unary_predicate pred)
{
    if (!first || !last || !d_first || !d_last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++copied;
        }
        C_ITER_INC(__first);
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

size_t algo_copy_backward(c_iterator_t* __c_bidirection_iterator first,
                          c_iterator_t* __c_bidirection_iterator last,
                          c_iterator_t* __c_bidirection_iterator d_last,
                          c_iterator_t** __c_bidirection_iterator d_last_copied)
{
    if (!first || !last || !d_last || !d_last_copied) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(d_last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_MODIFIABLE(d_last));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_last);

    while (C_ITER_NE(__last, __first)) {
        C_ITER_DEC(__last);
        C_ITER_DEC(__d_last);
        C_ITER_DEREF_ASSIGN(__d_last, __last);
        ++copied;
    }

    if (*d_last_copied == 0)
        C_ITER_COPY(d_last_copied, __d_last);
    else {
        assert(C_ITER_AT_LEAST(*d_last_copied, C_ITER_CATE_BIDIRECTION));
        C_ITER_ASSIGN(*d_last_copied, __d_last);
    }

    __C_ALGO_END_3(first, last, d_last);

    return copied;
}

size_t algo_fill(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator last,
                 c_ref_t value)
{
    if (!first || !last || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t filled = 0;

    __C_ALGO_BEGIN_2(first, last);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEREF_ASSIGN_V(__first, value);
        C_ITER_INC(__first);
        ++filled;
    }

    __C_ALGO_END_2(first, last);

    return filled;
}

void algo_fill_n(c_iterator_t* __c_forward_iterator first,
                 size_t n,
                 c_ref_t value,
                 c_iterator_t** __c_forward_iterator last)
{
    if (!first || !value || !last) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    __C_ALGO_BEGIN_1(first);

    while (n--) {
        C_ITER_DEREF_ASSIGN_V(__first, value);
        C_ITER_INC(__first);
    }

    if (*last == 0)
        C_ITER_COPY(last, __first);
    else {
        assert(C_ITER_AT_LEAST(*last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*last, __first);
    }

    __C_ALGO_END_1(first);
}

size_t algo_transform(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t* __c_forward_iterator d_first,
                      c_unary_func op)
{
    if (!first || !last || !d_first || !op) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t transformed = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    c_ref_t __value = malloc(__first->value_type->size());
    assert(__value);
    __first->value_type->create(__value);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_V_ASSIGN_DEREF(__value, __first);
        op(__value);
        C_ITER_DEREF_ASSIGN_V(__d_first, __value);
        C_ITER_INC(__first);
        C_ITER_INC(__d_first);
        ++transformed;
    }

    __c_free(__value);

    __C_ALGO_END_3(first, last, d_first);

    return transformed;
}

size_t algo_generate(c_iterator_t* __c_forward_iterator first,
                     c_iterator_t* __c_forward_iterator last,
                     c_generator_emplace gen)
{
    if (!first || !last || !gen) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t gened = 0;

    size_t __size = first->value_type->size();
    c_ref_t __value = malloc(__size);
    if (!__value) goto out;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        memset(__value, 0, __size);
        gen(__value);
        C_ITER_DEREF_ASSIGN_V(__first, __value);
        C_ITER_INC(__first);
        ++gened;
    }

    __C_ALGO_END_2(first, last)

    __c_free(__value);
out:
    return gened;
}

void algo_generate_n(c_iterator_t* __c_forward_iterator first,
                     size_t n,
                     c_generator_emplace gen,
                     c_iterator_t** __c_forward_iterator last)
{
    if (!first || !gen || !last) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t gened = 0;

    __C_ALGO_BEGIN_1(first)

    if (n == 0) goto out;

    size_t __size = __first->value_type->size();
    c_ref_t __value = malloc(__size);
    if (!__value) goto out;

    while (n--) {
        memset(__value, 0, __size);
        gen(__value);
        C_ITER_DEREF_ASSIGN_V(__first, __value);
        C_ITER_INC(__first);
        ++gened;
    }

    __c_free(__value);

out:
    if (*last == 0)
        C_ITER_COPY(last, gened ? __first : first);
    else {
        assert(C_ITER_AT_LEAST(*last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*last, gened ? __first : first);
    }

    __C_ALGO_END_1(first)
}

size_t algo_remove(c_iterator_t* __c_forward_iterator first,
                   c_iterator_t* __c_forward_iterator last,
                   c_ref_t value,
                   c_iterator_t** __c_forward_iterator new_last)
{
    if (!first || !last || !value || !new_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t removed = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find(__first, __last, &__first, value);
    if (C_ITER_NE(__first, __last)) {
        ++removed;
        c_iterator_t* __i = 0;
        C_ITER_COPY(&__i, __first);
        C_ITER_INC(__i);
        while (C_ITER_NE(__i, __last)) {
            if (!(C_ITER_DEREF_EQUAL_V(__i, value))) {
                C_ITER_DEREF_ASSIGN(__first, __i);
                C_ITER_INC(__first);
            }
            else
                ++removed;

            C_ITER_INC(__i);
        }
        __c_free(__i);
    }

    if (*new_last == 0)
        C_ITER_COPY(new_last, __first);
    else {
        assert(C_ITER_AT_LEAST(*new_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*new_last, __first);
    }

    __C_ALGO_END_2(first, last)

    return removed;
}

size_t algo_remove_if(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_unary_predicate pred)
{
    if (!first || !last || !pred || !new_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t removed = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find_if(__first, __last, &__first, pred);
    if (C_ITER_NE(__first, __last)) {
        ++removed;
        c_iterator_t* __i = 0;
        C_ITER_COPY(&__i, __first);
        C_ITER_INC(__i);
        while (C_ITER_NE(__i, __last)) {
            if (!pred(C_ITER_DEREF(__i))) {
                C_ITER_DEREF_ASSIGN(__first, __i);
                C_ITER_INC(__first);
            }
            else
                ++removed;
            C_ITER_INC(__i);
        }
        __c_free(__i);
    }

    if (*new_last == 0)
        C_ITER_COPY(new_last, __first);
    else {
        assert(C_ITER_AT_LEAST(*new_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*new_last, __first);
    }

    __C_ALGO_END_2(first, last)

    return removed;
}

size_t algo_remove_copy(c_iterator_t* __c_forward_iterator first,
                        c_iterator_t* __c_forward_iterator last,
                        c_iterator_t* __c_forward_iterator d_first,
                        c_ref_t value,
                        c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !value || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (!(C_ITER_DEREF_EQUAL_V(__first, value))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++copied;
        }
        C_ITER_INC(__first);
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

size_t algo_remove_copy_if(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first,
                           c_iterator_t** __c_forward_iterator d_last,
                           c_unary_predicate pred)
{
    if (!first || !last || !d_first || !pred || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++copied;
        }
        C_ITER_INC(__first);
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

size_t algo_replace(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_ref_t old_value,
                    c_ref_t new_value)
{
    if (!first || !last || !old_value || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t replaced = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find(__first, __last, &__first, old_value);
    while (C_ITER_NE(__first, __last)) {
        if (C_ITER_DEREF_EQUAL_V(__first, old_value)) {
            C_ITER_DEREF_ASSIGN_V(__first, new_value);
            ++replaced;
        }
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return replaced;
}

size_t algo_replace_if(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_unary_predicate pred,
                       c_ref_t new_value)
{
    if (!first || !last || !pred || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    size_t replaced = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find_if(__first, __last, &__first, pred);
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN_V(__first, new_value);
            ++replaced;
        }
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return replaced;
}

size_t algo_replace_copy(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_ref_t old_value,
                         c_ref_t new_value,
                         c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !old_value || !new_value || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (C_ITER_DEREF_EQUAL_V(__first, old_value))
            C_ITER_DEREF_ASSIGN_V(__d_first, new_value);
        else
            C_ITER_DEREF_ASSIGN(__d_first, __first);
        C_ITER_INC(__d_first);
        C_ITER_INC(__first);
        ++copied;
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

size_t algo_replace_copy_if(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t* __c_forward_iterator d_first,
                            c_unary_predicate pred,
                            c_ref_t new_value,
                            c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !pred || !new_value || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first)))
            C_ITER_DEREF_ASSIGN_V(__d_first, new_value);
        else
            C_ITER_DEREF_ASSIGN(__d_first, __first);
        C_ITER_INC(__d_first);
        C_ITER_INC(__first);
        ++copied;
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}

void algo_swap(c_type_info_t* value_type,
               c_ref_t x,
               c_ref_t y)
{
    if (!value_type || !x || !y) return;

    c_ref_t tmp = malloc(value_type->size());
    if (tmp) {
        value_type->copy(tmp, x);
        value_type->assign(x, y);
        value_type->assign(y, tmp);
    }
    __c_free(tmp);
}

size_t algo_swap_range(c_iterator_t* __c_forward_iterator first1,
                       c_iterator_t* __c_forward_iterator last1,
                       c_iterator_t* __c_forward_iterator first2)
{
    if (!first1 || !last1 || !first2) return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first1));
    assert(C_ITER_MODIFIABLE(first2));

    size_t swapped = 0;

    __C_ALGO_BEGIN_3(first1, last1, first2);

    while (C_ITER_NE(__first1, __last1)) {
        algo_swap(__first1->value_type, C_ITER_DEREF(__first1), C_ITER_DEREF(__first2));
        C_ITER_INC(__first1);
        C_ITER_INC(__first2);
        ++swapped;
    }

    __C_ALGO_END_3(first1, last1, first2);

    return swapped;
}

void algo_iter_swap(c_iterator_t* __c_forward_iterator x,
                    c_iterator_t* __c_forward_iterator y)
{
    if (!x || !y) return;
    assert(C_ITER_AT_LEAST(x, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(y, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(x));
    assert(C_ITER_MODIFIABLE(y));
    algo_swap(x->value_type, C_ITER_DEREF(x), C_ITER_DEREF(y));
}

size_t algo_reverse(c_iterator_t* __c_bidirection_iterator first,
                    c_iterator_t* __c_bidirection_iterator last)
{
    if (!first || !last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_MODIFIABLE(first));
    assert(C_ITER_MODIFIABLE(last));

    size_t reversed = 0;

    __C_ALGO_BEGIN_2(first, last);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEC(__last);
        if (C_ITER_NE(__first, __last)) {
            algo_swap(__first->value_type, C_ITER_DEREF(__first), C_ITER_DEREF(__last));
            C_ITER_INC(__first);
            ++reversed;
        }
        ++reversed;
    }

    __C_ALGO_END_2(first, last);

    return reversed;
}

size_t algo_reverse_copy(c_iterator_t* __c_bidirection_iterator first,
                         c_iterator_t* __c_bidirection_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t reversed = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEC(__last);
        C_ITER_DEREF_ASSIGN(__d_first, __last);
        C_ITER_INC(__d_first);
        ++reversed;
    }

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return reversed;
}

void algo_rotate(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator n_first,
                 c_iterator_t* __c_forward_iterator last,
                 c_iterator_t** __c_forward_iterator rotate_point)
{
    if (!first || !n_first || !last || !rotate_point) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(n_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));
    assert(C_ITER_MODIFIABLE(n_first));

    if (C_ITER_EQ(first, n_first)) {
        if (*rotate_point == 0)
            C_ITER_COPY(rotate_point, last);
        else {
            assert(C_ITER_AT_LEAST(*rotate_point, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*rotate_point, last);
        }
        return;
    }

    if (C_ITER_EQ(n_first, last)) {
        if (*rotate_point == 0)
            C_ITER_COPY(rotate_point, first);
        else {
            assert(C_ITER_AT_LEAST(*rotate_point, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*rotate_point, first);
        }
        return;
    }

    __C_ALGO_BEGIN_3(first, n_first, last)

    c_iterator_t* __next = 0;
    C_ITER_COPY(&__next, __n_first);

    do {
        algo_iter_swap(__first, __next);
        C_ITER_INC(__first);
        C_ITER_INC(__next);
        if (C_ITER_EQ(__first, __n_first)) C_ITER_ASSIGN(__n_first, __next);
    } while (C_ITER_NE(__next, __last));

    if (*rotate_point == 0)
        C_ITER_COPY(rotate_point, __first);
    else {
        assert(C_ITER_AT_LEAST(*rotate_point, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*rotate_point, __first);
    }

    C_ITER_ASSIGN(__next, __n_first);
    while (C_ITER_NE(__next, __last)) {
        algo_iter_swap(__first, __next);
        C_ITER_INC(__first);
        C_ITER_INC(__next);
        if (C_ITER_EQ(__first, __n_first)) C_ITER_ASSIGN(__n_first, __next);
        if (C_ITER_EQ(__next, __last)) C_ITER_ASSIGN(__next, __n_first);
    }

    __c_free(__next);

    __C_ALGO_END_3(first, n_first, last)
}

size_t algo_rotate_copy(c_iterator_t* __c_forward_iterator first,
                        c_iterator_t* __c_forward_iterator n_first,
                        c_iterator_t* __c_forward_iterator last,
                        c_iterator_t* __c_forward_iterator d_first,
                        c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !n_first || !last || !d_first || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(n_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    size_t copied = 0;

    __C_ALGO_BEGIN_4(first, n_first, last, d_first)

    copied += algo_copy(__n_first, __last, __d_first, &__d_first);
    copied += algo_copy(__first, __n_first, __d_first, &__d_first);

    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_4(first, n_first, last, d_first)

    return copied;
}

void algo_random_shuffle(c_iterator_t* __c_random_iterator first,
                         c_iterator_t* __c_random_iterator last)
{
    algo_random_shuffle_by(first, last, algo_uniform);
}

void algo_random_shuffle_by(c_iterator_t* __c_random_iterator first,
                            c_iterator_t* __c_random_iterator last,
                            c_random_func r)
{
    if (!first || !last || !r) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
    assert(C_ITER_MODIFIABLE(first));

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t n = C_ITER_DISTANCE(__first, __last);
    c_iterator_t* __x = 0;
    c_iterator_t* __y = 0;

    for (ptrdiff_t i = n - 1; i > 0; --i) {
        __c_iter_move_copy(&__x, __first, i);
        __c_iter_move_copy(&__y, __first, (ptrdiff_t)(r(i + 1)));
        algo_iter_swap(__x, __y);
    }

    __c_free(__x);
    __c_free(__y);

    __C_ALGO_END_2(first, last)
}

size_t algo_unique_by(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_binary_predicate pred)
{
    if (!first || !last || !new_last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(first));

    if (C_ITER_EQ(first, last)) {
        if (*new_last == 0)
            C_ITER_COPY(new_last, last);
        else {
            assert(C_ITER_AT_LEAST(*new_last, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*new_last, last);
        }
        return 0;
    }

    size_t uniqued = 1;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __next = 0;
    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);
    while (C_ITER_NE(__next, __last)) {
        if (!pred(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
            C_ITER_INC(__first);
            C_ITER_DEREF_ASSIGN(__first, __next);
            ++uniqued;
        }
        C_ITER_INC(__next);
    }

    C_ITER_INC(__first);
    if (*new_last == 0)
        C_ITER_COPY(new_last, __first);
    else {
        assert(C_ITER_AT_LEAST(*new_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*new_last, __first);
    }
    __c_free(__next);

    __C_ALGO_END_2(first, last)

    return uniqued;
}

size_t algo_unique_copy_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first,
                           c_iterator_t** __c_forward_iterator d_last,
                           c_binary_predicate pred)
{
    if (!first || !last || !d_first || !d_last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_MODIFIABLE(d_first));

    if (C_ITER_EQ(first, last)) {
        if (*d_last == 0)
            C_ITER_COPY(d_last, d_first);
        else {
            assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*d_last, d_first);
        }
        return 0;
    }

    size_t copied = 1;

    __C_ALGO_BEGIN_3(first, last, d_first)

    C_ITER_DEREF_ASSIGN(__d_first, __first);
    C_ITER_INC(__first);

    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__d_first), C_ITER_DEREF(__first))) {
            C_ITER_INC(__d_first);
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            ++copied;
        }
        C_ITER_INC(__first);
    }

    C_ITER_INC(__d_first);
    if (*d_last == 0)
        C_ITER_COPY(d_last, __d_first);
    else {
        assert(C_ITER_AT_LEAST(*d_last, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*d_last, __d_first);
    }

    __C_ALGO_END_3(first, last, d_first)

    return copied;
}
