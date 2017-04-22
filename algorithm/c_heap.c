
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

__c_static c_iterator_t* random_add(c_iterator_t** dst, c_iterator_t* src, ptrdiff_t n)
{
    if (!dst) return 0;

    c_iterator_operation_t* iter_ops = src->iterator_ops;

    if (*dst) {
        iter_ops->assign(*dst, src);
    }
    else {
        iter_ops->alloc_and_copy(dst, src);
    }
    iter_ops->advance(*dst, n);

    return *dst;
}

__c_static void push_heap(c_iterator_t* first, c_iterator_t* last,
                          ptrdiff_t top_index, ptrdiff_t hole_index, c_compare comp)
{
    __C_ALGO_BEGIN_2(first, last)

    C_ITER_DEC(__last);
    c_containable_t* type_info = __first->type_info;
    c_ref_t __value = malloc(type_info->size());
    type_info->copy(__value, C_ITER_DEREF(__last));
    ptrdiff_t parent_index = (hole_index - 1) / 2;
    c_iterator_t* __tmp_parent = 0;
    c_iterator_t* __tmp_hole = 0;
    while (hole_index > top_index) {
        random_add(&__tmp_parent, __first, parent_index);
        random_add(&__tmp_hole, __first, hole_index);
        if (comp(C_ITER_DEREF(__tmp_parent), __value)) {
            type_info->assign(C_ITER_DEREF(__tmp_hole), C_ITER_DEREF(__tmp_parent));
            hole_index = parent_index;
            parent_index = (hole_index - 1) / 2;
        }
        else
            break;
    }
    random_add(&__tmp_hole, __first, hole_index);
    type_info->assign(C_ITER_DEREF(__tmp_hole), __value);
    __c_free(__tmp_hole);
    __c_free(__tmp_parent);
    __c_free(__value);

    __C_ALGO_END_2(first, last);
}

__c_static void adjust_heap(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    __c_unuse(first);
    __c_unuse(last);
    __c_unuse(comp);
}

bool algo_is_heap_by(c_iterator_t* __c_random_iterator first,
                     c_iterator_t* __c_random_iterator last,
                     c_compare comp)
{
    if (!first || !last || !comp) return false;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __until = 0;
    algo_is_heap_until_by(__first, __last, &__until, comp);
    bool is_heap = C_ITER_EQ(__until, __last);
    __c_free(__until);

    __C_ALGO_END_2(first, last);

    return is_heap;
}

void algo_is_heap_until_by(c_iterator_t* __c_random_iterator first,
                           c_iterator_t* __c_random_iterator last,
                           c_iterator_t** __c_random_iterator until,
                           c_compare comp)
{
    if (!first || !last || !until || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last);

    c_iterator_operation_t* iter_ops = __first->iterator_ops;
    if (*until == 0)
        iter_ops->alloc_and_copy(until, __first);

    bool is_heap = true;
    ptrdiff_t __distance = iter_ops->distance(__first, __last);
    ptrdiff_t __parent_index = 0;
    ptrdiff_t __left_index = (__parent_index + 1) * 2 - 1;
    ptrdiff_t __right_index = (__parent_index + 1) * 2;
    c_iterator_t* __parent = 0;
    c_iterator_t* __left = 0;
    c_iterator_t* __right = 0;
    while (__left_index < __distance) {
        random_add(&__parent, __first, __parent_index);

        random_add(&__left, __first, __left_index);
        if (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__left))) {
            is_heap = false;
            break;
        }

        random_add(&__right, __first, __right_index);
        if ((__right_index < __distance) &&
            (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__right)))) {
            is_heap = false;
            break;
        }

        ++__parent_index;
        __left_index = (__parent_index + 1) * 2 - 1;
        __right_index = (__parent_index + 1) * 2;
    }

    iter_ops->assign(*until, is_heap ? __last : __parent);
    __c_free(__right);
    __c_free(__left);
    __c_free(__parent);

    __C_ALGO_END_2(first, last);
}

void algo_push_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t top_index = 0;
    ptrdiff_t hole_index = __first->iterator_ops->distance(__first, __last) - 1;
    push_heap(__first, __last, top_index, hole_index, comp);

    __C_ALGO_END_2(first, last)
}

void algo_pop_heap_by(c_iterator_t* __c_random_iterator first,
                      c_iterator_t* __c_random_iterator last,
                      c_compare comp)
{
    adjust_heap(first, last, comp);
}

void algo_make_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last)
    __C_ALGO_END_2(first, last)
}

void algo_sort_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last)

    while (__first->iterator_ops->distance(__first, __last) > 1) {
        algo_pop_heap_by(__first, __last, comp);
        C_ITER_DEC(__last);
    }
    __C_ALGO_END_2(first, last)
}
