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

bool algo_all_of(c_iterator_t* __c_input_iterator first,
                 c_iterator_t* __c_input_iterator last,
                 c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    ret = C_ITER_EQ(__first, __last);

    __C_ALGO_END_2(first, last)

    return ret;
}

bool algo_any_of(c_iterator_t* __c_input_iterator first,
                 c_iterator_t* __c_input_iterator last,
                 c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    ret = C_ITER_NE(__first, __last);

    __C_ALGO_END_2(first, last)

    return ret;
}

bool algo_none_of(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    ret = C_ITER_EQ(__first, __last);

    __C_ALGO_END_2(first, last)

    return ret;
}

size_t algo_for_each(c_iterator_t* __c_input_iterator first,
                     c_iterator_t* __c_input_iterator last,
                     c_unary_func op)
{
    if (!first || !last || !op) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t n_oped = 0;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        op(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
        ++n_oped;
    }

    __C_ALGO_END_2(first, last)

    return n_oped;
}

void algo_for_each_n(c_iterator_t* __c_input_iterator first,
                     size_t n,
                     c_iterator_t** __c_input_iterator reached,
                     c_unary_func op)
{
    if (!first || !op) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(reached == 0 || *reached == 0 || C_ITER_AT_LEAST(*reached, C_ITER_CATE_INPUT));

    __C_ALGO_BEGIN_1(first);

    while (n--) {
        op(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(reached, __first);

    __C_ALGO_END_1(first);
}

size_t algo_count(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_ref_t value)
{
    if (!first || !last || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t n_count = 0;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        if (C_ITER_DEREF_EQUAL_V(__first, value)) ++n_count;
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return n_count;
}

size_t algo_count_if(c_iterator_t* __c_input_iterator first,
                     c_iterator_t* __c_input_iterator last,
                     c_unary_predicate pred)
{
    if (!first || !last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t n_count = 0;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) ++n_count;
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return n_count;
}

bool algo_mismatch_by(c_iterator_t* __c_input_iterator first1,
                      c_iterator_t* __c_input_iterator last1,
                      c_iterator_t* __c_input_iterator first2,
                      c_iterator_t** __c_input_iterator mismatch1,
                      c_iterator_t** __c_input_iterator mismatch2,
                      c_binary_predicate pred)
{
    if (!first1 || !last1 || !first2 || !pred) return false;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_INPUT));
    assert(mismatch1 == 0 || *mismatch1 == 0 || C_ITER_AT_LEAST(*mismatch1, C_ITER_CATE_INPUT));
    assert(mismatch2 == 0 || *mismatch2 == 0 || C_ITER_AT_LEAST(*mismatch2, C_ITER_CATE_INPUT));

    bool is_mismatch = false;

    __C_ALGO_BEGIN_3(first1, last1, first2)

    c_iterator_t* __m1 = 0;
    c_iterator_t* __m2 = 0;

    while (C_ITER_NE(__first1, __last1) && pred(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
        C_ITER_INC(__first1);
        C_ITER_INC(__first2);
    }

    is_mismatch = C_ITER_NE(__first1, __last1);
    C_ITER_COPY(&__m1, __first1);
    C_ITER_COPY(&__m2, __first2);

    __c_iter_copy_or_assign(mismatch1, __m1);
    __c_iter_copy_or_assign(mismatch2, __m2);

    __c_free(__m2);
    __c_free(__m1);

    __C_ALGO_END_3(first1, last1, first2)

    return is_mismatch;
}

bool algo_equal_by(c_iterator_t* __c_input_iterator first1,
                   c_iterator_t* __c_input_iterator last1,
                   c_iterator_t* __c_input_iterator first2,
                   c_binary_predicate pred)
{
    if (!first1 || !last1 || !first2 || !pred) return false;
    assert(C_ITER_AT_LEAST(first1, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last1, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_INPUT));

    bool is_equal = false;

    __C_ALGO_BEGIN_3(first1, last1, first2)

    while (C_ITER_NE(__first1, __last1) && pred(C_ITER_DEREF(__first1), C_ITER_DEREF(__first2))) {
        C_ITER_INC(__first1);
        C_ITER_INC(__first2);
    }
    is_equal = C_ITER_EQ(__first1, __last1);

    __C_ALGO_END_3(first1, last1, first2)

    return is_equal;
}

bool algo_find_by(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_ref_t value,
                  c_binary_predicate pred)
{
    if (!first || !last || !value || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_INPUT));

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first), value))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    __c_iter_copy_or_assign(found, __first);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_find_if(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_INPUT));

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    __c_iter_copy_or_assign(found, __first);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_find_if_not(c_iterator_t* __c_input_iterator first,
                      c_iterator_t* __c_input_iterator last,
                      c_iterator_t** __c_input_iterator found,
                      c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_INPUT));

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    __c_iter_copy_or_assign(found, __first);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_find_first_of_by(c_iterator_t* __c_input_iterator first,
                           c_iterator_t* __c_input_iterator last,
                           c_iterator_t* __c_forward_iterator s_first,
                           c_iterator_t* __c_forward_iterator s_last,
                           c_iterator_t** __c_input_iterator found,
                           c_binary_predicate pred)
{
    if (!first || !last || !s_first || !s_last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(s_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_INPUT));

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    while (!is_found && C_ITER_NE(__first, __last)) {
        C_ITER_ASSIGN(__s_first, s_first);
        while (C_ITER_NE(__s_first, __s_last)) {
            if (pred(C_ITER_DEREF(__first), C_ITER_DEREF(__s_first))) {
                is_found = true;
                break;
            }
            C_ITER_INC(__s_first);
        }
        if (!is_found) C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(found, __first);

    __C_ALGO_END_4(first, last, s_first, s_last)

    return is_found;
}

bool algo_find_last_of_by(c_iterator_t* __c_input_iterator first,
                          c_iterator_t* __c_input_iterator last,
                          c_iterator_t* __c_forward_iterator s_first,
                          c_iterator_t* __c_forward_iterator s_last,
                          c_iterator_t** __c_input_iterator found,
                          c_binary_predicate pred)
{
    if (!first || !last || !s_first || !s_last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(s_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_INPUT));

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    c_iterator_t* __found = 0;

    C_ITER_COPY(&__found, __last);
    while (c_algo_find_first_of_by(__first, __last, s_first, s_last, &__first, pred)) {
        is_found = true;
        C_ITER_ASSIGN(__found, __first);
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(found, __found);

    __c_free(__found);

    __C_ALGO_END_4(first, last, s_first, s_last)

    return is_found;
}

bool algo_adjacent_find_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t** __c_forward_iterator found,
                           c_binary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_FORWARD));

    if (C_ITER_EQ(first, last)) {
        __c_iter_copy_or_assign(found, last);
        return false;
    }

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __found = 0; // in case *found == first
    c_iterator_t* __next = 0;

    C_ITER_COPY(&__found, __last);
    __c_iter_copy_and_move(&__next, __first, 1);
    while (C_ITER_NE(__next, __last) && !pred(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
        C_ITER_ASSIGN(__first, __next);
        C_ITER_INC(__next);
    }
    is_found = C_ITER_NE(__next, __last);
    if (is_found) C_ITER_ASSIGN(__found, __first);

    __c_iter_copy_or_assign(found, __found);

    __c_free(__next);
    __c_free(__found);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_search_by(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t* __c_forward_iterator s_first,
                    c_iterator_t* __c_forward_iterator s_last,
                    c_iterator_t** __c_forward_iterator found,
                    c_binary_predicate pred)
{
    if (!first || !last || !s_first || !s_last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_FORWARD));

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    c_iterator_t* __found = 0;
    c_iterator_t* __i = 0;
    c_iterator_t* __s = 0;

    C_ITER_COPY(&__found, __last);
    C_ITER_COPY(&__i, __first);
    C_ITER_COPY(&__s, __s_first);

    while (C_ITER_NE(__first, __last)) {
        while (C_ITER_NE(__i, __last) && C_ITER_NE(__s, __s_last) &&
               pred(C_ITER_DEREF(__i), C_ITER_DEREF(__s))) {
            C_ITER_INC(__i);
            C_ITER_INC(__s);
        }

        if (C_ITER_EQ(__s, __s_last)) {
            is_found = true;
            break;
        }

        C_ITER_INC(__first);
        C_ITER_ASSIGN(__i, __first);
        C_ITER_ASSIGN(__s, __s_first);
    }

    if (is_found) C_ITER_ASSIGN(__found, __first);

    __c_iter_copy_or_assign(found, __found);

    __c_free(__s);
    __c_free(__i);
    __c_free(__found);

    __C_ALGO_END_4(first, last, s_first, s_last)

    return is_found;
}

