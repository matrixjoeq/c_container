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
    if (!first || !last || !d_first) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEREF_ASSIGN(__d_first, __first);
        C_ITER_INC(__first);
        C_ITER_INC(__d_first);
        ++n_copied;
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

size_t algo_copy_if(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t* __c_forward_iterator d_first,
                    c_iterator_t** __c_forward_iterator d_last,
                    c_unary_predicate pred)
{
    if (!first || !last || !d_first || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++n_copied;
        }
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

size_t algo_copy_backward(c_iterator_t* __c_bidirection_iterator first,
                          c_iterator_t* __c_bidirection_iterator last,
                          c_iterator_t* __c_bidirection_iterator d_last,
                          c_iterator_t** __c_bidirection_iterator d_last_copied)
{
    if (!first || !last || !d_last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(d_last, C_ITER_CATE_BIDIRECTION));
    assert(d_last_copied == 0 || *d_last_copied == 0 ||
           C_ITER_AT_LEAST(d_last_copied, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_MUTABLE(d_last));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_last);

    while (C_ITER_NE(__last, __first)) {
        C_ITER_DEC(__last);
        C_ITER_DEC(__d_last);
        C_ITER_DEREF_ASSIGN(__d_last, __last);
        ++n_copied;
    }

    __c_iter_copy_or_assign(d_last_copied, __d_last);

    __C_ALGO_END_3(first, last, d_last);

    return n_copied;
}

size_t algo_fill(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator last,
                 c_ref_t value)
{
    if (!first || !last || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_filled = 0;

    __C_ALGO_BEGIN_2(first, last);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEREF_ASSIGN_V(__first, value);
        C_ITER_INC(__first);
        ++n_filled;
    }

    __C_ALGO_END_2(first, last);

    return n_filled;
}

void algo_fill_n(c_iterator_t* __c_forward_iterator first,
                 size_t n,
                 c_ref_t value,
                 c_iterator_t** __c_forward_iterator last)
{
    if (!first || !value) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(last == 0 || *last == 0 || C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    __C_ALGO_BEGIN_1(first);

    while (n--) {
        C_ITER_DEREF_ASSIGN_V(__first, value);
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(last, __first);

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
    assert(C_ITER_MUTABLE(d_first));

    size_t n_transformed = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    const c_type_info_t* value_type = __first->value_type;
    c_ref_t __value = __c_allocate(value_type);

    value_type->create(__value);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_V_ASSIGN_DEREF(__value, __first);
        op(__value);
        C_ITER_DEREF_ASSIGN_V(__d_first, __value);
        C_ITER_INC(__first);
        C_ITER_INC(__d_first);
        ++n_transformed;
    }

    value_type->destroy(__value);
    __c_deallocate(value_type, __value);

    __C_ALGO_END_3(first, last, d_first);

    return n_transformed;
}

size_t algo_generate(c_iterator_t* __c_forward_iterator first,
                     c_iterator_t* __c_forward_iterator last,
                     c_generator_emplace gen)
{
    if (!first || !last || !gen) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_gened = 0;

    __C_ALGO_BEGIN_2(first, last)

    const c_type_info_t* value_type = __first->value_type;

    size_t __size = __first->value_type->size();
    c_ref_t __value = __c_allocate(value_type);

    while (C_ITER_NE(__first, __last)) {
        memset(__value, 0, __size);
        gen(__value);
        C_ITER_DEREF_ASSIGN_V(__first, __value);
        C_ITER_INC(__first);
        ++n_gened;
    }

    value_type->destroy(__value);
    __c_deallocate(value_type, __value);

    __C_ALGO_END_2(first, last)

    return n_gened;
}

void algo_generate_n(c_iterator_t* __c_forward_iterator first,
                     size_t n,
                     c_generator_emplace gen,
                     c_iterator_t** __c_forward_iterator last)
{
    if (!first || !gen) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(last == 0 || *last == 0 || C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    if (n == 0) {
        __c_iter_copy_or_assign(last, first);
        return;
    }

    __C_ALGO_BEGIN_1(first)

    const c_type_info_t* value_type = __first->value_type;
    size_t __size = __first->value_type->size();
    c_ref_t __value = __c_allocate(value_type);

    while (n--) {
        memset(__value, 0, __size);
        gen(__value);
        C_ITER_DEREF_ASSIGN_V(__first, __value);
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(last, __first);

    value_type->destroy(__value);
    __c_deallocate(value_type, __value);

    __C_ALGO_END_1(first)
}

size_t algo_remove(c_iterator_t* __c_forward_iterator first,
                   c_iterator_t* __c_forward_iterator last,
                   c_ref_t value,
                   c_iterator_t** __c_forward_iterator new_last)
{
    if (!first || !last || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(new_last == 0 || *new_last == 0 || C_ITER_AT_LEAST(new_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_removed = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find(__first, __last, &__first, value);
    if (C_ITER_NE(__first, __last)) {
        ++n_removed;
        c_iterator_t* __i = 0;
        __c_iter_copy_and_move(&__i, __first, 1);
        while (C_ITER_NE(__i, __last)) {
            if (!(C_ITER_DEREF_EQUAL_V(__i, value))) {
                C_ITER_DEREF_ASSIGN(__first, __i);
                C_ITER_INC(__first);
            }
            else {
                ++n_removed;
            }

            C_ITER_INC(__i);
        }
        __c_free(__i);
    }

    __c_iter_copy_or_assign(new_last, __first);

    __C_ALGO_END_2(first, last)

    return n_removed;
}

size_t algo_remove_if(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_unary_predicate pred)
{
    if (!first || !last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(new_last == 0 || *new_last == 0 || C_ITER_AT_LEAST(new_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_removed = 0;

    __C_ALGO_BEGIN_2(first, last)

    algo_find_if(__first, __last, &__first, pred);
    if (C_ITER_NE(__first, __last)) {
        ++n_removed;
        c_iterator_t* __i = 0;
        __c_iter_copy_and_move(&__i, __first, 1);
        while (C_ITER_NE(__i, __last)) {
            if (!pred(C_ITER_DEREF(__i))) {
                C_ITER_DEREF_ASSIGN(__first, __i);
                C_ITER_INC(__first);
            }
            else {
                ++n_removed;
            }
            C_ITER_INC(__i);
        }
        __c_free(__i);
    }

    __c_iter_copy_or_assign(new_last, __first);

    __C_ALGO_END_2(first, last)

    return n_removed;
}

size_t algo_remove_copy(c_iterator_t* __c_forward_iterator first,
                        c_iterator_t* __c_forward_iterator last,
                        c_iterator_t* __c_forward_iterator d_first,
                        c_ref_t value,
                        c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (!(C_ITER_DEREF_EQUAL_V(__first, value))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++n_copied;
        }
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

size_t algo_remove_copy_if(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first,
                           c_iterator_t** __c_forward_iterator d_last,
                           c_unary_predicate pred)
{
    if (!first || !last || !d_first || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            C_ITER_INC(__d_first);
            ++n_copied;
        }
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

size_t algo_replace(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_ref_t old_value,
                    c_ref_t new_value)
{
    if (!first || !last || !old_value || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_replaced = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_algo_find(__first, __last, &__first, old_value);
    while (C_ITER_NE(__first, __last)) {
        if (C_ITER_DEREF_EQUAL_V(__first, old_value)) {
            C_ITER_DEREF_ASSIGN_V(__first, new_value);
            ++n_replaced;
        }
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return n_replaced;
}

size_t algo_replace_if(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_unary_predicate pred,
                       c_ref_t new_value)
{
    if (!first || !last || !pred || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    size_t n_replaced = 0;

    __C_ALGO_BEGIN_2(first, last)

    algo_find_if(__first, __last, &__first, pred);
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN_V(__first, new_value);
            ++n_replaced;
        }
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return n_replaced;
}

size_t algo_replace_copy(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_ref_t old_value,
                         c_ref_t new_value,
                         c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !old_value || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (C_ITER_DEREF_EQUAL_V(__first, old_value)) {
            C_ITER_DEREF_ASSIGN_V(__d_first, new_value);
        }
        else {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
        }
        C_ITER_INC(__d_first);
        C_ITER_INC(__first);
        ++n_copied;
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

size_t algo_replace_copy_if(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t* __c_forward_iterator d_first,
                            c_unary_predicate pred,
                            c_ref_t new_value,
                            c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first || !pred || !new_value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            C_ITER_DEREF_ASSIGN_V(__d_first, new_value);
        }
        else {
            C_ITER_DEREF_ASSIGN(__d_first, __first);
        }
        C_ITER_INC(__d_first);
        C_ITER_INC(__first);
        ++n_copied;
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}

void algo_swap(const c_type_info_t* value_type,
               c_ref_t x,
               c_ref_t y)
{
    if (!value_type || !x || !y) return;

    c_ref_t tmp = __c_allocate(value_type);
    value_type->copy(tmp, x);
    value_type->assign(x, y);
    value_type->assign(y, tmp);
    value_type->destroy(tmp);
    __c_deallocate(value_type, tmp);
}

size_t algo_swap_range(c_iterator_t* __c_forward_iterator first1,
                       c_iterator_t* __c_forward_iterator last1,
                       c_iterator_t* __c_forward_iterator first2)
{
    if (!first1 || !last1 || !first2) return 0;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first1));
    assert(C_ITER_MUTABLE(first2));

    size_t n_swapped = 0;

    __C_ALGO_BEGIN_3(first1, last1, first2);

    while (C_ITER_NE(__first1, __last1)) {
        algo_swap(__first1->value_type, C_ITER_DEREF(__first1), C_ITER_DEREF(__first2));
        C_ITER_INC(__first1);
        C_ITER_INC(__first2);
        ++n_swapped;
    }

    __C_ALGO_END_3(first1, last1, first2);

    return n_swapped;
}

void algo_iter_swap(c_iterator_t* __c_forward_iterator x,
                    c_iterator_t* __c_forward_iterator y)
{
    if (!x || !y) return;
    assert(C_ITER_AT_LEAST(x, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(y, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(x));
    assert(C_ITER_MUTABLE(y));
    assert(x->value_type == y->value_type);
    algo_swap(x->value_type, C_ITER_DEREF(x), C_ITER_DEREF(y));
}

size_t algo_reverse(c_iterator_t* __c_bidirection_iterator first,
                    c_iterator_t* __c_bidirection_iterator last)
{
    if (!first || !last) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_MUTABLE(first));
    assert(C_ITER_MUTABLE(last));

    size_t n_reversed = 0;

    __C_ALGO_BEGIN_2(first, last);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEC(__last);
        if (C_ITER_NE(__first, __last)) {
            algo_swap(__first->value_type, C_ITER_DEREF(__first), C_ITER_DEREF(__last));
            C_ITER_INC(__first);
            ++n_reversed;
        }
        ++n_reversed;
    }

    __C_ALGO_END_2(first, last);

    return n_reversed;
}

size_t algo_reverse_copy(c_iterator_t* __c_bidirection_iterator first,
                         c_iterator_t* __c_bidirection_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_iterator_t** __c_forward_iterator d_last)
{
    if (!first || !last || !d_first) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_BIDIRECTION));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_reversed = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    while (C_ITER_NE(__first, __last)) {
        C_ITER_DEC(__last);
        C_ITER_DEREF_ASSIGN(__d_first, __last);
        C_ITER_INC(__d_first);
        ++n_reversed;
    }

    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_reversed;
}

void algo_rotate(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator n_first,
                 c_iterator_t* __c_forward_iterator last,
                 c_iterator_t** __c_forward_iterator rotate_point)
{
    if (!first || !n_first || !last) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(n_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(rotate_point == 0 || *rotate_point == 0 ||
           C_ITER_AT_LEAST(rotate_point, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));
    assert(C_ITER_MUTABLE(n_first));

    if (C_ITER_EQ(first, n_first)) {
        __c_iter_copy_or_assign(rotate_point, last);
        return;
    }

    if (C_ITER_EQ(n_first, last)) {
        __c_iter_copy_or_assign(rotate_point, first);
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

    __c_iter_copy_or_assign(rotate_point, __first);

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
    if (!first || !n_first || !last || !d_first) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(n_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(d_first, C_ITER_CATE_FORWARD));
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    size_t n_copied = 0;

    __C_ALGO_BEGIN_4(first, n_first, last, d_first)

    n_copied += algo_copy(__n_first, __last, __d_first, &__d_first);
    n_copied += algo_copy(__first, __n_first, __d_first, &__d_first);
    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_4(first, n_first, last, d_first)

    return n_copied;
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
    assert(C_ITER_MUTABLE(first));

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t __n = C_ITER_DISTANCE(__first, __last);
    c_iterator_t* __x = 0;
    c_iterator_t* __y = 0;

    for (ptrdiff_t __i = __n - 1; __i > 0; --__i) {
        __c_iter_copy_and_move(&__x, __first, __i);
        __c_iter_copy_and_move(&__y, __first, (ptrdiff_t)(r(__i + 1)));
        algo_iter_swap(__x, __y);
    }

    __c_free(__y);
    __c_free(__x);

    __C_ALGO_END_2(first, last)
}

size_t algo_unique_by(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_binary_predicate pred)
{
    if (!first || !last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(new_last == 0 || *new_last == 0 || C_ITER_AT_LEAST(new_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(first));

    if (C_ITER_EQ(first, last)) {
        __c_iter_copy_or_assign(new_last, last);
        return 0;
    }

    size_t n_uniqued = 1;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __next = 0;

    __c_iter_copy_and_move(&__next, __first, 1);
    while (C_ITER_NE(__next, __last)) {
        if (!pred(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
            C_ITER_INC(__first);
            C_ITER_DEREF_ASSIGN(__first, __next);
            ++n_uniqued;
        }
        C_ITER_INC(__next);
    }

    C_ITER_INC(__first);
    __c_iter_copy_or_assign(new_last, __first);

    __c_free(__next);

    __C_ALGO_END_2(first, last)

    return n_uniqued;
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
    assert(d_last == 0 || *d_last == 0 || C_ITER_AT_LEAST(d_last, C_ITER_CATE_FORWARD));
    assert(C_ITER_MUTABLE(d_first));

    if (C_ITER_EQ(first, last)) {
        __c_iter_copy_or_assign(d_last, d_first);
        return 0;
    }

    size_t n_copied = 1;

    __C_ALGO_BEGIN_3(first, last, d_first)

    C_ITER_DEREF_ASSIGN(__d_first, __first);
    C_ITER_INC(__first);

    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__d_first), C_ITER_DEREF(__first))) {
            C_ITER_INC(__d_first);
            C_ITER_DEREF_ASSIGN(__d_first, __first);
            ++n_copied;
        }
        C_ITER_INC(__first);
    }

    C_ITER_INC(__d_first);
    __c_iter_copy_or_assign(d_last, __d_first);

    __C_ALGO_END_3(first, last, d_first)

    return n_copied;
}
