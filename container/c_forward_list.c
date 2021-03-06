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
#include "c_util.h"
#include "c_forward_list.h"

struct __c_slist_node {
    struct __c_slist_node* next;
    c_ref_t value;
};
typedef struct __c_slist_node c_slist_node_t;

struct __c_slist {
    c_slist_node_t* ancient; // before_begin() of list
    c_slist_node_t* node; // end() of list
    const c_type_info_t* value_type;
};

__c_static __c_inline bool __is_slist_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_FORWARD &&
            iter->iterator_type == C_ITER_TYPE_FORWARD_LIST);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_slist_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_slist_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_slist_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_slist_iterator(dst) && __is_slist_iterator(src) && dst != src) {
        ((c_slist_iterator_t*)dst)->node = ((c_slist_iterator_t*)src)->node;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (__is_slist_iterator(iter)) {
        ((c_slist_iterator_t*)iter)->node = ((c_slist_iterator_t*)iter)->node->next;
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_slist_iterator(iter)) {
        if (*tmp == 0) {
            iter_alloc_and_copy(tmp, iter);
        }
        else {
            assert(__is_slist_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_slist_iterator_t*)iter)->node = ((c_slist_iterator_t*)iter)->node->next;
    }
    return *tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (__is_slist_iterator(iter)) {
        return ((c_slist_iterator_t*)iter)->node->value;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_slist_iterator(x) || !__is_slist_iterator(y)) return false;
    return ((c_slist_iterator_t*)x)->node == ((c_slist_iterator_t*)y)->node;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (__is_slist_iterator(iter)) {
        while (n-- > 0) {
            ((c_slist_iterator_t*)iter)->node = ((c_slist_iterator_t*)iter)->node->next;
        }
    }
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_slist_iterator(first) || !__is_slist_iterator(last)) return 0;

    c_slist_node_t* _first = ((c_slist_iterator_t*)first)->node;
    c_slist_node_t* _last = ((c_slist_iterator_t*)last)->node;
    ptrdiff_t n = 0;
    while (_first != _last) {
        _first = _first->next;
        ++n;
    }

    return n;
}

static c_iterator_operation_t s_iter_ops = {
    .alloc_and_copy = iter_alloc_and_copy,
    .assign = iter_assign,
    .increment = iter_increment,
    .decrement = 0,
    .post_increment = iter_post_increment,
    .post_decrement = 0,
    .dereference = iter_dereference,
    .equal = iter_equal,
    .not_equal = iter_not_equal,
    .less = 0,
    .advance = iter_advance,
    .distance = iter_distance
};

__c_static __c_inline c_slist_iterator_t __create_iterator(const c_type_info_t* value_type, c_slist_node_t* node)
{
    assert(value_type);
    assert(node);

    c_slist_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_FORWARD,
            .iterator_type = C_ITER_TYPE_FORWARD_LIST,
            .iterator_ops = &s_iter_ops,
            .value_type = value_type
        },
        .node = node
    };
    return iter;
}

__c_static __c_inline c_slist_node_t* __before_begin(c_slist_t* list)
{
    assert(list);
    return list->ancient;
}

__c_static __c_inline c_slist_node_t* __begin(c_slist_t* list)
{
    assert(list);
    return list->ancient->next;
}

__c_static __c_inline c_slist_node_t* __end(c_slist_t* list)
{
    assert(list);
    return list->node;
}

__c_static __c_inline c_slist_node_t* __create_node(c_slist_t* list, c_ref_t value)
{
    assert(list);

    c_slist_node_t* node = (c_slist_node_t*)malloc(sizeof(c_slist_node_t));
    if (!node) return 0;

    node->next = 0;
    node->value = __c_allocate(list->value_type);

    if (!node->value) {
        __c_free(node);
        return 0;
    }

    if (value) {
        list->value_type->copy(node->value, value);
    }
    else {
        list->value_type->create(node->value);
    }

    return node;
}

