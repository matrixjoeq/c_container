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

__c_static __c_inline ptrdiff_t __get_parent(ptrdiff_t index)
{
    return (index - 1) >> 1;    /* (index - 1) / 2 */
}

__c_static __c_inline ptrdiff_t __get_left(ptrdiff_t index)
{
    return (index << 1) + 1;    /* (index + 1) * 2 - 1 */
}

__c_static __c_inline ptrdiff_t __get_right(ptrdiff_t index)
{
    return (index + 1) << 1;    /* (index + 1) * 2 */
}

__c_static void __percolate_down(c_iterator_t* __c_random_iterator first,
                                 ptrdiff_t length,
                                 ptrdiff_t hole_index,
                                 c_compare comp)
{
    __C_ALGO_BEGIN_1(first)

    ptrdiff_t left_index = 0;
    ptrdiff_t right_index = 0;
    ptrdiff_t swap_index = 0;

    c_iterator_t* __hole = 0;
    c_iterator_t* __swap = 0;
    c_iterator_t* __left = 0;
    c_iterator_t* __right = 0;

    while (true) {
        swap_index = hole_index;
        __c_iter_copy_and_move(&__hole, __first, hole_index);
        __c_iter_copy_and_move(&__swap, __first, swap_index);

        left_index = __get_left(hole_index);
        __c_iter_copy_and_move(&__left, __first, left_index);
        if ((left_index < length) && comp(C_ITER_DEREF(__hole), C_ITER_DEREF(__left))) {
            swap_index = left_index;
            __c_iter_copy_and_move(&__swap, __first, swap_index);
        }

        right_index = __get_right(hole_index);
        __c_iter_copy_and_move(&__right, __first, right_index);
        if ((right_index < length) && comp(C_ITER_DEREF(__swap), C_ITER_DEREF(__right))) {
            swap_index = right_index;
            __c_iter_copy_and_move(&__swap, __first, swap_index);
        }

        if (swap_index != hole_index) {
            algo_iter_swap(__hole, __swap);
            hole_index = swap_index;
        }
        else {
            break;
        }
    }

    __c_free(__hole);
    __c_free(__swap);
    __c_free(__left);
    __c_free(__right);

    __C_ALGO_END_1(first)
}

__c_static void __heapify(c_iterator_t* __c_random_iterator first,
                          c_iterator_t* __c_random_iterator last,
                          c_compare comp)
{
    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t length = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t hole_index = __get_parent(length - 1);
    while (hole_index >= 0) {
        __percolate_down(__first, length, hole_index, comp);
        --hole_index;
    }

    __C_ALGO_END_2(first, last)
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
    assert(*until == 0 || C_ITER_EXACT(*until, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN_2(first, last);

    bool __is_heap = true;
    ptrdiff_t __distance = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __parent_index = 0;
    ptrdiff_t __left_index = __get_left(__parent_index);
    ptrdiff_t __right_index = __get_right(__parent_index);
    c_iterator_t* __parent = 0;
    c_iterator_t* __left = 0;
    c_iterator_t* __right = 0;

    while (__left_index < __distance) {
        __c_iter_copy_and_move(&__parent, __first, __parent_index);

        __c_iter_copy_and_move(&__left, __first, __left_index);
        if (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__left))) {
            __is_heap = false;
            break;
        }

        __c_iter_copy_and_move(&__right, __first, __right_index);
        if ((__right_index < __distance) &&
            (comp(C_ITER_DEREF(__parent), C_ITER_DEREF(__right)))) {
            __is_heap = false;
            break;
        }

        ++__parent_index;
        __left_index = __get_left(__parent_index);
        __right_index = __get_right(__parent_index);
    }

    __c_iter_copy_or_assign(until, __is_heap ? __last : __parent);

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

    C_ITER_DEC(__last);
    ptrdiff_t __top_index = 0;
    ptrdiff_t __hole_index = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __parent_index = __get_parent(__hole_index);
    c_iterator_t* __parent = 0;
    c_iterator_t* __hole = 0;
    c_ref_t __value = malloc(first->value_type->size());
    __first->value_type->copy(__value, C_ITER_DEREF(__last));

    while (__hole_index > __top_index) {
        __c_iter_copy_and_move(&__parent, __first, __parent_index);
        __c_iter_copy_and_move(&__hole, __first, __hole_index);
        if (comp(C_ITER_DEREF(__parent), __value)) {
            C_ITER_DEREF_ASSIGN(__hole, __parent);
            __hole_index = __parent_index;
            __parent_index = __get_parent(__hole_index);
        }
        else {
            break;
        }
    }
    __c_iter_copy_and_move(&__hole, __first, __hole_index);
    C_ITER_DEREF_ASSIGN_V(__hole, __value);

    __c_free(__value);
    __c_free(__hole);
    __c_free(__parent);

    __C_ALGO_END_2(first, last)
}

void algo_pop_heap_by(c_iterator_t* __c_random_iterator first,
                      c_iterator_t* __c_random_iterator last,
                      c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    if (C_ITER_DISTANCE(first, last) < 2) return;

    __C_ALGO_BEGIN_2(first, last)

    C_ITER_DEC(__last);
    algo_iter_swap(__first, __last);

    ptrdiff_t __last_index = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __hole_index = 0;
    ptrdiff_t __left_index = __get_left(__hole_index);
    ptrdiff_t __right_index = __get_right(__hole_index);

    c_iterator_t* __hole = 0;
    c_iterator_t* __left = 0;
    c_iterator_t* __right = 0;

    while (__left_index < __last_index) {
        __c_iter_copy_and_move(&__hole, __first, __hole_index);
        __c_iter_copy_and_move(&__left, __first, __left_index);

        if (__right_index < __last_index) {
            __c_iter_copy_and_move(&__right, __first, __right_index);
            if (comp(C_ITER_DEREF(__left), C_ITER_DEREF(__right))) {
                if (comp(C_ITER_DEREF(__hole), C_ITER_DEREF(__right))) {
                    algo_iter_swap(__hole, __right);
                    __hole_index = __right_index;
                }
                else {
                    break;
                }
            }
            else {
                if (comp(C_ITER_DEREF(__hole), C_ITER_DEREF(__left))) {
                    algo_iter_swap(__hole, __left);
                    __hole_index = __left_index;
                }
                else {
                    break;
                }
            }
        }
        else {
            if (comp(C_ITER_DEREF(__hole), C_ITER_DEREF(__left))) {
                algo_iter_swap(__hole, __left);
                __hole_index = __left_index;
            }
            else {
                break;
            }
        }

        __left_index = __get_left(__hole_index);
        __right_index = __get_right(__hole_index);
    }

    __c_free(__right);
    __c_free(__left);
    __c_free(__hole);

    __C_ALGO_END_2(first, last)
}

void algo_make_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    if (C_ITER_DISTANCE(first, last) < 2) return;

    __C_ALGO_BEGIN_2(first, last)

    __heapify(__first, __last, comp);

    __C_ALGO_END_2(first, last)
}

void algo_sort_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    if (C_ITER_DISTANCE(first, last) < 2) return;

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        algo_pop_heap_by(__first, __last, comp);
        C_ITER_DEC(__last);
    }

    __C_ALGO_END_2(first, last)
}
