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

#include <stdlib.h>
#include <assert.h>
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_pair_size(void)
{
    return sizeof(c_pair_t);
}

// TODO: solve type info pass problem by adding creation args
// constructor:
// void (*create)(c_ref_t obj, int argc, c_ref_t argv[])
// if argc == 0 || argv == 0, act as default constructor
// else act as constructor with args? Maybe...
__c_static __c_inline void c_pair_create(c_ref_t pair)
{
    c_pair_t* _pair = (c_pair_t*)pair;

    assert(_pair->first == 0);
    assert(_pair->second == 0);
    assert(_pair->first_type);
    assert(_pair->second_type);

    _pair->first = __c_allocate(_pair->first_type);
    _pair->second = __c_allocate(_pair->second_type);

    if (!_pair->first || !_pair->second) {
        __c_deallocate(_pair->first_type, _pair->first);
        __c_deallocate(_pair->second_type, _pair->second);
        return;
    }

    _pair->first_type->create(_pair->first);
    _pair->second_type->create(_pair->second);
}

__c_static __c_inline void c_pair_copy(c_ref_t dst, const c_ref_t src)
{
    c_pair_t* _dst = (c_pair_t*)dst;
    c_pair_t* _src = (c_pair_t*)src;

    _dst->first_type = _src->first_type;
    _dst->second_type = _src->second_type;

    assert(_dst->first == 0);
    assert(_dst->second == 0);
    assert(_dst->first_type);
    assert(_dst->second_type);

    _dst->first = __c_allocate(_dst->first_type);
    _dst->second = __c_allocate(_dst->second_type);

    if (!_dst->first || !_dst->second) {
        __c_deallocate(_dst->first_type, _dst->first);
        __c_deallocate(_dst->second_type, _dst->second);
        return;
    }

    _dst->first_type->copy(_dst->first, _src->first);
    _dst->second_type->copy(_dst->second, _src->second);
}

__c_static __c_inline void c_pair_destroy(c_ref_t pair)
{
    c_pair_t* _pair = (c_pair_t*)pair;

    assert(_pair->first_type);
    _pair->first_type->destroy(_pair->first);
    __c_deallocate(_pair->first_type, _pair->first);

    assert(_pair->second_type);
    _pair->second_type->destroy(_pair->second);
    __c_deallocate(_pair->second_type, _pair->second);
}

__c_static __c_inline c_ref_t c_pair_assign(c_ref_t dst, const c_ref_t src)
{
    c_pair_t* _dst = (c_pair_t*)dst;
    c_pair_t* _src = (c_pair_t*)src;

    assert(_dst->first_type);
    _dst->first_type->assign(_dst->first, _src->first);
    assert(_dst->second_type);
    _dst->second_type->assign(_dst->second, _src->second);
    return dst;
}

__c_static __c_inline bool c_pair_less(const c_ref_t x, const c_ref_t y)
{
    c_pair_t* _x = (c_pair_t*)x;
    c_pair_t* _y = (c_pair_t*)y;
    assert(_x->first_type);
    assert(_x->second_type);
    return ((_x->first_type->less(_x->first, _y->first)) ||
            (!_x->first_type->less(_y->first, _x->first) && _x->second_type->less(_x->second, _y->second)));
}

__c_static __c_inline bool c_pair_equal(const c_ref_t x, const c_ref_t y)
{
    c_pair_t* _x = (c_pair_t*)x;
    c_pair_t* _y = (c_pair_t*)y;
    assert(_x->first_type);
    assert(_x->second_type);
    return (_x->first_type->equal(_x->first, _y->first) && _x->second_type->equal(_x->second, _y->second));
}

const c_type_info_t* c_get_pair_type_info(void)
{
    static const c_type_info_t type_info = {
        .size = c_pair_size,
        .create = c_pair_create,
        .copy = c_pair_copy,
        .destroy = c_pair_destroy,
        .assign = c_pair_assign,
        .less = c_pair_less,
        .equal = c_pair_equal
    };

    return &type_info;
}

c_pair_t c_make_pair(const c_type_info_t* T1, const c_type_info_t* T2, c_ref_t x, c_ref_t y)
{
    c_pair_t pair = {
        .first_type = T1,
        .second_type = T2,
        .first = x,
        .second = y,
    };
    return pair;
}
