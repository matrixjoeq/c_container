

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "c_internal.h"
#include "c_list.h"

struct __c_list_node {
    struct __c_list_node* prev;
    struct __c_list_node* next;
    c_ref_t data;
};
typedef struct __c_list_node c_list_node_t;

struct __c_list {
    c_list_node_t* node;
    c_containable_t type_info;
};

static const char* const s_list_iterator_category = "bidirection_iterator";
static const char* const s_list_iterator_type = "list_iterator";
static const char* const s_list_reverse_iterator_category = "bidirection_iterator";
static const char* const s_list_reverse_iterator_type = "list_reverse_iterator";

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    c_list_iterator_t* _iter = (c_list_iterator_t*)iter;
    _iter->node = _iter->node->next;
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    c_list_iterator_t* _iter = (c_list_iterator_t*)iter;
    _iter->node = _iter->node->prev;
    return iter;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    c_list_iterator_t* _iter = (c_list_iterator_t*)iter;
    return _iter->node->data;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    c_list_iterator_t* _x = (c_list_iterator_t*)x;
    c_list_iterator_t* _y = (c_list_iterator_t*)y;
    return _x->node == _y->node;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static c_list_iterator_t create_iterator(c_containable_t* type_info, c_list_node_t* node)
{
    assert(type_info);
    assert(node);

    c_list_iterator_t iter = {
        .base_iter = {
            .iterator_category = s_list_iterator_category,
            .iterator_type = s_list_iterator_type,
            .increment = iter_increment,
            .decrement = iter_decrement,
            .dereference = iter_dereference,
            .equal = iter_equal,
            .not_equal = iter_not_equal,
            .type_info = type_info
        },
        .node = node
    };
    return iter;
}

__c_static c_list_iterator_t create_reverse_iterator(c_containable_t* type_info, c_list_node_t* node)
{
    assert(type_info);
    assert(node);

    c_list_iterator_t iter = {
        .base_iter = {
            .iterator_category = s_list_reverse_iterator_category,
            .iterator_type = s_list_reverse_iterator_type,
            .increment = iter_decrement,
            .decrement = iter_increment,
            .dereference = iter_dereference,
            .equal = iter_equal,
            .not_equal = iter_not_equal,
            .type_info = type_info
        },
        .node = node
    };
    return iter;
}

__c_static __c_inline c_list_node_t* begin(c_list_t* list)
{
    return list->node->next;
}

__c_static __c_inline c_list_node_t* end(c_list_t* list)
{
    return list->node;
}

__c_static c_list_node_t* create_node(
    c_list_t* list, c_list_node_t** node, c_ref_t data)
{
    assert(list);
    assert(node);
    assert(*node == 0);

    *node = (c_list_node_t*)malloc(sizeof(c_list_node_t));
    if (!(*node))
        return 0;

    (*node)->prev = 0;
    (*node)->next = 0;

    c_containable_t* type_info = &(list->type_info);
    assert(type_info);
    assert(type_info->size);
    (*node)->data = malloc(type_info->size());
    if (!(*node)->data) {
        __c_free(*node);
        return 0;
    }

    if (data) {
        assert(type_info->copy);
        type_info->copy((*node)->data, data);
    }
    else {
        assert(type_info->create);
        type_info->create((*node)->data);
    }

    return *node;
}

__c_static c_list_node_t* pop_node(c_list_t* list, c_list_node_t* node)
{
    assert(list);
    assert(node);

    if (node == list->node)
        return node;

    c_containable_t* type_info = &(list->type_info);
    assert(type_info);
    assert(type_info->destroy);

    c_list_node_t* next_node = node->next;
    c_list_node_t* prev_node = node->prev;

    next_node->prev = prev_node;
    prev_node->next = next_node;
    type_info->destroy(node->data);
    __c_free(node->data);
    __c_free(node);

    return next_node;
}

// move [first, last) in front of pos
__c_static void transfer(c_list_node_t* pos, c_list_node_t* first, c_list_node_t* last)
{
    if (pos != last) {
        pos->prev->next = first;
        first->prev->next = last;
        last->prev->next = pos;

        c_list_node_t* tmp = pos->prev;
        pos->prev = last->prev;
        last->prev = first->prev;
        first->prev = tmp;
    }
}

/**
 * constructor/destructor
 */
