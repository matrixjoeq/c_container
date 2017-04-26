
#include <assert.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"

void algo_max_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator max,
                         c_compare comp)
{
    if (!first || !last || !max || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __max = 0;
    C_ITER_COPY(&__max, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__max), C_ITER_DEREF(__first))) {
                C_ITER_ASSIGN(__max, __first);
            }
        }
    }

    if (*max == 0)
        C_ITER_COPY(max, __max);
    else {
        assert(C_ITER_AT_LEAST(*max, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*max, __max);
    }

    __c_free(__max);
    __C_ALGO_END_2(first, last)
}

void algo_min_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator min,
                         c_compare comp)
{
    if (!first || !last || !min || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __min = 0;
    C_ITER_COPY(&__min, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__first), C_ITER_DEREF(__min))) {
                C_ITER_ASSIGN(__min, __first);
            }
        }
    }

    if (*min == 0)
        C_ITER_COPY(min, __min);
    else {
        assert(C_ITER_AT_LEAST(*min, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*min, __min);
    }

    __c_free(__min);
    __C_ALGO_END_2(first, last)
}

void algo_minmax_element_by(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t** __c_forward_iterator min,
                            c_iterator_t** __c_forward_iterator max,
                            c_compare comp)
{
    if (!first || !last || !min || !max || !comp) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_FORWARD));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_FORWARD));

    __C_ALGO_BEGIN_2(first, last)
    c_iterator_t* __min = 0;
    c_iterator_t* __max = 0;
    C_ITER_COPY(&__min, __first);
    C_ITER_COPY(&__max, __first);

    while (C_ITER_NE(__first, __last)) {
        C_ITER_INC(__first);
        if (C_ITER_NE(__first, __last)) {
            if (comp(C_ITER_DEREF(__first), C_ITER_DEREF(__min))) {
                C_ITER_ASSIGN(__min, __first);
            }

            if (comp(C_ITER_DEREF(__max), C_ITER_DEREF(__first))) {
                C_ITER_ASSIGN(__max, __first);
            }
        }
    }

    if (*min == 0)
        C_ITER_COPY(min, __min);
    else {
        assert(C_ITER_AT_LEAST(*min, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*min, __min);
    }

    if (*max == 0)
        C_ITER_COPY(max, __max);
    else {
        assert(C_ITER_AT_LEAST(*max, C_ITER_CATE_FORWARD));
        C_ITER_ASSIGN(*max, __max);
    }

    __c_free(__min);
    __c_free(__max);
    __C_ALGO_END_2(first, last)
}