__c_static __c_inline c_slist_node_t* __pop_node_after(c_slist_t* list, c_slist_node_t* node)
{
    assert(list);

    if (!node || node == list->node) return list->node;

    c_slist_node_t* pos = node->next;
    if (!pos || pos == list->node || pos == list->ancient) return list->node;

    node->next = pos->next;
    list->value_type->destroy(pos->value);
    __c_deallocate(list->value_type, pos->value);
    __c_free(pos);

    return node->next;
}

// move (first, last) after pos
__c_static __c_inline void __transfer(c_slist_node_t* pos, c_slist_node_t* first, c_slist_node_t* last)
{
    if (pos != first && first != last && first->next != last) {
        c_slist_node_t* last_prev = first->next;
        while (last_prev->next != last) {
            last_prev = last_prev->next;
        }

        last_prev->next = pos->next;
        pos->next = first->next;
        first->next = last;
    }
}

/**
 * constructor/destructor
 */
c_slist_t* c_slist_create(const c_type_info_t* value_type)
{
    if (!value_type) return 0;
    validate_type_info(value_type);

    c_slist_t* list = (c_slist_t*)malloc(sizeof(c_slist_t));
    if (!list) return 0;

    list->node = (c_slist_node_t*)malloc(sizeof(c_slist_node_t));
    if (!list->node) {
        __c_free(list);
        return 0;
    }

    list->ancient = (c_slist_node_t*)malloc(sizeof(c_slist_node_t));
    if (!list->ancient) {
        __c_free(list->node);
        __c_free(list);
        return 0;
    }

    list->value_type = value_type;
    list->ancient->next = list->node;
    list->ancient->value = 0;
    list->node->next = 0;
    list->node->value = 0;

    return list;
}

c_slist_t* c_slist_create_from(const c_type_info_t* value_type, c_ref_t values, size_t length)
{
    if (!value_type || !values || length == 0) return 0;

    c_slist_t* list = c_slist_create(value_type);
    if (!list) return 0;

    c_slist_iterator_t iter = c_slist_before_begin(list);
    c_ref_t value = values;
    for (size_t n = 0; n < length; ++n) {
        iter = c_slist_insert_after(list, iter, value);
        value += value_type->size();
    }

    return list;
}

c_slist_t* c_slist_copy(c_slist_t* other)
{
    if (!other) return 0;

    c_slist_t* list = c_slist_create(other->value_type);
    if (!list) return 0;

    c_slist_iterator_t iter = c_slist_before_begin(list);
    for (c_slist_node_t* node = __begin(other); node != __end(other); node = node->next) {
        iter = c_slist_insert_after(list, iter, node->value);
    }

    return list;
}

c_slist_t* c_slist_assign(c_slist_t* self, c_slist_t* other)
{
    if (!self || !other) return self;

    if (self != other) {
        c_slist_clear(self);
        self->value_type = other->value_type;
        c_slist_iterator_t iter = c_slist_before_begin(self);
        for (c_slist_node_t* node = __begin(other); node != __end(other); node = node->next) {
            iter = c_slist_insert_after(self, iter, node->value);
        }
    }

    return self;
}

void c_slist_destroy(c_slist_t* list)
{
    if (!list) return;

    c_slist_clear(list);
    __c_free(list->ancient);
    __c_free(list->node);
    __c_free(list);
}

/**
 * element access
 */
c_ref_t c_slist_front(c_slist_t* list)
{
    return c_slist_empty(list) ? 0 : __begin(list)->value;
}

/**
 * iterators
 */
c_slist_iterator_t c_slist_before_begin(c_slist_t* list)
{
    assert(list);
    return __create_iterator(list->value_type, __before_begin(list));
}

c_slist_iterator_t c_slist_begin(c_slist_t* list)
{
    assert(list);
    return __create_iterator(list->value_type, __begin(list));
}

c_slist_iterator_t c_slist_end(c_slist_t* list)
{
    assert(list);
    return __create_iterator(list->value_type, __end(list));
}