c_list_t* c_list_create(c_list_t** list, c_containable_t type_info)
{
    if (!list || *list)
        return 0;

    *list = (c_list_t*)malloc(sizeof(c_list_t));
    if (!(*list))
        return 0;

    (*list)->node = (c_list_node_t*)malloc(sizeof(c_list_node_t));;
    if (!(*list)->node) {
        __c_free(*list);
        return 0;
    }

    (*list)->type_info = type_info;
    (*list)->node->prev = (*list)->node;
    (*list)->node->next = (*list)->node;
    (*list)->node->data = 0;

    return *list;
}

void c_list_destroy(c_list_t* list)
{
    if (!list)
        return;

    c_containable_t* type_info = &(list->type_info);
    c_list_node_t* first = begin(list);
    c_list_node_t* last = end(list);
    c_list_node_t* next = 0;
    assert(type_info->destroy);
    while (first != last) {
        assert(first);
        next = first->next;
        type_info->destroy(first->data);
        __c_free(first->data);
        __c_free(first);
        first = next;
    }

    __c_free(last);
    __c_free(list);
}

/**
 * element access
 */
c_ref_t c_list_front(c_list_t* list)
{
    return c_list_empty(list) ? 0 : begin(list)->data;
}

c_ref_t c_list_back(c_list_t* list)
{
    return c_list_empty(list) ? 0 : end(list)->prev->data;
}

/**
 * iterators
 */
c_list_iterator_t c_list_begin(c_list_t* list)
{
    assert(list);
    return create_iterator(&(list->type_info), begin(list));
}

c_list_iterator_t c_list_rbegin(c_list_t* list)
{
    assert(list);
    return create_reverse_iterator(&(list->type_info), end(list));
}

c_list_iterator_t c_list_end(c_list_t* list)
{
    assert(list);
    return create_iterator(&(list->type_info), end(list));
}

c_list_iterator_t c_list_rend(c_list_t* list)
{
    assert(list);
    return create_reverse_iterator(&(list->type_info), begin(list));
}

/**
 * capacity
 */
bool c_list_empty(c_list_t* list)
{
    return list ? begin(list) == end(list) : true;
}

size_t c_list_size(c_list_t* list)
{
    if (!list)
        return 0;

    size_t size = 0;
    for (c_list_node_t* node = begin(list);
         node != end(list);
         node = node->next) {
        assert(node);
        ++size;
    }

    return size;
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
    c_list_node_t* node = begin(list);
    c_list_node_t* last = end(list);
    while (node != last) {
        node = pop_node(list, node);
    }
}

c_list_iterator_t c_list_insert(c_list_t* list, c_list_iterator_t pos, const c_ref_t data)
{
    assert(list);
    assert(data);

    c_list_node_t* node = 0;
    if (!create_node(list, &node, data)) {
        return pos;
    }

    node->next = pos.node;
    node->prev = pos.node->prev;
    pos.node->prev->next = node;
    pos.node->prev = node;

    return create_iterator(&(list->type_info), node);
}

c_list_iterator_t c_list_erase(c_list_t* list, c_list_iterator_t pos)
{
    assert(list);
    c_list_node_t* node = pop_node(list, pos.node);
    return create_iterator(&(list->type_info), node);
}

c_list_iterator_t c_list_erase_range(c_list_t* list, c_list_iterator_t first, c_list_iterator_t last)
{
    c_list_iterator_t pos = first;
    while (pos.node != last.node) {
        pos = c_list_erase(list, pos);
    }

    return pos;
}

void c_list_push_back(c_list_t* list, const c_ref_t data)
{
    if (!list || !data)
        return;

    c_list_node_t* node = 0;
    if (!create_node(list, &node, data))
        return;

    node->next = list->node;
    node->prev = list->node->prev;
    list->node->prev->next = node;
    list->node->prev = node;
}

void c_list_pop_back(c_list_t* list)
{
    if (!c_list_empty(list))
        pop_node(list, list->node->prev);
}

void c_list_push_front(c_list_t* list, const c_ref_t data)
{
    if (!list || !data)
        return;

    c_list_node_t* node = 0;
    if (!create_node(list, &node, data))
        return;

    node->next = list->node->next;
    node->prev = list->node;
    list->node->next->prev = node;
    list->node->next = node;
}

void c_list_pop_front(c_list_t* list)
{
    if (!c_list_empty(list))
        pop_node(list, list->node->next);
}

/**
 * operations
 */
