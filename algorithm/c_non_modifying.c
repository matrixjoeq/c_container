
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

    size_t oped = 0;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        op(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
        ++oped;
    }

    __C_ALGO_END_2(first, last)

    return oped;
}

void algo_for_each_n(c_iterator_t* __c_input_iterator first,
                     size_t n,
                     c_iterator_t** __c_input_iterator reached,
                     c_unary_func op)
{
    if (!first || !reached || !op) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));

    __C_ALGO_BEGIN_1(first);

    while (n--) {
        op(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }

    if (*reached == 0)
        C_ITER_COPY(reached, __first);
    else
        C_ITER_ASSIGN(*reached, __first);

    __C_ALGO_END_1(first);
}

size_t algo_count(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_ref_t value)
{
    if (!first || !last || !value) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t count = 0;

    __C_ALGO_BEGIN_2(first, last)

    c_containable_t* type_info = __first->type_info;
    while (C_ITER_NE(__first, __last)) {
        if (type_info->equal(value, C_ITER_DEREF(__first))) ++count;
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return count;
}

size_t algo_count_if(c_iterator_t* __c_input_iterator first,
                     c_iterator_t* __c_input_iterator last,
                     c_unary_predicate pred)
{
    if (!first || !last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t count = 0;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) ++count;
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)

    return count;
}

bool algo_mismatch_by(c_iterator_t* __c_input_iterator first,
                      c_iterator_t* __c_input_iterator last,
                      c_iterator_t* __c_input_iterator first2,
                      c_iterator_t** __c_input_iterator mismatch1,
                      c_iterator_t** __c_input_iterator mismatch2,
                      c_binary_predicate pred)
{
    if (!first || !last || !first2 || !mismatch1 || !mismatch2 || !pred) return false;

    bool is_mismatch = false;

    __C_ALGO_BEGIN_3(first, last, first2)

    c_iterator_t* __m1 = 0;
    c_iterator_t* __m2 = 0;
    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first), C_ITER_DEREF(__first2))) {
        C_ITER_INC(__first);
        C_ITER_INC(__first2);
    }

    is_mismatch = C_ITER_NE(__first, __last);
    C_ITER_COPY(&__m1, __first);
    C_ITER_COPY(&__m2, __first2);

    if (*mismatch1 == 0)
        C_ITER_COPY(mismatch1, __m1);
    else
        C_ITER_ASSIGN(*mismatch1, __m1);

    if (*mismatch2 == 0)
        C_ITER_COPY(mismatch2, __m2);
    else
        C_ITER_ASSIGN(*mismatch2, __m2);
    __c_free(__m2);
    __c_free(__m1);

    __C_ALGO_END_3(first, last, first2)

    return is_mismatch;
}

bool algo_equal_by(c_iterator_t* __c_input_iterator first,
                   c_iterator_t* __c_input_iterator last,
                   c_iterator_t* __c_input_iterator first2,
                   c_binary_predicate pred)
{
    if (!first || !last || !first2 || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_INPUT));

    bool is_equal = false;

    __C_ALGO_BEGIN_3(first, last, first2)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first), C_ITER_DEREF(__first2))) {
        C_ITER_INC(__first);
        C_ITER_INC(__first2);
    }
    is_equal = C_ITER_EQ(__first, __last);

    __C_ALGO_END_3(first, last, first2)

    return is_equal;
}

bool algo_find_by(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_ref_t value,
                  c_binary_predicate pred)
{
    if (!first || !last || !found || !value || !pred) return false;

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first), value))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    if (*found == 0)
        C_ITER_COPY(found, __first);
    else
        C_ITER_ASSIGN(*found, __first);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_find_if(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_unary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && !pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    if (*found == 0)
        C_ITER_COPY(found, __first);
    else
        C_ITER_ASSIGN(*found, __first);

    __C_ALGO_END_2(first, last)

    return is_found;
}

bool algo_find_if_not(c_iterator_t* __c_input_iterator first,
                      c_iterator_t* __c_input_iterator last,
                      c_iterator_t** __c_input_iterator found,
                      c_unary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last) && pred(C_ITER_DEREF(__first)))
        C_ITER_INC(__first);
    is_found = C_ITER_NE(__first, __last);

    if (*found == 0)
        C_ITER_COPY(found, __first);
    else
        C_ITER_ASSIGN(*found, __first);

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
    if (!first || !last || !s_first || !s_last || !found || !pred) return false;

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

    if (*found == 0)
        C_ITER_COPY(found, __first);
    else
        C_ITER_ASSIGN(*found, __first);

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
    if (!first || !last || !s_first || !s_last || !found || !pred) return false;

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    c_iterator_t* __found = 0;
    C_ITER_COPY(&__found, __last);
    while (c_algo_find_first_of_by(__first, __last, s_first, s_last, &__first, pred)) {
        is_found = true;
        C_ITER_ASSIGN(__found, __first);
        C_ITER_INC(__first);
    }

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);
    __c_free(__found);

    __C_ALGO_END_4(first, last, s_first, s_last)

    return is_found;
}

bool algo_adjacent_find_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t** __c_forward_iterator found,
                           c_binary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    if (C_ITER_EQ(first, last)) {
        if (*found == 0)
            C_ITER_COPY(found, last);
        else
            C_ITER_ASSIGN(*found, last);
        return false;
    }

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __found = 0; // in case *found == first
    c_iterator_t* __next = 0;
    C_ITER_COPY(&__found, __last);
    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);
    while (C_ITER_NE(__next, __last) && !pred(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
        C_ITER_ASSIGN(__first, __next);
        C_ITER_INC(__next);
    }
    is_found = C_ITER_NE(__next, __last);
    if (is_found) C_ITER_ASSIGN(__found, __first);

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);
    __c_free(__found);
    __c_free(__next);

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
    if (!first || !last || !s_first || !s_last || !found || !pred) return false;

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

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);

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
    if (!first || !last || !s_first || !s_last || !found || !pred) return false;

    bool is_found = false;

    __C_ALGO_BEGIN_4(first, last, s_first, s_last)

    c_iterator_t* __found = 0;
    C_ITER_COPY(&__found, __last);

    while (c_algo_search_by(__first, __last, __s_first, __s_last, &__first, pred)) {
        is_found = true;
        C_ITER_ASSIGN(__found, __first);
        C_ITER_INC(__first);
    }

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);
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
    if (!first || !last || !value || !found || !pred) return false;

    if (n == 0) {
        if (*found == 0)
            C_ITER_COPY(found, last);
        else
            C_ITER_ASSIGN(*found, last);
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

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);
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
    if (!first || !last || !value || !found || !pred) return false;

    if (n == 0) {
        if (*found == 0)
            C_ITER_COPY(found, last);
        else
            C_ITER_ASSIGN(*found, last);
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

    if (*found == 0)
        C_ITER_COPY(found, __found);
    else
        C_ITER_ASSIGN(*found, __found);
    __c_free(__found);

    __C_ALGO_END_2(first, last)

    return is_found;
}
