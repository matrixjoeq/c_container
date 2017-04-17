
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "c_internal.h"
#include "c_iterator.h"
#include "c_forward_list.h"

struct __c_slist_node {
    struct __c_slist_node* next;
    c_ref_t data;
};
typedef struct __c_slist_node c_slist_node_t;

struct __c_slist {
    c_slist_node_t* ancient; // before_begin() of list
    c_slist_node_t* node; // end() of list
    c_containable_t type_info;
};

__c_static __c_inline bool is_slist_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_FORWARD &&
            iter->iterator_type == C_ITER_TYPE_FORWARD_LIST);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_slist_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_slist_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_slist_iterator_t));
    }
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (is_slist_iterator(iter)) {
        c_slist_iterator_t* _iter = (c_slist_iterator_t*)iter;
        _iter->node = _iter->node->next;
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_slist_iterator(iter) && is_slist_iterator(tmp)) {
        c_slist_iterator_t* _iter = (c_slist_iterator_t*)iter;
        c_slist_iterator_t* _tmp = (c_slist_iterator_t*)tmp;
        _tmp->node = _iter->node;
        _iter->node = _iter->node->next;
    }
    return tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (is_slist_iterator(iter)) {
        c_slist_iterator_t* _iter = (c_slist_iterator_t*)iter;
        return _iter->node->data;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_slist_iterator(x) || !is_slist_iterator(y)) return false;

    c_slist_iterator_t* _x = (c_slist_iterator_t*)x;
    c_slist_iterator_t* _y = (c_slist_iterator_t*)y;
    return _x->node == _y->node;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (is_slist_iterator(iter)) {
        c_slist_iterator_t* _iter = (c_slist_iterator_t*)iter;
        while (n-- > 0) _iter->node = _iter->node->next;
    }
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_slist_iterator(first) || !is_slist_iterator(last)) return 0;

    c_slist_node_t* _first = ((c_slist_iterator_t*)first)->node;
    c_slist_node_t* _last = ((c_slist_iterator_t*)last)->node;
    size_t n = 0;
    while (_first != _last) {
        _first = _first->next;
        ++n;
    }

    return n;
}

__c_static c_slist_iterator_t create_iterator(
    c_containable_t* type_info, c_slist_node_t* node)
{
    assert(type_info);
    assert(node);

    c_slist_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_FORWARD,
            .iterator_type = C_ITER_TYPE_FORWARD_LIST,
            .alloc_and_copy = iter_alloc_and_copy,
            .increment = iter_increment,
            .decrement = 0,
            .post_increment = iter_post_increment,
            .post_decrement = 0,
            .dereference = iter_dereference,
            .equal = iter_equal,
            .not_equal = iter_not_equal,
            .advance = iter_advance,
            .distance = iter_distance,
            .type_info = type_info
        },
        .node = node
    };
    return iter;
}

__c_static __c_inline c_slist_node_t* before_begin(c_slist_t* list)
{
    assert(list);
    return list->ancient;
}

__c_static __c_inline c_slist_node_t* begin(c_slist_t* list)
{
    assert(list);
    return list->ancient->next;
}

__c_static __c_inline c_slist_node_t* end(c_slist_t* list)
{
    assert(list);
    return list->node;
}

__c_static c_slist_node_t* create_node(c_slist_t* list, c_ref_t data)
{
    assert(list);

    c_slist_node_t* node = (c_slist_node_t*)malloc(sizeof(c_slist_node_t));
    if (!node) return 0;

    node->next = 0;

    c_containable_t* type_info = &(list->type_info);
    assert(type_info);
    assert(type_info->size);
    node->data = malloc(type_info->size());
    if (!node->data) {
        __c_free(node);
        return 0;
    }

    if (data) {
        assert(type_info->copy);
        type_info->copy(node->data, data);
    }
    else {
        assert(type_info->create);
        type_info->create(node->data);
    }

    return node;
}

__c_static c_slist_node_t* pop_node_after(c_slist_t* list, c_slist_node_t* node)
{
    assert(list);

    if (!node || node == list->node)
        return list->node;

    c_slist_node_t* pos = node->next;
    if (!pos || pos == list->node || pos == list->ancient)
        return list->node;

    c_containable_t* type_info = &list->type_info;
    assert(type_info);
    assert(type_info->destroy);

    node->next = pos->next;
    type_info->destroy(pos->data);
    __c_free(pos->data);
    __c_free(pos);

    return node->next;
}