void c_list_merge(c_list_t* list, c_list_t* other)
{
    if (!list || c_list_empty(other))
        return;

    //assert(list->type_info == other->type_info);
    c_list_node_t* node = begin(list);
    c_list_node_t* last = end(list);
    c_list_node_t* other_node = begin(other);
    c_list_node_t* other_last = end(other);
    c_containable_t* type_info = &(list->type_info);
    assert(type_info->less);
    while (node != last && other_node != other_last) {
        if (type_info->less(other_node->data, node->data)) {
            c_list_node_t* other_next = other_node->next;
            transfer(node, other_node, other_next);
            other_node = other_next;
        }
        else
            node = node->next;
    }

    if (other_node != other_last)
        transfer(last, other_node, other_last);
}

void c_list_merge_by(c_list_t* list, c_list_t* other, c_compare comp)
{
    if (!list || c_list_empty(other) || !comp)
        return;

    c_list_node_t* node = begin(list);
    c_list_node_t* last = end(list);
    c_list_node_t* other_node = begin(other);
    c_list_node_t* other_last = end(other);
    while (node != last && other_node != other_last) {
        if (comp(other_node->data, node->data)) {
            c_list_node_t* other_next = other_node->next;
            transfer(node, other_node, other_next);
            other_node = other_next;
        }
        else
            node = node->next;
    }

    if (other_node != other_last)
        transfer(last, other_node, other_last);
}

void c_list_remove(c_list_t* list, const c_ref_t data)
{
    if (c_list_empty(list) || !data)
        return;

    c_list_node_t* node = begin(list);
    c_list_node_t* last = end(list);
    c_containable_t* type_info = &(list->type_info);
    assert(type_info->equal || type_info->less);
    while (node != last) {
        if (type_info->equal) {
            if (type_info->equal(node->data, data))
                node = pop_node(list, node);
            else
                node = node->next;
        }
        else {
            if (!type_info->less(node->data, data) &&
                !type_info->less(data, node->data))
                node = pop_node(list, node);
            else
                node = node->next;
        }
    }
}

void c_list_remove_if(c_list_t* list, c_unary_predicate pred)
{
    if (c_list_empty(list) || !pred)
        return;

    c_list_node_t* node = begin(list);
    c_list_node_t* last = end(list);
    while (node != last) {
        if (pred(node->data))
            node = pop_node(list, node);
        else
            node = node->next;
    }
}

void c_list_sort(c_list_t* list)
{
    if (c_list_empty(list) || c_list_size(list) == 1)
        return;

    /*
    INSERT-SORT(A):
    for (i = 1; i < length(A); ++i) {
        key = A[i]; // key is the value to be inserted
        j = i - 1;
        while (j >= 0 && key < A[j]) {
            // sort in low priority order
            // > A[j], sort in high priority order
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1]=key;
    }
    */
    c_containable_t* type_info = &(list->type_info);
    assert(type_info->less);
    // start from the second element
    for (c_list_node_t* i = begin(list)->next; i != end(list); i = i->next) {
        c_ref_t key = i->data;
        c_list_node_t* j = i->prev;
        while (j != end(list) && type_info->less(key, j->data)) {
            j->next->data = j->data;
            j = j->prev;
        }
        j->next->data = key;
    }
}

void c_list_sort_by(c_list_t* list, c_compare comp)
{
    if (c_list_empty(list) || c_list_size(list) == 1 || !comp)
        return;

    // start from the second element
    for (c_list_node_t* i = begin(list)->next; i != end(list); i = i->next) {
        c_ref_t key = i->data;
        c_list_node_t* j = i->prev;
        while (j != end(list) && comp(key, j->data)) {
            j->next->data = j->data;
            j = j->prev;
        }
        j->next->data = key;
    }
}

void c_list_reverse(c_list_t* list)
{
    if (c_list_empty(list) || c_list_size(list) == 1) {
        return;
    }

    c_list_node_t* x = begin(list);
    c_list_node_t* y = 0;
    while (x != end(list)) {
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
    if (c_list_empty(list) || c_list_size(list) == 1)
        return;

    c_list_node_t* x = begin(list);
    c_list_node_t* y = 0;
    c_containable_t* type_info = &(list->type_info);
    assert(type_info->equal);
    while (x != end(list) && x->next != end(list)) {
        y = x->next;
        while (y != end(list) && type_info->equal(x->data, y->data)) {
            y = pop_node(list, y);
        }
        x = x->next;
    }
}

void c_list_unique_if(c_list_t* list, c_binary_predicate pred)
{
    if (c_list_empty(list) || c_list_size(list) == 1 || !pred)
        return;

    c_list_node_t* x = begin(list);
    c_list_node_t* y = 0;
    while (x != end(list) && x->next != end(list)) {
        y = x->next;
        while (y != end(list) && pred(x->data, y->data)) {
            y = pop_node(list, y);
        }
        x = x->next;
    }
}
