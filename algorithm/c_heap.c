
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

__c_static void adjust_heap(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    __c_unuse(first);
    __c_unuse(last);
    __c_unuse(comp);
}

bool c_algo_is_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN
    __C_ALGO_END
}

void c_algo_is_heap_until_by(c_iterator_t* first, c_iterator_t* last, c_iterator_t** until, c_compare comp)
{
    if (!first || !last || !until || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN
    __C_ALGO_END
}

void c_algo_push_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN
    ptrdiff_t top_index = 1;
    ptrdiff_t hole_index = __first->distance(__first, __last);
    __C_ALGO_END
}

void c_algo_pop_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    adjust_heap(first, last, comp);
}

void c_algo_make_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN
    __C_ALGO_END
}

void c_algo_sort_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
    if (!first || !last || !comp) return;
    assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
    assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));

    __C_ALGO_BEGIN
    __C_ALGO_END
}
