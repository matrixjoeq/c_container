
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

void algo_lower_bound_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator bound,
                         c_compare comp)
{
    if (!first || !last || !value || !bound || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t __count = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __step = 0;
    c_iterator_t* __it = 0;
    C_ITER_COPY(&__it, __first);

    while (__count > 0) {
        C_ITER_ASSIGN(__it, __first);
        __step = __count / 2;
        C_ITER_ADVANCE(__it, __step);
        if (comp(C_ITER_DEREF(__it), value)) {
            C_ITER_INC(__it);
            C_ITER_ASSIGN(__first, __it);
            __count -= __step + 1;
        }
        else
            __count = __step;
    }

    if (*bound == 0)
        C_ITER_COPY(bound, __first);
    else {
        assert(C_ITER_AT_LEAST(*bound, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*bound, __first);
    }

    __c_free(__it);

    __C_ALGO_END_2(first, last)
}

void algo_upper_bound_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator bound,
                         c_compare comp)
{
    if (!first || !last || !value || !bound || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)

    ptrdiff_t __count = C_ITER_DISTANCE(__first, __last);
    ptrdiff_t __step = 0;
    c_iterator_t* __it = 0;
    C_ITER_COPY(&__it, __first);
    while (__count > 0) {
        C_ITER_ASSIGN(__it, __first);
        __step = __count / 2;
        C_ITER_ADVANCE(__it, __step);
        if (!comp(value, C_ITER_DEREF(__it))) {
            C_ITER_INC(__it);
            C_ITER_ASSIGN(__first, __it);
            __count -= __step + 1;
        }
        else
            __count = __step;
    }

    if (*bound == 0)
        C_ITER_COPY(bound, __first);
    else {
        assert(C_ITER_AT_LEAST(*bound, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*bound, __first);
    }

    __c_free(__it);

    __C_ALGO_END_2(first, last)
}

bool algo_binary_search_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_ref_t value,
                           c_compare comp)
{
    if (!first || !last || !value || !comp) return false;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    bool is_found = false;

    __C_ALGO_BEGIN_2(first, last)
    algo_lower_bound_by(__first, __last, value, &__first, comp);
    is_found = (C_ITER_NE(__first, __last) && !(comp(value, C_ITER_DEREF(__first))));
    __C_ALGO_END_2(first, last)

    return is_found;
}

void algo_equal_range_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator lower_bound,
                         c_iterator_t** __c_forward_iterator upper_bound,
                         c_compare comp)
{
    if (!first || !last || !value || !lower_bound || !upper_bound || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    algo_lower_bound_by(__first, __last, value, lower_bound, comp);
    algo_upper_bound_by(__first, __last, value, upper_bound, comp);
    __C_ALGO_END_2(first, last)
}
