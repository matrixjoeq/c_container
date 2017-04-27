
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

__c_static __c_inline ptrdiff_t parent(ptrdiff_t index)
{
    return (index - 1) / 2;
}

__c_static __c_inline ptrdiff_t left(ptrdiff_t index)
{
    return (index + 1) * 2 - 1;
}

__c_static __c_inline ptrdiff_t right(ptrdiff_t index)
{
    return (index + 1) * 2;
}

__c_static void push_heap(c_iterator_t* first, ptrdiff_t top_index, ptrdiff_t hole_index, c_ref_t value, c_compare comp)
{
    __C_ALGO_BEGIN_1(first)

    ptrdiff_t parent_index = parent(hole_index);
    c_iterator_t* __parent = 0;
    c_iterator_t* __hole = 0;

    while (hole_index > top_index) {
        __c_iter_move_copy(&__parent, __first, parent_index);
        __c_iter_move_copy(&__hole, __first, hole_index);
        if (comp(C_ITER_DEREF(__parent), value)) {
            C_ITER_DEREF_ASSIGN(__hole, __parent);
            hole_index = parent_index;
            parent_index = parent(hole_index);
        }
        else
            break;
    }
    __c_iter_move_copy(&__hole, __first, hole_index);
    C_ITER_DEREF_ASSIGN_V(__hole, value);

    __c_free(__hole);
    __c_free(__parent);

    __C_ALGO_END_1(first);
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

    bool __is_heap = true;
    ptrdiff_t __distance = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __parent_index = 0;
    ptrdiff_t __left_index = left(__parent_index);
    ptrdiff_t __right_index = right(__parent_index);
    c_iterator_t* __parent = 0;
    c_iterator_t* __left = 0;
    c_iterator_t* __right = 0;
    while (__left_index < __distance) {
        __c_iter_move_copy(&__parent, __first, __parent_index);

        __c_iter_move_copy(&__left, __first, __left_index);
        if (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__left))) {
            __is_heap = false;
            break;
        }

        __c_iter_move_copy(&__right, __first, __right_index);
        if ((__right_index < __distance) &&
            (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__right)))) {
            __is_heap = false;
            break;
        }

        ++__parent_index;
        __left_index = left(__parent_index);
        __right_index = right(__parent_index);
    }

    if (*until == 0)
        C_ITER_COPY(until, __is_heap ? __last : __parent);
    else {
        assert(C_ITER_EXACT(*until, C_ITER_CATE_RANDOM));
        C_ITER_ASSIGN(*until, __is_heap ? __last : __parent);
    }

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

    if (C_ITER_DISTANCE(first, last) < 2) return;

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t top_index = 0;
    ptrdiff_t hole_index = C_ITER_DISTANCE(__first, __last) - 1;
    C_ITER_DEC(__last);
    push_heap(__first, top_index, hole_index, C_ITER_DEREF(__last), comp);

    __C_ALGO_END_2(first, last)
}

void algo_pop_heap_by(c_iterator_t* __c_random_iterator first,
                      c_iterator_t* __c_random_iterator last,
                      c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
    adjust_heap(first, last, comp);
}

void algo_make_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    if (C_ITER_DISTANCE(first, last) <= 1) return;

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __heap_last = 0;
    __c_iter_move_copy(&__heap_last, __first, 2);

    do {
        algo_push_heap_by(__first, __heap_last, comp);
    } while (C_ITER_NE(__heap_last, __last) && C_ITER_INC(__heap_last));

    __c_free(__heap_last);

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

    while (C_ITER_DISTANCE(__first, __last) > 1) {
        algo_pop_heap_by(__first, __last, comp);
        C_ITER_DEC(__last);
    }

    __C_ALGO_END_2(first, last)
}
