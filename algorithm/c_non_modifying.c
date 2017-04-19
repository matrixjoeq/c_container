
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

size_t c_algo_count(c_iterator_t* first, c_iterator_t* last, c_ref_t data)
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

size_t c_algo_count_if(c_iterator_t* first, c_iterator_t* last, c_unary_predicate pred)
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

