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
#include "c_set.h"

/* set */
c_set_t* c_set_create(c_type_info_t* key_type, c_compare key_comp)
{
    return c_tree_create(key_type, key_type, __c_identity, key_comp);
}

void c_set_destroy(c_set_t* set)
{
    c_tree_destroy(set);
}

c_set_iterator_t c_set_begin(c_set_t* set)
{
    return c_tree_begin(set);
}

c_set_iterator_t c_set_rbegin(c_set_t* set)
{
    return c_tree_rbegin(set);
}

c_set_iterator_t c_set_end(c_set_t* set)
{
    return c_tree_end(set);
}

c_set_iterator_t c_set_rend(c_set_t* set)
{
    return c_tree_rend(set);
}

bool c_set_empty(c_set_t* set)
{
    return c_tree_empty(set);
}

size_t c_set_size(c_set_t* set)
{
    return c_tree_size(set);
}

size_t c_set_max_size(void)
{
    return c_tree_max_size();
}

void c_set_clear(c_set_t* set)
{
    c_tree_clear(set);
}

c_set_iterator_t c_set_insert_value(c_set_t* set, c_ref_t value)
{
    return c_tree_insert_unique_value(set, value);
}

c_set_iterator_t c_set_insert(c_set_t* set, c_set_iterator_t hint, c_ref_t value)
{
    return c_tree_insert_unique(set, hint, value);
}

void c_set_insert_range(c_set_t* set,
                        c_iterator_t* __c_input_iterator first,
                        c_iterator_t* __c_input_iterator last)
{
    c_tree_insert_unique_range(set, first, last);
}

void c_set_insert_from(c_set_t* set, c_ref_t first_value, c_ref_t last_value)
{
    c_tree_insert_unique_from(set, first_value, last_value);
}

c_set_iterator_t c_set_erase(c_set_t* set, c_set_iterator_t pos)
{
    return c_tree_erase(set, pos);
}

size_t c_set_erase_key(c_set_t* set, c_ref_t key)
{
    return c_tree_erase_key(set, key);
}

void c_set_erase_range(c_set_t* set, c_set_iterator_t first, c_set_iterator_t last)
{
    c_tree_erase_range(set, first, last);
}

void c_set_erase_from(c_set_t* set, c_ref_t first_key, c_ref_t last_key)
{
    c_tree_erase_from(set, first_key, last_key);
}

void c_set_swap(c_set_t* set, c_set_t* other)
{
    c_tree_swap(set, other);
}

c_set_iterator_t c_set_find(c_set_t* set, c_ref_t key)
{
    return c_tree_find(set, key);
}

size_t c_set_count(c_set_t* set, c_ref_t key)
{
    return c_tree_count(set, key);
}

c_set_iterator_t c_set_lower_bound(c_set_t* set, c_ref_t key)
{
    return c_tree_lower_bound(set, key);
}

c_set_iterator_t c_set_upper_bound(c_set_t* set, c_ref_t key)
{
    return c_tree_upper_bound(set, key);
}

void c_set_equal_range(c_set_t* set, c_ref_t key,
                       c_set_iterator_t** lower,
                       c_set_iterator_t** upper)
{
    c_tree_equal_range(set, key, lower, upper);
}

/* multiset */
c_multiset_t* c_multiset_create(c_type_info_t* key_type, c_compare key_comp)
{
    return c_tree_create(key_type, key_type, __c_identity, key_comp);
}

void c_multiset_destroy(c_multiset_t* multiset)
{
    c_tree_destroy(multiset);
}

c_multiset_iterator_t c_multiset_begin(c_multiset_t* multiset)
{
    return c_tree_begin(multiset);
}

c_multiset_iterator_t c_multiset_rbegin(c_multiset_t* multiset)
{
    return c_tree_rbegin(multiset);
}

c_multiset_iterator_t c_multiset_end(c_multiset_t* multiset)
{
    return c_tree_end(multiset);
}

c_multiset_iterator_t c_multiset_rend(c_multiset_t* multiset)
{
    return c_tree_rend(multiset);
}

bool c_multiset_empty(c_multiset_t* multiset)
{
    return c_tree_empty(multiset);
}

size_t c_multiset_size(c_multiset_t* multiset)
{
    return c_tree_size(multiset);
}

size_t c_multiset_max_size(void)
{
    return c_tree_max_size();
}

void c_multiset_clear(c_multiset_t* multiset)
{
    c_tree_clear(multiset);
}

c_multiset_iterator_t c_multiset_insert_value(c_multiset_t* multiset, c_ref_t value)
{
    return c_tree_insert_equal_value(multiset, value);
}

c_multiset_iterator_t c_multiset_insert(c_multiset_t* multiset,
                                        c_multiset_iterator_t hint,
                                        c_ref_t value)
{
    return c_tree_insert_equal(multiset, hint, value);
}

void c_multiset_insert_range(c_multiset_t* multiset,
                             c_iterator_t* __c_input_iterator first,
                             c_iterator_t* __c_input_iterator last)
{
    c_tree_insert_equal_range(multiset, first, last);
}

void c_multiset_insert_from(c_multiset_t* multiset, c_ref_t first_value, c_ref_t last_value)
{
    c_tree_insert_equal_from(multiset, first_value, last_value);
}

c_multiset_iterator_t c_multiset_erase(c_multiset_t* multiset, c_multiset_iterator_t pos)
{
    return c_tree_erase(multiset, pos);
}

size_t c_multiset_erase_key(c_multiset_t* multiset, c_ref_t key)
{
    return c_tree_erase_key(multiset, key);
}

void c_multiset_erase_range(c_multiset_t* multiset,
                            c_multiset_iterator_t first,
                            c_multiset_iterator_t last)
{
    c_tree_erase_range(multiset, first, last);
}

void c_multiset_erase_from(c_multiset_t* multiset, c_ref_t first_key, c_ref_t last_key)
{
    c_tree_erase_from(multiset, first_key, last_key);
}

void c_multiset_swap(c_multiset_t* multiset, c_multiset_t* other)
{
    c_tree_swap(multiset, other);
}

c_multiset_iterator_t c_multiset_find(c_multiset_t* multiset, c_ref_t key)
{
    return c_tree_find(multiset, key);
}

size_t c_multiset_count(c_multiset_t* multiset, c_ref_t key)
{
    return c_tree_count(multiset, key);
}

c_multiset_iterator_t c_multiset_lower_bound(c_multiset_t* multiset, c_ref_t key)
{
    return c_tree_lower_bound(multiset, key);
}

c_multiset_iterator_t c_multiset_upper_bound(c_multiset_t* multiset, c_ref_t key)
{
    return c_tree_upper_bound(multiset, key);
}

void c_multiset_equal_range(c_multiset_t* multiset, c_ref_t key,
                            c_multiset_iterator_t** lower,
                            c_multiset_iterator_t** upper)
{
    c_tree_equal_range(multiset, key, lower, upper);
}
