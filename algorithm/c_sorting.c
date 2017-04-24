
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

bool algo_is_sorted_by(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_compare comp)
{
    if (!first || !last || !comp) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __until = 0;
    algo_is_sorted_until_by(__first, __last, &__until, comp);
    bool is_sorted = C_ITER_EQ(__until, __last);
    __c_free(__until);

    __C_ALGO_END_2(first, last)

    return is_sorted;
}

void algo_is_sorted_until_by(c_iterator_t* __c_forward_iterator first,
                             c_iterator_t* __c_forward_iterator last,
                             c_iterator_t** __c_forward_iterator until,
                             c_compare comp)
{
    if (!first || !last || !until || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    if (C_ITER_EQ(first, last)) {
        if (*until == 0)
            C_ITER_COPY(until, last);
        else {
            assert(C_ITER_AT_LEAST(*until, C_ITER_CATE_FORWARD));
            C_ITER_ASSIGN(*until, last);
        }
        return;
    }

    __C_ALGO_BEGIN_2(first, last)

    c_iterator_t* __next = 0;
    C_ITER_COPY(&__next, __first);
    C_ITER_INC(__next);

    bool is_sorted = true;
    while (C_ITER_NE(__next, __last)) {
        if (!comp(C_ITER_DEREF(__first), C_ITER_DEREF(__next))) {
            is_sorted = false;
            break;
        }
        C_ITER_INC(__first);
        C_ITER_INC(__next);
    }

    if (*until == 0)
        C_ITER_COPY(until, is_sorted ? __last : __first);
    else {
        assert(C_ITER_AT_LEAST(*until, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*until, is_sorted ? __last : __first);
    }

    __C_ALGO_END_2(first, last)
}
