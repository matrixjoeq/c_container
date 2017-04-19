
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

void algo_swap(c_containable_t* type_info, c_ref_t x, c_ref_t y)
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

void algo_iter_swap(c_iterator_t* x, c_iterator_t* y)
{
    if (!x || !y) return;
    assert(C_ITER_AT_LEAST(x, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(y, C_ITER_CATE_FORWARD));
    algo_swap(x->type_info, C_ITER_DEREF(x), C_ITER_DEREF(y));
}
