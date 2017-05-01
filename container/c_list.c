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
#include "c_list.h"

struct __c_list_node {
    struct __c_list_node* prev;
    struct __c_list_node* next;
    c_ref_t value;
};
typedef struct __c_list_node c_list_node_t;

struct __c_list {
    c_list_node_t* node; // __end() of list
    c_containable_t* value_type;
};

struct __c_backend_list {
    c_backend_container_t interface;
    c_list_t* impl;
};

__c_static __c_inline bool __is_list_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_BIDIRECTION &&
            iter->iterator_type == C_ITER_TYPE_LIST);
}

__c_static __c_inline bool __is_list_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_BIDIRECTION &&
            iter->iterator_type == C_ITER_TYPE_LIST_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_list_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_list_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_list_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_list_iterator(dst) && __is_list_iterator(src) && dst != src) {
        ((c_list_iterator_t*)dst)->node = ((c_list_iterator_t*)src)->node;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (__is_list_iterator(iter)) {
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->next;
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (__is_list_iterator(iter)) {
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->prev;
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_list_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_list_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->next;
    }
    return *tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_list_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_list_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->prev;
    }
    return *tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (__is_list_iterator(iter)) {
        return ((c_list_iterator_t*)iter)->node->value;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_list_iterator(x) || !__is_list_iterator(y)) return false;
    return ((c_list_iterator_t*)x)->node == ((c_list_iterator_t*)y)->node;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (__is_list_iterator(iter)) {
        c_list_node_t* node = ((c_list_iterator_t*)iter)->node;
        if (n > 0) {
            while (n--) node = node->next;
        }
        else {
            while (n++) node = node->prev;
        }
        ((c_list_iterator_t*)iter)->node = node;
    }
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_list_iterator(first) || !__is_list_iterator(last)) return 0;

    c_list_node_t* _first = ((c_list_iterator_t*)first)->node;
    c_list_node_t* _last = ((c_list_iterator_t*)last)->node;
    ptrdiff_t n = 0;
    while (_first != _last) {
        _first = _first->next;
        ++n;
    }

    return n;
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_list_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_list_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_list_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_list_reverse_iterator(dst) && __is_list_reverse_iterator(src) && dst != src) {
        ((c_list_iterator_t*)dst)->node = ((c_list_iterator_t*)src)->node;
    }
    return dst;
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (__is_list_reverse_iterator(iter)) {
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->prev;
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (__is_list_reverse_iterator(iter)) {
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->next;
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_list_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_list_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->prev;
    }
    return *tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_list_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_list_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_list_iterator_t*)iter)->node = ((c_list_iterator_t*)iter)->node->next;
    }
    return *tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (__is_list_reverse_iterator(iter)) {
        return ((c_list_iterator_t*)iter)->node->prev->value;
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_list_reverse_iterator(x) || !__is_list_reverse_iterator(y)) return false;
    return ((c_list_iterator_t*)x)->node == ((c_list_iterator_t*)y)->node;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static void reverse_iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (__is_list_reverse_iterator(iter)) {
        c_list_node_t* node = ((c_list_iterator_t*)iter)->node;
        if (n > 0) {
            while (n--) node = node->prev;
        }
        else {
            while (n++) node = node->next;
        }
        ((c_list_iterator_t*)iter)->node = node;
    }
}

__c_static ptrdiff_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_list_reverse_iterator(first) || !__is_list_reverse_iterator(last)) return 0;

    c_list_node_t* _first = ((c_list_iterator_t*)first)->node;
    c_list_node_t* _last = ((c_list_iterator_t*)last)->node;
    ptrdiff_t n = 0;
    while (_first != _last) {
        _first = _first->prev;
        ++n;
    }

    return n;
}

static c_iterator_operation_t s_iter_ops = {
    .alloc_and_copy = iter_alloc_and_copy,
    .assign = iter_assign,
    .increment = iter_increment,
    .decrement = iter_decrement,
    .post_increment = iter_post_increment,
    .post_decrement = iter_post_decrement,
    .dereference = iter_dereference,
    .equal = iter_equal,
    .not_equal = iter_not_equal,
    .advance = iter_advance,
    .distance = iter_distance
};