/**
 * capacity
 */
bool c_slist_empty(c_slist_t* list)
{
    return list ? __begin(list) == __end(list) : true;
}

size_t c_slist_max_size(void)
{
    return (-1);
}

/**
 * modifiers
 */
void c_slist_clear(c_slist_t* list)
{
    if (c_slist_empty(list)) return;

    c_slist_node_t* node = __before_begin(list);
    c_slist_node_t* next = __begin(list);
    c_slist_node_t* last = __end(list);
    while (next != last) {
        next = __pop_node_after(list, node);
    }
}

c_slist_iterator_t c_slist_insert_after(c_slist_t* list, c_slist_iterator_t pos, c_ref_t value)
{
    assert(list);
    assert(value);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_FORWARD_LIST);

    c_slist_node_t* node = __create_node(list, value);
    if (!node) return pos;

    node->next = pos.node->next;
    pos.node->next = node;

    return __create_iterator(list->value_type, node);
}

c_slist_iterator_t c_slist_erase_after(c_slist_t* list, c_slist_iterator_t pos)
{
    assert(list);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_FORWARD_LIST);
    c_slist_node_t* node = __pop_node_after(list, pos.node);
    return __create_iterator(list->value_type, node);
}

c_slist_iterator_t c_slist_erase_after_range(c_slist_t* list, c_slist_iterator_t first, c_slist_iterator_t last)
{
    c_slist_iterator_t pos = first;
    C_ITER_INC(&pos);
    while (pos.node != last.node) {
        pos = c_slist_erase_after(list, first);
    }

    return last;
}

void c_slist_push_front(c_slist_t* list, c_ref_t value)
{
    if (!list || !value) return;

    c_slist_node_t* node = __create_node(list, value);
    if (!node) return;

    node->next = list->ancient->next;
    list->ancient->next = node;
}

void c_slist_pop_front(c_slist_t* list)
{
    if (!c_slist_empty(list)) __pop_node_after(list, list->ancient);
}

void c_slist_resize(c_slist_t* list, size_t count)
{
    c_slist_resize_with_value(list, count, 0);
}

void c_slist_resize_with_value(c_slist_t* list, size_t count, c_ref_t value)
{
    if (!list) return;

    c_slist_node_t* prev = list->ancient;
    c_slist_node_t* node = prev->next;
    while (node != list->node && count > 0) {
        prev = node;
        node = node->next;
        --count;
    }

    if (count > 0) {
        do {
            c_slist_node_t* node = __create_node(list, value);
            if (!node) return;

            prev->next = node;
            node->next = list->node;
            prev = node;
        } while (--count > 0);
    }
    else {
        while (node != list->node)
            node = __pop_node_after(list, prev);
    }
}

void c_slist_swap(c_slist_t* list, c_slist_t* other)
{
    if (!list || !other) return;

    c_slist_t tmp = *list;
    *list = *other;
    *other = tmp;
}

/**
 * operations
 */

void c_slist_merge(c_slist_t* list, c_slist_t* other)
{
    if (!list) return;
    c_slist_merge_by(list, other, list->value_type->less);
}

void c_slist_merge_by(c_slist_t* list, c_slist_t* other, c_compare comp)
{
    if (!list || c_slist_empty(other) || !comp || !__c_is_same(list->value_type, other->value_type)) return;

    c_slist_node_t* node = __before_begin(list);
    c_slist_node_t* last = __end(list);
    c_slist_node_t* other_node = __before_begin(other);
    c_slist_node_t* other_last = __end(other);
    while (node->next != last && other_node->next != other_last) {
        if (comp(other_node->next->value, node->next->value)) {
            __transfer(node, other_node, other_node->next->next);
        }
        else {
            node = node->next;
        }
    }

    if (other_node->next != other_last) {
        __transfer(node, other_node, other_last);
    }
}

