
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

size_t algo_copy_backward(c_iterator_t* __c_bidirection_iterator first,
                          c_iterator_t* __c_bidirection_iterator last,
                          c_iterator_t* __c_bidirection_iterator d_last,
                          c_iterator_t** __c_bidirection_iterator d_last_copied)
{
    if (!first || !last || !d_last || !d_last_copied) return 0;

    size_t copied = 0;

    __C_ALGO_BEGIN_3(first, last, d_last);

    while (C_ITER_NE(__last, __first)) {
        C_ITER_DEC(__last);
        C_ITER_DEC(__d_last);
        __d_last->type_info->assign(C_ITER_DEREF(__d_last), C_ITER_DEREF(__last));
        ++copied;
    }

    if (*d_last_copied == 0)
        C_ITER_COPY(d_last_copied, __d_last);
    else
        C_ITER_ASSIGN(*d_last_copied, __d_last);

    __C_ALGO_END_3(first, last, d_last);

    return copied;
}

size_t algo_fill(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator last,
                 c_ref_t value)
{
    if (!first || !last || !value) return 0;

    size_t filled = 0;

    __C_ALGO_BEGIN_2(first, last);

    while (C_ITER_NE(__first, __last)) {
        __first->type_info->assign(C_ITER_DEREF(__first), value);
        C_ITER_INC(__first);
        ++filled;
    }

    __C_ALGO_END_2(first, last);

    return filled;
}

void algo_fill_n(c_iterator_t* __c_forward_iterator first,
                 size_t n,
                 c_ref_t value,
                 c_iterator_t** __c_forward_iterator last_filled)
{
    if (!first || !value || !last_filled) return;

    __C_ALGO_BEGIN_1(first);

    while (n--) {
        __first->type_info->assign(C_ITER_DEREF(__first), value);
        C_ITER_INC(__first);
    }

    if (*last_filled == 0)
        C_ITER_COPY(last_filled, __first);
    else
        C_ITER_ASSIGN(*last_filled, __first);

    __C_ALGO_END_1(first);
}

size_t algo_transform(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t* __c_forward_iterator d_first,
                      c_unary_func op)
{
    if (!first || !last || !d_first || !op) return 0;

    size_t transformed = 0;

    __C_ALGO_BEGIN_3(first, last, d_first)

    c_ref_t __value = malloc(__first->type_info->size());
    assert(__value);
    __first->type_info->create(__value);

    while (C_ITER_NE(__first, __last)) {
        __first->type_info->assign(__value, C_ITER_DEREF(__first));
        op(__value);
        __d_first->type_info->assign(C_ITER_DEREF(__d_first), __value);
        C_ITER_INC(__first);
        C_ITER_INC(__d_first);
        ++transformed;
    }

    __c_free(__value);

    __C_ALGO_END_3(first, last, d_first);

    return transformed;
}

void algo_swap(c_containable_t* type_info,
               c_ref_t x,
               c_ref_t y)
{
    if (!type_info || !x || !y) return;

    c_ref_t tmp = malloc(type_info->size());
    if (tmp) {
        type_info->copy(tmp, x);
        type_info->assign(x, y);
        type_info->assign(y, tmp);
    }
    __c_free(tmp);
}

void algo_iter_swap(c_iterator_t* __c_forward_iterator x,
                    c_iterator_t* __c_forward_iterator y)
{
    if (!x || !y) return;
    assert(C_ITER_AT_LEAST(x, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(y, C_ITER_CATE_FORWARD));
    algo_swap(x->type_info, C_ITER_DEREF(x), C_ITER_DEREF(y));
}