__c_static __c_inline c_list_iterator_t __create_iterator(
    c_containable_t* value_type, c_list_node_t* node)
{
    assert(value_type);
    assert(node);

    c_list_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_LIST,
            .iterator_ops = &s_iter_ops,
            .value_type = value_type
        },
        .node = node
    };
    return iter;
}

static c_iterator_operation_t s_reverse_iter_ops = {
    .alloc_and_copy = reverse_iter_alloc_and_copy,
    .assign = reverse_iter_assign,
    .increment = reverse_iter_increment,
    .decrement = reverse_iter_decrement,
    .post_increment = reverse_iter_post_increment,
    .post_decrement = reverse_iter_post_decrement,
    .dereference = reverse_iter_dereference,
    .equal = reverse_iter_equal,
    .not_equal = reverse_iter_not_equal,
    .advance = reverse_iter_advance,
    .distance = reverse_iter_distance
};

__c_static __c_inline c_list_iterator_t __create_reverse_iterator(
    c_containable_t* value_type, c_list_node_t* node)
{
    assert(value_type);
    assert(node);

    c_list_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_LIST_REVERSE,
            .iterator_ops = &s_reverse_iter_ops,
            .value_type = value_type
        },
        .node = node
    };
    return iter;
}

__c_static __c_inline c_list_node_t* __begin(c_list_t* list)
{
    assert(list);
    return list->node->next;
}

__c_static __c_inline c_list_node_t* __end(c_list_t* list)
{
    assert(list);
    return list->node;
}

__c_static __c_inline c_list_node_t* __create_node(c_list_t* list, c_ref_t value)
{
    assert(list);

    c_list_node_t* node = (c_list_node_t*)malloc(sizeof(c_list_node_t));
    if (!node) return 0;

    node->prev = 0;
    node->next = 0;

    node->value = malloc(list->value_type->size());
    if (!node->value) {
        __c_free(node);
        return 0;
    }

    if (value)
        list->value_type->copy(node->value, value);
    else
        list->value_type->create(node->value);

    return node;
}

__c_static __c_inline c_list_node_t* __pop_node(c_list_t* list, c_list_node_t* node)
{
    assert(list);
    assert(node);

    if (node == list->node)
        return node;

    c_list_node_t* next_node = node->next;
    c_list_node_t* prev_node = node->prev;

    next_node->prev = prev_node;
    prev_node->next = next_node;
    list->value_type->destroy(node->value);
    __c_free(node->value);
    __c_free(node);

    return next_node;
}

// move [first, last) in front of pos
__c_static __c_inline void __transfer(c_list_node_t* pos, c_list_node_t* first, c_list_node_t* last)
{
    if (pos != last && first != last) {
        pos->prev->next = first;
        first->prev->next = last;
        last->prev->next = pos;

        c_list_node_t* tmp = pos->prev;
        pos->prev = last->prev;
        last->prev = first->prev;
        first->prev = tmp;
    }
}

__c_static void backend_destroy(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_destroy(_c->impl);
    __c_free(_c);
}

__c_static c_ref_t backend_front(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    return c_list_front(_c->impl);
}

__c_static c_ref_t backend_back(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    return c_list_back(_c->impl);
}

__c_static c_iterator_t* backend_begin(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_iterator_t first = c_list_begin(_c->impl);
    if (*iter == 0)
        C_ITER_COPY(iter, &first);
    else
        C_ITER_ASSIGN(*iter, &first);

    return *iter;
}

__c_static c_iterator_t* backend_end(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_iterator_t last = c_list_end(_c->impl);
    if (*iter == 0)
        C_ITER_COPY(iter, &last);
    else
        C_ITER_ASSIGN(*iter, &last);

    return *iter;
}

__c_static bool backend_empty(c_backend_container_t* c)
{
    if (!c) return true;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    return c_list_empty(_c->impl);
}

__c_static size_t backend_size(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    return c_list_size(_c->impl);
}

__c_static size_t backend_max_size(void)
{
    return (-1);
}

__c_static void backend_push_back(c_backend_container_t* c, c_ref_t value)
{
    if (!c || !value) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_push_back(_c->impl, value);
}