void c_slist_splice_after(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other)
{
    if (!list || c_slist_empty(other) || !__c_is_same(list->value_type, other->value_type)) return;

    __transfer(pos.node, __before_begin(other), __end(other));
}

void c_slist_splice_after_from(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t from)
{
    if (!list || c_slist_empty(other) || !__c_is_same(list->value_type, other->value_type)) return;

    __transfer(pos.node, from.node, __end(other));
}

void c_slist_splice_after_range(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t first, c_slist_iterator_t last)
{
    if (!list || c_slist_empty(other) || !__c_is_same(list->value_type, other->value_type)) return;

    __transfer(pos.node, first.node, last.node);
}

void c_slist_remove(c_slist_t* list, c_ref_t value)
{
    if (c_slist_empty(list) || !value) return;

    c_slist_node_t* prev = __before_begin(list);
    c_slist_node_t* node = prev->next;
    c_slist_node_t* last = __end(list);
    while (node != last && prev != last) {
        if (list->value_type->equal(node->value, value)) {
            node = __pop_node_after(list, prev);
        }
        else {
            prev = prev->next;
            node = prev->next;
        }
    }
}

void c_slist_remove_if(c_slist_t* list, c_unary_predicate pred)
{
    if (c_slist_empty(list) || !pred) return;

    c_slist_node_t* prev = __before_begin(list);
    c_slist_node_t* node = prev->next;
    c_slist_node_t* last = __end(list);
    while (node != last && prev != last) {
        if (pred(node->value)) {
            node = __pop_node_after(list, prev);
        }
        else {
            prev = prev->next;
            node = prev->next;
        }
    }
}

void c_slist_sort(c_slist_t* list)
{
    if (!list) return;
    c_slist_sort_by(list, list->value_type->less);
}

void c_slist_sort_by(c_slist_t* list, c_compare comp)
{
    if (c_slist_empty(list) || __begin(list)->next == __end(list) || !comp) return;

    c_slist_t* carry = c_slist_create(list->value_type);
    if (!carry) return;

    c_slist_t* counter[64] = { 0 };
    __array_foreach(counter, i) {
        counter[i] = c_slist_create(list->value_type);
        if (!counter[i]) goto out;
    }

    int fill = 0;
    while (!c_slist_empty(list)) {
        __transfer(__before_begin(carry), __before_begin(list), __begin(list)->next);
        int i = 0;
        while (i < fill && !c_slist_empty(counter[i])) {
            c_slist_merge_by(counter[i], carry, comp);
            c_slist_swap(carry, counter[i++]);
        }
        c_slist_swap(carry, counter[i]);
        if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; ++i) {
        c_slist_merge_by(counter[i], counter[i - 1], comp);
    }
    c_slist_swap(list, counter[fill - 1]);

out:
    __array_foreach(counter, i) {
        c_slist_destroy(counter[i]);
    }
    c_slist_destroy(carry);
}

void c_slist_reverse(c_slist_t* list)
{
    if (c_slist_empty(list) || __begin(list)->next == __end(list)) return;

    c_slist_node_t* node = __begin(list);
    c_slist_node_t* last = __end(list);
    c_slist_node_t* next = node->next;
    while (node != __end(list)) {
        next = node->next;
        list->ancient->next = node;
        node->next = last;
        last = node;
        node = next;
    }
}

void c_slist_unique(c_slist_t* list)
{
    if (!list) return;
    c_slist_unique_if(list, list->value_type->equal);
}

void c_slist_unique_if(c_slist_t* list, c_binary_predicate pred)
{
    if (c_slist_empty(list) || __begin(list)->next == __end(list) || !pred) return;

    c_slist_node_t* last = __end(list);
    c_slist_node_t* x = __begin(list);
    c_slist_node_t* y = 0;
    while (x != last && x->next != last) {
        y = x->next;
        while (y != last && pred(x->value, y->value)) {
            y = __pop_node_after(list, x);
        }
        x = x->next;
    }
}
