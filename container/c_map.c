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

__c_static __c_inline c_ref_t __key_of_pair(c_ref_t pair)
{
    return __c_select1st((c_pair_t*)pair);
}

/* map */
c_map_t* c_map_create(const c_type_info_t* key_type, const c_type_info_t* value_type, c_compare key_comp)
{
    c_map_t* map = c_tree_create(key_type, c_get_pair_type_info(), value_type, __key_of_pair, key_comp);
    return map;
}

void c_map_destroy(c_map_t* map)
{
    c_tree_destroy(map);
}

c_map_iterator_t c_map_begin(c_map_t* map)
{
    return c_tree_begin(map);
}

c_map_iterator_t c_map_rbegin(c_map_t* map)
{
    return c_tree_rbegin(map);
}

c_map_iterator_t c_map_end(c_map_t* map)
{
    return c_tree_end(map);
}

c_map_iterator_t c_map_rend(c_map_t* map)
{
    return c_tree_rend(map);
}

bool c_map_empty(c_map_t* map)
{
    return c_tree_empty(map);
}

size_t c_map_size(c_map_t* map)
{
    return c_tree_size(map);
}

size_t c_map_max_size(void)
{
    return c_tree_max_size();
}

void c_map_clear(c_map_t* map)
{
    c_tree_clear(map);
}

c_map_iterator_t c_map_insert_value(c_map_t* map, c_ref_t value)
{
    return c_tree_insert_unique_value(map, value);
}

c_map_iterator_t c_map_insert(c_map_t* map, c_map_iterator_t hint, c_ref_t value)
{
    return c_tree_insert_unique(map, hint, value);
}

void c_map_insert_range(c_map_t* map,
                        c_iterator_t* __c_input_iterator first,
                        c_iterator_t* __c_input_iterator last)
{
    c_tree_insert_unique_range(map, first, last);
}

void c_map_insert_from(c_map_t* map, c_ref_t first_value, c_ref_t last_value)
{
    c_tree_insert_unique_from(map, first_value, last_value);
}

c_map_iterator_t c_map_erase(c_map_t* map, c_map_iterator_t pos)
{
    return c_tree_erase(map, pos);
}

size_t c_map_erase_key(c_map_t* map, c_ref_t key)
{
    return c_tree_erase_key(map, key);
}

void c_map_erase_range(c_map_t* map, c_map_iterator_t first, c_map_iterator_t last)
{
    c_tree_erase_range(map, first, last);
}

void c_map_erase_from(c_map_t* map, c_ref_t first_key, c_ref_t last_key)
{
    c_tree_erase_from(map, first_key, last_key);
}

void c_map_swap(c_map_t* map, c_map_t* other)
{
    c_tree_swap(map, other);
}

c_map_iterator_t c_map_find(c_map_t* map, c_ref_t key)
{
    return c_tree_find(map, key);
}

size_t c_map_count(c_map_t* map, c_ref_t key)
{
    return c_tree_count(map, key);
}

c_map_iterator_t c_map_lower_bound(c_map_t* map, c_ref_t key)
{
    return c_tree_lower_bound(map, key);
}

c_map_iterator_t c_map_upper_bound(c_map_t* map, c_ref_t key)
{
    return c_tree_upper_bound(map, key);
}

void c_map_equal_range(c_map_t* map, c_ref_t key,
                       c_map_iterator_t** lower,
                       c_map_iterator_t** upper)
{
    c_tree_equal_range(map, key, lower, upper);
}

/* multimap */
c_multimap_t* c_multimap_create(const c_type_info_t* key_type, const c_type_info_t* value_type, c_compare key_comp)
{
    return c_tree_create(key_type, c_get_pair_type_info(), value_type, __key_of_pair, key_comp);
}

void c_multimap_destroy(c_multimap_t* multimap)
{
    c_tree_destroy(multimap);
}

c_multimap_iterator_t c_multimap_begin(c_multimap_t* multimap)
{
    return c_tree_begin(multimap);
}

c_multimap_iterator_t c_multimap_rbegin(c_multimap_t* multimap)
{
    return c_tree_rbegin(multimap);
}

c_multimap_iterator_t c_multimap_end(c_multimap_t* multimap)
{
    return c_tree_end(multimap);
}

c_multimap_iterator_t c_multimap_rend(c_multimap_t* multimap)
{
    return c_tree_rend(multimap);
}

bool c_multimap_empty(c_multimap_t* multimap)
{
    return c_tree_empty(multimap);
}

size_t c_multimap_size(c_multimap_t* multimap)
{
    return c_tree_size(multimap);
}

size_t c_multimap_max_size(void)
{
    return c_tree_max_size();
}

void c_multimap_clear(c_multimap_t* multimap)
{
    c_tree_clear(multimap);
}

c_multimap_iterator_t c_multimap_insert_value(c_multimap_t* multimap, c_ref_t value)
{
    return c_tree_insert_equal_value(multimap, value);
}

c_multimap_iterator_t c_multimap_insert(c_multimap_t* multimap,
                                        c_multimap_iterator_t hint,
                                        c_ref_t value)
{
    return c_tree_insert_equal(multimap, hint, value);
}

void c_multimap_insert_range(c_multimap_t* multimap,
                             c_iterator_t* __c_input_iterator first,
                             c_iterator_t* __c_input_iterator last)
{
    c_tree_insert_equal_range(multimap, first, last);
}

void c_multimap_insert_from(c_multimap_t* multimap, c_ref_t first_value, c_ref_t last_value)
{
    c_tree_insert_equal_from(multimap, first_value, last_value);
}

c_multimap_iterator_t c_multimap_erase(c_multimap_t* multimap, c_multimap_iterator_t pos)
{
    return c_tree_erase(multimap, pos);
}

size_t c_multimap_erase_key(c_multimap_t* multimap, c_ref_t key)
{
    return c_tree_erase_key(multimap, key);
}

void c_multimap_erase_range(c_multimap_t* multimap,
                            c_multimap_iterator_t first,
                            c_multimap_iterator_t last)
{
    c_tree_erase_range(multimap, first, last);
}

void c_multimap_erase_from(c_multimap_t* multimap, c_ref_t first_key, c_ref_t last_key)
{
    c_tree_erase_from(multimap, first_key, last_key);
}

void c_multimap_swap(c_multimap_t* multimap, c_multimap_t* other)
{
    c_tree_swap(multimap, other);
}

c_multimap_iterator_t c_multimap_find(c_multimap_t* multimap, c_ref_t key)
{
    return c_tree_find(multimap, key);
}

size_t c_multimap_count(c_multimap_t* multimap, c_ref_t key)
{
    return c_tree_count(multimap, key);
}

c_multimap_iterator_t c_multimap_lower_bound(c_multimap_t* multimap, c_ref_t key)
{
    return c_tree_lower_bound(multimap, key);
}

c_multimap_iterator_t c_multimap_upper_bound(c_multimap_t* multimap, c_ref_t key)
{
    return c_tree_upper_bound(multimap, key);
}

void c_multimap_equal_range(c_multimap_t* multimap, c_ref_t key,
                            c_multimap_iterator_t** lower,
                            c_multimap_iterator_t** upper)
{
    c_tree_equal_range(multimap, key, lower, upper);
}