__c_static void backend_pop_back(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_pop_back(_c->impl);
}

__c_static void backend_push_front(c_backend_container_t* c, c_ref_t value)
{
    if (!c || !value) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_push_front(_c->impl, value);
}

__c_static void backend_pop_front(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_list_pop_front(_c->impl);
}

__c_static void backend_swap(c_backend_container_t* c, c_backend_container_t* other)
{
    if (!c || !other) return;

    c_backend_list_t* _c = (c_backend_list_t*)c;
    c_backend_list_t* _other = (c_backend_list_t*)other;
    c_backend_container_t tmp = _c->interface;
    c_list_swap(_c->impl, _other->impl);
    _c->interface = _other->interface;
    _other->interface = tmp;
}

/**
 * constructor/destructor
 */
c_list_t* c_list_create(c_containable_t* value_type)
{
    if (!value_type) return 0;

    c_list_t* list = (c_list_t*)malloc(sizeof(c_list_t));
    if (!list) return 0;

    list->node = (c_list_node_t*)malloc(sizeof(c_list_node_t));
    if (!list->node) {
        __c_free(list);
        return 0;
    }

    list->value_type = value_type;
    list->node->prev = list->node;
    list->node->next = list->node;
    list->node->value = 0;

    return list;
}

c_list_t* c_list_create_from(c_containable_t* value_type, c_ref_t values, size_t length)
{
    if (!value_type || !values || length == 0) return 0;

    c_list_t* list = c_list_create(value_type);
    if (!list) return 0;

    c_ref_t value = values;
    for (size_t n = 0; n < length; ++n) {
        c_list_push_back(list, value);
        value += value_type->size();
    }

    return list;
}

c_list_t* c_list_copy(c_list_t* other)
{
    if (!other) return 0;

    c_list_t* list = c_list_create(other->value_type);
    if (!list) return 0;

    for (c_list_node_t* node = __begin(other); node != __end(other); node = node->next)
        c_list_push_back(list, node->value);

    return list;
}

c_list_t* c_list_assign(c_list_t* self, c_list_t* other)
{
    if (!self || !other) return self;

    if (self != other) {
        c_list_clear(self);
        self->value_type = other->value_type;
        for (c_list_node_t* node = __begin(other); node != __end(other); node = node->next)
            c_list_push_back(self, node->value);
    }

    return self;
}

void c_list_destroy(c_list_t* list)
{
    if (!list) return;

    c_list_clear(list);
    __c_free(list->node);
    __c_free(list);
}

/**
 * element access
 */
c_ref_t c_list_front(c_list_t* list)
{
    return c_list_empty(list) ? 0 : __begin(list)->value;
}

c_ref_t c_list_back(c_list_t* list)
{
    return c_list_empty(list) ? 0 : __end(list)->prev->value;
}

/**
 * iterators
 */
c_list_iterator_t c_list_begin(c_list_t* list)
{
    assert(list);
    return __create_iterator(list->value_type, __begin(list));
}

c_list_iterator_t c_list_rbegin(c_list_t* list)
{
    assert(list);
    return __create_reverse_iterator(list->value_type, __end(list));
}

c_list_iterator_t c_list_end(c_list_t* list)
{
    assert(list);
    return __create_iterator(list->value_type, __end(list));
}

c_list_iterator_t c_list_rend(c_list_t* list)
{
    assert(list);
    return __create_reverse_iterator(list->value_type, __begin(list));
}

/**
 * capacity
 */
bool c_list_empty(c_list_t* list)
{
    return list ? __begin(list) == __end(list) : true;
}

size_t c_list_size(c_list_t* list)
{
    if (!list) return 0;
    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    return iter_distance((c_iterator_t*)&first, (c_iterator_t*)&last);
}

size_t c_list_max_size(void)
{
    return (-1);
}

/**
 * modifiers
 */
void c_list_clear(c_list_t* list)
{
    if (c_list_empty(list)) return;

    c_list_node_t* node = __begin(list);
    c_list_node_t* last = __end(list);
    while (node != last) {
        node = __pop_node(list, node);
    }
}

