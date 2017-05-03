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
#include <string.h>
#include "c_internal.h"
#include "c_map.h"

struct __c_map {
    c_tree_t* repr;
    c_type_info_t* pair_info;
};

__c_static __c_inline c_ref_t __key_of_pair(c_ref_t pair)
{
    return __c_select1st((c_pair_t*)pair);
}

__c_static __c_inline size_t c_pair_size(void)
{
    return sizeof(c_pair_t);
}

// TODO: solve type info pass problem by adding creation args
// constructor:
// void (*create)(c_ref_t obj, int argc, c_ref_t argv[])
// if argc == 0 || argv == 0, act as default constructor
// else act as constructor with args
__c_static __c_inline void c_pair_create(c_ref_t pair)
{
    c_pair_t* _pair = (c_pair_t*)pair;

    // TODO: make allocation in type's creator
    assert(_pair->first == 0);
    assert(_pair->second == 0);
    _pair->first = malloc(_pair->first_type->size());
    _pair->second = malloc(_pair->second_type->size());

    if (!_pair->first || !_pair->second) {
        __c_free(_pair->first);
        __c_free(_pair->second);
        return;
    }

    _pair->first_type->create(_pair->first);
    _pair->second_type->create(_pair->second);
}

__c_static __c_inline void c_pair_copy(c_ref_t dst, const c_ref_t src)
{
    c_pair_t* _dst = (c_pair_t*)dst;
    c_pair_t* _src = (c_pair_t*)src;

    // TODO: make allocation in type's creator
    assert(_dst->first == 0);
    assert(_dst->second == 0);
    _dst->first = malloc(_dst->first_type->size());
    _dst->second = malloc(_dst->second_type->size());

    if (!_dst->first || !_dst->second) {
        __c_free(_dst->first);
        __c_free(_dst->second);
        return;
    }

    _dst->first_type->copy(_dst->first, _src->first);
    _dst->second_type->copy(_dst->second, _src->second);
}

__c_static __c_inline void c_pair_destroy(c_ref_t pair)
{
    c_pair_t* _pair = (c_pair_t*)pair;

    // TODO: make deallocation in type's destructor
    _pair->first_type->destroy(_pair->first);
    __c_free(_pair->first);

    _pair->second_type->destroy(_pair->second);
    __c_free(_pair->second);
}

__c_static __c_inline c_ref_t c_pair_assign(c_ref_t dst, const c_ref_t src)
{
    c_pair_t* _dst = (c_pair_t*)dst;
    c_pair_t* _src = (c_pair_t*)src;

    _dst->first_type->assign(_dst->first, _src->first);
    _dst->second_type->assign(_dst->second, _src->second);
    return dst;
}

__c_static __c_inline bool c_pair_less(const c_ref_t x, const c_ref_t y)
{
    c_pair_t* _x = (c_pair_t*)x;
    c_pair_t* _y = (c_pair_t*)y;
    return ((_x->first_type->less(_x->first, _y->first)) ||
            (!_x->first_type->less(_y->first, _x->first) && _x->second_type->less(_x->second, _y->second)));
}

__c_static __c_inline bool c_pair_equal(const c_ref_t x, const c_ref_t y)
{
    c_pair_t* _x = (c_pair_t*)x;
    c_pair_t* _y = (c_pair_t*)y;
    return (_x->first_type->equal(_x->first, _y->first) && _x->second_type->equal(_x->second, _y->second));
}

/* map */
c_map_t* c_map_create(c_type_info_t* key_type, c_type_info_t* value_type, c_compare key_comp)
{
    c_map_t* map = (c_map_t*)malloc(sizeof(c_map_t));
    if (!map) return 0;
    __c_unuse(value_type);
    map->repr = c_tree_create(key_type, map->pair_info, __key_of_pair, key_comp);
    if (!map->repr) {
        __c_free(map->pair_info);
        __c_free(map);
        return 0;
    }

    return map;
}

void c_map_destroy(c_map_t* map)
{
    c_tree_destroy(map->repr);
    __c_free(map->pair_info);
    __c_free(map);
}

c_map_iterator_t c_map_begin(c_map_t* map)
{
    return c_tree_begin(map->repr);
}

c_map_iterator_t c_map_rbegin(c_map_t* map)
{
    return c_tree_rbegin(map->repr);
}

c_map_iterator_t c_map_end(c_map_t* map)
{
    return c_tree_end(map->repr);
}

c_map_iterator_t c_map_rend(c_map_t* map)
{
    return c_tree_rend(map->repr);
}

bool c_map_empty(c_map_t* map)
{
    return c_tree_empty(map->repr);
}

size_t c_map_size(c_map_t* map)
{
    return c_tree_size(map->repr);
}

size_t c_map_max_size(void)
{
    return c_tree_max_size();
}

void c_map_clear(c_map_t* map)
{
    c_tree_clear(map->repr);
}