// move (first, last) after pos
__c_static void transfer(c_slist_node_t* pos, c_slist_node_t* first, c_slist_node_t* last)
{
    if (pos != first && first != last && first->next != last) {
        c_slist_node_t* last_prev = first->next;
        while (last_prev->next != last)
            last_prev = last_prev->next;

        last_prev->next = pos->next;
        pos->next = first->next;
        first->next = last;
    }
}

/**
 * constructor/destructor
 */
c_slist_t* c_slist_create(const c_containable_t* type_info)
{
    if (!type_info) return 0;

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

    list->type_info = *type_info;
    list->ancient->next = list->node;
    list->ancient->data = 0;
    list->node->next = 0;
    list->node->data = 0;

    return list;
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
    return c_slist_empty(list) ? 0 : begin(list)->data;
}

/**
 * iterators
 */
c_slist_iterator_t c_slist_before_begin(c_slist_t* list)
{
    assert(list);
    return create_iterator(&(list->type_info), before_begin(list));
}

c_slist_iterator_t c_slist_begin(c_slist_t* list)
{
    assert(list);
    return create_iterator(&(list->type_info), begin(list));
}

c_slist_iterator_t c_slist_end(c_slist_t* list)
{
    assert(list);
    return create_iterator(&(list->type_info), end(list));
}

/**
 * capacity
 */
bool c_slist_empty(c_slist_t* list)
{
    return list ? begin(list) == end(list) : true;
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

    c_slist_node_t* node = before_begin(list);
    c_slist_node_t* next = begin(list);
    c_slist_node_t* last = end(list);
    while (next != last) {
        next = pop_node_after(list, node);
    }
}

c_slist_iterator_t c_slist_insert_after(c_slist_t* list, c_slist_iterator_t pos, const c_ref_t data)
{
    assert(list);
    assert(data);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_FORWARD_LIST);

    c_slist_node_t* node = create_node(list, data);
    if (!node) return pos;

    node->next = pos.node->next;
    pos.node->next = node;

    return create_iterator(&(list->type_info), node);
}

c_slist_iterator_t c_slist_erase_after(c_slist_t* list, c_slist_iterator_t pos)
{
    assert(list);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_FORWARD_LIST);
    c_slist_node_t* node = pop_node_after(list, pos.node);
    return create_iterator(&(list->type_info), node);
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

void c_slist_push_front(c_slist_t* list, const c_ref_t data)
{
    if (!list || !data)
        return;

    c_slist_node_t* node = create_node(list, data);
    if (!node) return;

    node->next = list->ancient->next;
    list->ancient->next = node;
}

void c_slist_pop_front(c_slist_t* list)
{
    if (!c_slist_empty(list))
        pop_node_after(list, list->ancient);
}

void c_slist_resize(c_slist_t* list, size_t count)
{
    c_slist_resize_with_value(list, count, 0);
}

void c_slist_resize_with_value(c_slist_t* list, size_t count, const c_ref_t data)
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
            c_slist_node_t* node = create_node(list, data);
            if (!node) return;

            prev->next = node;
            node->next = list->node;
            prev = node;
        } while (--count > 0);
    }
    else {
        while (node != list->node)
            node = pop_node_after(list, prev);
    }
}

void c_slist_swap(c_slist_t* list, c_slist_t* other)
{
    if (!list || !other) return;

    c_slist_t* tmp = c_slist_create(&list->type_info);
    if (!tmp) return;

    transfer(before_begin(tmp), before_begin(list), end(list));
    transfer(before_begin(list), before_begin(other), end(other));
    transfer(before_begin(other), before_begin(tmp), end(tmp));
    other->type_info = tmp->type_info;

    c_slist_destroy(tmp);
}

/**
 * operations
 */

void c_slist_merge(c_slist_t* list, c_slist_t* other)
{
    if (!list) return;
    c_slist_merge_by(list, other, list->type_info.less);
}