c_list_iterator_t c_list_insert(c_list_t* list, c_list_iterator_t pos, c_ref_t value)
{
    if (!list || !value) return pos;

    c_list_node_t* node = __create_node(list, value);
    if (!node) return pos;

    node->next = pos.node;
    node->prev = pos.node->prev;
    pos.node->prev->next = node;
    pos.node->prev = node;

    return __create_iterator(list->value_type, node);
}

c_list_iterator_t c_list_erase(c_list_t* list, c_list_iterator_t pos)
{
    if (!list) return pos;

    c_list_node_t* node = __pop_node(list, pos.node);
    return __create_iterator(list->value_type, node);
}

c_list_iterator_t c_list_erase_range(c_list_t* list, c_list_iterator_t first, c_list_iterator_t last)
{
    c_list_iterator_t pos = first;
    while (pos.node != last.node) {
        pos = c_list_erase(list, pos);
    }

    return pos;
}

void c_list_push_back(c_list_t* list, c_ref_t value)
{
    if (!list || !value)
        return;

    c_list_node_t* node = __create_node(list, value);
    if (!node) return;

    node->next = list->node;
    node->prev = list->node->prev;
    list->node->prev->next = node;
    list->node->prev = node;
}

void c_list_pop_back(c_list_t* list)
{
    if (!c_list_empty(list))
        __pop_node(list, list->node->prev);
}

void c_list_push_front(c_list_t* list, c_ref_t value)
{
    if (!list || !value)
        return;

    c_list_node_t* node = __create_node(list, value);
    if (!node) return;

    node->next = list->node->next;
    node->prev = list->node;
    list->node->next->prev = node;
    list->node->next = node;
}

void c_list_pop_front(c_list_t* list)
{
    if (!c_list_empty(list))
        __pop_node(list, list->node->next);
}

void c_list_resize(c_list_t* list, size_t count)
{
    c_list_resize_with_value(list, count, 0);
}

void c_list_resize_with_value(c_list_t* list, size_t count, c_ref_t value)
{
    if (!list) return;

    c_list_node_t* node = list->node->next;
    while (node != list->node && count > 0) {
        node = node->next;
        --count;
    }

    if (count > 0) {
        do {
            c_list_node_t* node = __create_node(list, value);
            if (!node) return;

            node->next = list->node;
            node->prev = list->node->prev;
            list->node->prev->next = node;
            list->node->prev = node;
        } while (--count > 0);
    }
    else {
        while (node != list->node)
            node = __pop_node(list, node);
    }
}

void c_list_swap(c_list_t* list, c_list_t* other)
{
    if (!list || !other) return;
    c_list_t tmp = *list;
    *list = *other;
    *other = tmp;
}

/**
 * operations
 */
void c_list_merge(c_list_t* list, c_list_t* other)
{
    if (!list) return;
    c_list_merge_by(list, other, list->value_type->less);
}

void c_list_merge_by(c_list_t* list, c_list_t* other, c_compare comp)
{
    if (!list || c_list_empty(other) || !comp || !__c_is_same(list->value_type, other->value_type))
        return;

    c_list_node_t* node = __begin(list);
    c_list_node_t* last = __end(list);
    c_list_node_t* other_node = __begin(other);
    c_list_node_t* other_last = __end(other);
    while (node != last && other_node != other_last) {
        if (comp(other_node->value, node->value)) {
            c_list_node_t* other_next = other_node->next;
            __transfer(node, other_node, other_next);
            other_node = other_next;
        }
        else
            node = node->next;
    }

    if (other_node != other_last)
        __transfer(last, other_node, other_last);
}

void c_list_splice(c_list_t* list, c_list_iterator_t pos, c_list_t* other)
{
    if (!list || c_list_empty(other) || !__c_is_same(list->value_type, other->value_type))
        return;

    __transfer(pos.node, __begin(other), __end(other));
}

void c_list_splice_from(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t from)
{
    if (!list || c_list_empty(other) || !__c_is_same(list->value_type, other->value_type))
        return;

    __transfer(pos.node, from.node, __end(other));
}

