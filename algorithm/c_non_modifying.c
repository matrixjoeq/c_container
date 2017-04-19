
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

bool algo_all_of(c_iterator_t* first, c_iterator_t* last, c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = true;
    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__first))) {
            ret = false;
            break;
        }
        C_ITER_INC(__first);
    }
    __C_ALGO_END

    return ret;
}

bool algo_any_of(c_iterator_t* first, c_iterator_t* last, c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = false;
    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            ret = true;
            break;
        }
        C_ITER_INC(__first);
    }
    __C_ALGO_END

    return ret;
}

bool algo_none_of(c_iterator_t* first, c_iterator_t* last, c_unary_predicate pred)
{
    if (!first || !last || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    bool ret = true;
    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            ret = false;
            break;
        }
        C_ITER_INC(__first);
    }
    __C_ALGO_END

    return ret;
}

void algo_for_each(c_iterator_t* first, c_iterator_t* last, c_unary_func func)
{
    if (!first || !last || !func) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        func(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }
    __C_ALGO_END
}

void algo_for_each_n(c_iterator_t* first, size_t n, c_iterator_t** reach, c_unary_func func)
{
    if (!first || !reach || !func) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));

    c_iterator_operation_t* iter_ops = first->iterator_ops;
    c_iterator_t* __first = 0;
    iter_ops->alloc_and_copy(&__first, first);

    if (*reach == 0)
        iter_ops->alloc_and_copy(reach, first);

    while (n--) {
        func(C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }

    iter_ops->assign(*reach, __first);

    __c_free(__first);
}

size_t algo_count(c_iterator_t* first, c_iterator_t* last, c_ref_t data)
{
    if (!first || !last || !data) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t ret = 0;

    __C_ALGO_BEGIN
    c_containable_t* type_info = __first->type_info;
    while (C_ITER_NE(__first, __last)) {
        if (type_info->equal(data, C_ITER_DEREF(__first))) ++ret;
        C_ITER_INC(__first);
    }
    __C_ALGO_END

    return ret;
}

size_t algo_count_if(c_iterator_t* first, c_iterator_t* last, c_unary_predicate pred)
{
    if (!first || !last || !pred) return 0;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    size_t ret = 0;

    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) ++ret;
        C_ITER_INC(__first);
    }
    __C_ALGO_END

    return ret;
}

bool algo_equal_by(c_iterator_t* first, c_iterator_t* last, c_iterator_t* first2, c_binary_predicate pred)
{
    if (!first || !last || !first2 || !pred) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(first2, C_ITER_CATE_INPUT));

    bool equal = true;
    __C_ALGO_BEGIN

    c_iterator_t* __first2 = 0;
    C_ITER_COPY(&__first2, first2);

    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__first), C_ITER_DEREF(__first2))) {
            equal = false;
            break;
        }
        C_ITER_INC(__first);
        C_ITER_INC(__first2);
    }

    __c_free(__first2);

    __C_ALGO_END

    return equal;
}

bool algo_find(c_iterator_t* first, c_iterator_t* last, c_iterator_t** found, const c_ref_t value)
{
    if (!first || !last || !found || !value) return false;

    bool is_found = false;

    if (*found == 0)
        C_ITER_COPY(found, last);

    __C_ALGO_BEGIN
    c_containable_t* type_info = __first->type_info;
    while (C_ITER_NE(__first, __last)) {
        if (type_info->equal(C_ITER_DEREF(__first), value)) {
            is_found = true;
            break;
        }
        C_ITER_INC(__first);
    }
    C_ITER_ASSIGN(*found, __first);
    __C_ALGO_END

    return is_found;
}

bool algo_find_if(c_iterator_t* first, c_iterator_t* last, c_iterator_t** found, c_unary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    bool is_found = false;

    if (*found == 0)
        C_ITER_COPY(found, last);

    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (pred(C_ITER_DEREF(__first))) {
            is_found = true;
            break;
        }
        C_ITER_INC(__first);
    }
    C_ITER_ASSIGN(*found, __first);
    __C_ALGO_END

    return is_found;
}

bool algo_find_if_not(c_iterator_t* first, c_iterator_t* last, c_iterator_t** found, c_unary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    bool is_found = false;

    if (*found == 0)
        C_ITER_COPY(found, last);

    __C_ALGO_BEGIN
    while (C_ITER_NE(__first, __last)) {
        if (!pred(C_ITER_DEREF(__first))) {
            is_found = true;
            break;
        }
        C_ITER_INC(__first);
    }
    C_ITER_ASSIGN(*found, __first);
    __C_ALGO_END

    return is_found;
}

bool algo_find_first_of_by(c_iterator_t* first, c_iterator_t* last,
                           c_iterator_t* s_first, c_iterator_t* s_last,
                           c_iterator_t** found, c_binary_predicate pred)
{
    if (!first || !last || !s_first || !s_last || !found || !pred) return false;

    bool is_found = false;

    if (*found == 0)
        C_ITER_COPY(found, last);

    __C_ALGO_BEGIN
    c_iterator_t* __s_first = 0;
    c_iterator_t* __s_last = 0;
    C_ITER_COPY(&__s_first, s_first);
    C_ITER_COPY(&__s_last, s_last);
    while (!is_found && C_ITER_NE(__first, __last)) {
        while (C_ITER_NE(__s_first, __s_last)) {
            if (pred(C_ITER_DEREF(__first), C_ITER_DEREF(__s_first))) {
                is_found = true;
                break;
            }
            C_ITER_INC(__s_first);
        }
        if (!is_found) C_ITER_INC(__first);
    }
    C_ITER_ASSIGN(*found, __first);
    __c_free(__s_last);
    __c_free(__s_first);
    __C_ALGO_END

    return is_found;
}

bool algo_adjacent_find_by(c_iterator_t* first, c_iterator_t* last, c_iterator_t** found, c_binary_predicate pred)
{
    if (!first || !last || !found || !pred) return false;

    bool is_found = false;

    if (*found == 0)
        C_ITER_COPY(found, last);

    __C_ALGO_BEGIN
    c_iterator_t* __next = 0;
    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);
    while (C_ITER_NE(__next, __last)) {
        if (pred(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
            is_found = true;
            break;
        }
        C_ITER_INC(__first);
        C_ITER_INC(__next);
    }
    C_ITER_ASSIGN(*found, __first);
    __c_free(__next);
    __C_ALGO_END

    return is_found;
}