void c_slist_merge_by(c_slist_t* list, c_slist_t* other, c_compare comp)
{
    if (!list || c_slist_empty(other) || !comp ||
        !type_info_equal(&list->type_info, &other->type_info))
        return;

    c_slist_node_t* node = before_begin(list);
    c_slist_node_t* last = end(list);
    c_slist_node_t* other_node = before_begin(other);
    c_slist_node_t* other_last = end(other);
    while (node->next != last && other_node->next != other_last) {
        if (comp(other_node->next->data, node->next->data))
            transfer(node, other_node, other_node->next->next);
        else
            node = node->next;
    }

    if (other_node->next != other_last)
        transfer(node, other_node, other_last);
}

void c_slist_splice_after(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other)
{
    if (!list || c_slist_empty(other) || !type_info_equal(&list->type_info, &other->type_info))
        return;

    transfer(pos.node, before_begin(other), end(other));
}

void c_slist_splice_after_from(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t from)
{
    if (!list || c_slist_empty(other) || !type_info_equal(&list->type_info, &other->type_info))
        return;

    transfer(pos.node, from.node, end(other));
}

void c_slist_splice_after_range(c_slist_t* list, c_slist_iterator_t pos, c_slist_t* other, c_slist_iterator_t first, c_slist_iterator_t last)
{
    if (!list || c_slist_empty(other) || !type_info_equal(&list->type_info, &other->type_info))
        return;

    transfer(pos.node, first.node, last.node);
}

void c_slist_remove(c_slist_t* list, const c_ref_t data)
{
    if (c_slist_empty(list) || !data)
        return;

    c_slist_node_t* prev = before_begin(list);
    c_slist_node_t* node = prev->next;
    c_slist_node_t* last = end(list);
    c_containable_t* type_info = &list->type_info;
    assert(type_info->equal);
    while (node != last && prev != last) {
        if (type_info->equal(node->data, data))
            node = pop_node_after(list, prev);
        else {
            prev = prev->next;
            node = prev->next;
        }
    }
}

void c_slist_remove_if(c_slist_t* list, c_unary_predicate pred)
{
    if (c_slist_empty(list) || !pred)
        return;

    c_slist_node_t* prev = before_begin(list);
    c_slist_node_t* node = prev->next;
    c_slist_node_t* last = end(list);
    while (node != last && prev != last) {
        if (pred(node->data))
            node = pop_node_after(list, prev);
        else {
            prev = prev->next;
            node = prev->next;
        }
    }
}

void c_slist_sort(c_slist_t* list)
{
    if (!list) return;
    c_slist_sort_by(list, list->type_info.less);
}

void c_slist_sort_by(c_slist_t* list, c_compare comp)
{
    if (c_slist_empty(list) || begin(list)->next == end(list) || !comp) return;

    c_slist_t* carry = c_slist_create(&list->type_info);
    if (!carry) return;

    c_slist_t* counter[64] = { 0 };
    __array_foreach(counter, i) {
        counter[i] = c_slist_create(&list->type_info);
        if (!counter[i]) goto out;
    }

    int fill = 0;
    while (!c_slist_empty(list)) {
        transfer(before_begin(carry), before_begin(list), begin(list)->next);
        int i = 0;
        while (i < fill && !c_slist_empty(counter[i])) {
            c_slist_merge_by(counter[i], carry, comp);
            c_slist_swap(carry, counter[i++]);
        }
        c_slist_swap(carry, counter[i]);
        if (i == fill) ++fill;
    }

    for (int i = 1; i < fill; ++i)
        c_slist_merge_by(counter[i], counter[i - 1], comp);
    c_slist_swap(list, counter[fill - 1]);

out:
    __array_foreach(counter, i) {
        c_slist_destroy(counter[i]);
    }
    c_slist_destroy(carry);
}

void c_slist_reverse(c_slist_t* list)
{
    if (c_slist_empty(list) || begin(list)->next == end(list)) return;

    c_slist_node_t* node = begin(list);
    c_slist_node_t* last = end(list);
    c_slist_node_t* next = node->next;
    while (node != end(list)) {
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
    c_slist_unique_if(list, list->type_info.equal);
}

void c_slist_unique_if(c_slist_t* list, c_binary_predicate pred)
{
    if (c_slist_empty(list) || begin(list)->next == end(list) || !pred) return;

    c_slist_node_t* last = end(list);
    c_slist_node_t* x = begin(list);
    c_slist_node_t* y = 0;
    while (x != last && x->next != last) {
        y = x->next;
        while (y != last && pred(x->data, y->data)) {
            y = pop_node_after(list, x);
        }
        x = x->next;
    }
}