void c_list_splice_range(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t first, c_list_iterator_t last)
{
    if (!list || c_list_empty(other) || !__c_is_same(list->value_type, other->value_type))
        return;

    __transfer(pos.node, first.node, last.node);
}

void c_list_remove(c_list_t* list, c_ref_t value)
{
    if (c_list_empty(list) || !value)
        return;

    c_list_node_t* node = __begin(list);
    c_list_node_t* last = __end(list);
    while (node != last) {
        if (list->value_type->equal(node->value, value))
            node = __pop_node(list, node);
        else
            node = node->next;
    }
}

void c_list_remove_if(c_list_t* list, c_unary_predicate pred)
{
    if (c_list_empty(list) || !pred)
        return;

    c_list_node_t* node = __begin(list);
    c_list_node_t* last = __end(list);
    while (node != last) {
        if (pred(node->value))
            node = __pop_node(list, node);
        else
            node = node->next;
    }
}

void c_list_sort(c_list_t* list)
{
    if (c_list_empty(list) || c_list_size(list) == 1)
        return;

    c_list_sort_by(list, list->value_type->less);
}

void c_list_sort_by(c_list_t* list, c_compare comp)
{
    if (c_list_empty(list) || c_list_size(list) == 1 || !comp)
        return;

    c_list_t* carry = c_list_create(list->value_type);
    if (!carry) return;

    c_list_t* counter[64] = { 0 };
    __array_foreach(counter, i) {
        counter[i] = c_list_create(list->value_type);
        if (!counter[i]) goto out;
    }

    int fill = 0;
    while (!c_list_empty(list)) {
        __transfer(__begin(carry), __begin(list), __begin(list)->next);
        int i = 0;
        while (i < fill && !c_list_empty(counter[i])) {
            c_list_merge_by(counter[i], carry, comp);
            c_list_swap(carry, counter[i++]);
        }
        c_list_swap(carry, counter[i]);
        if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; ++i)
        c_list_merge_by(counter[i], counter[i - 1], comp);
    c_list_swap(list, counter[fill - 1]);

out:
    __array_foreach(counter, i) {
        c_list_destroy(counter[i]);
    }
    c_list_destroy(carry);
}

void c_list_reverse(c_list_t* list)
{
    if (c_list_empty(list) || c_list_size(list) == 1) {
        return;
    }

    c_list_node_t* x = __begin(list);
    c_list_node_t* y = 0;
    while (x != __end(list)) {
        y = x->next;
        x->next = x->prev;
        x->prev = y;
        x = y;
    }

    y = list->node->next;
    list->node->next = list->node->prev;
    list->node->prev = y;
}

void c_list_unique(c_list_t* list)
{
    if (!list) return;
    c_list_unique_if(list, list->value_type->equal);
}

void c_list_unique_if(c_list_t* list, c_binary_predicate pred)
{
    if (c_list_empty(list) || c_list_size(list) == 1 || !pred)
        return;

    c_list_node_t* x = __begin(list);
    c_list_node_t* y = 0;
    while (x != __end(list) && x->next != __end(list)) {
        y = x->next;
        while (y != __end(list) && pred(x->value, y->value)) {
            y = __pop_node(list, y);
        }
        x = x->next;
    }
}

/**
 * backend
 */
c_backend_container_t* c_list_create_backend(c_containable_t* value_type)
{
    static c_backend_operation_t backend_list_ops = {
        .destroy = backend_destroy,
        .front = backend_front,
        .back = backend_back,
        .begin = backend_begin,
        .end = backend_end,
        .empty = backend_empty,
        .size = backend_size,
        .max_size = backend_max_size,
        .push_back = backend_push_back,
        .pop_back = backend_pop_back,
        .push_front = backend_push_front,
        .pop_front = backend_pop_front,
        .swap = backend_swap
    };

    c_backend_list_t* backend = (c_backend_list_t*)malloc(sizeof(c_backend_list_t));
    if (!backend) return 0;

    backend->impl = c_list_create(value_type);
    if (!backend->impl) {
        __c_free(backend);
        return 0;
    }

    backend->interface.ops = &backend_list_ops;

    return (c_backend_container_t*)backend;
}