bool algo_search_last_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t* __c_forward_iterator s_first,
                         c_iterator_t* __c_forward_iterator s_last,
                         c_iterator_t** __c_forward_iterator found,
                         c_binary_predicate pred)
{
    if (!first || !last || !s_first || !s_last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(s_last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_FORWARD));

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    c_iterator_t* __found = 0;

    C_ITER_COPY(&__found, __last);
    while (c_algo_search_by(__first, __last, __s_first, __s_last, &__first, pred)) {
        is_found = true;
        C_ITER_ASSIGN(__found, __first);
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(found, __found);

    __c_free(__found);

    __C_ALGO_END_4(first, last, s_first, s_last)

    return is_found;
}

bool algo_search_n_by(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      size_t n,
                      c_ref_t value,
                      c_iterator_t** __c_forward_iterator found,
                      c_binary_predicate pred)
{
    if (!first || !last || !value || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_FORWARD));

    if (n == 0) {
        __c_iter_copy_or_assign(found, last);
        return false;
    }

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __found = 0;
    c_iterator_t* __i = 0;

    C_ITER_COPY(&__found, __last);
    C_ITER_COPY(&__i, __first);
    while (algo_find_by(__i, __last, &__first, value, pred)) {
        size_t __n = n - 1;
        C_ITER_ASSIGN(__i, __first);
        C_ITER_INC(__i);
        while (__n != 0 && C_ITER_NE(__i, __last) && pred(C_ITER_DEREF(__i), value)) {
            C_ITER_INC(__i);
            --__n;
        }

        if (__n == 0) {
            is_found = true;
            break;
        }
    }
    if (is_found) C_ITER_ASSIGN(__found, __first);

    __c_iter_copy_or_assign(found, __found);

    __c_free(__i);
    __c_free(__found);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_search_last_n_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           size_t n,
                           c_ref_t value,
                           c_iterator_t** __c_forward_iterator found,
                           c_binary_predicate pred)
{
    if (!first || !last || !value || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));
    assert(found == 0 || *found == 0 || C_ITER_AT_LEAST(*found, C_ITER_CATE_FORWARD));

    if (n == 0) {
        __c_iter_copy_or_assign(found, last);
        return false;
    }

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __found = 0;

    C_ITER_COPY(&__found, __last);
    while (c_algo_search_n_by(__first, __last, n, value, &__first, pred)) {
        is_found = true;
        C_ITER_ASSIGN(__found, __first);
        C_ITER_INC(__first);
    }

    __c_iter_copy_or_assign(found, __found);

    __c_free(__found);

    __C_ALGO_END_2(first, last)

    return is_found;
}
