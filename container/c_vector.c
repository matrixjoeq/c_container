

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "c_internal.h"
#include "c_iterator.h"
#include "c_vector.h"

struct __c_vector {
    c_ref_t start;
    c_ref_t finish;
    c_ref_t end_of_storage;
    c_containable_t type_info;
};

struct __c_backend_vector {
    c_backend_container_t ops;
    c_vector_t* impl;
};

__c_static bool is_valid_pos(c_vector_t* vector, c_ref_t pos)
{
    if (!vector) return false;

    if (pos >= vector->start && pos <= vector->finish) {
        ptrdiff_t diff = pos - vector->start;
        assert(vector->type_info.size);
        if (diff % vector->type_info.size() == 0)
            return true;
    }

    return false;
}

__c_static __c_inline bool is_vector_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_VECTOR);
}

__c_static __c_inline bool is_vector_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_VECTOR_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_vector_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_vector_iterator_t));
    }
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (is_vector_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (is_vector_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_vector_iterator(iter) && is_vector_iterator(tmp)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)tmp)->pos = ((c_vector_iterator_t*)iter)->pos;
        ((c_vector_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_vector_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)tmp)->pos = ((c_vector_iterator_t*)iter)->pos;
        ((c_vector_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (is_vector_iterator(iter)) {
        return ((c_vector_iterator_t*)iter)->pos;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_vector_iterator(x) || !is_vector_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos == ((c_vector_iterator_t*)y)->pos;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!is_vector_iterator(iter)) return;
    assert(iter->type_info);
    assert(iter->type_info->size);
    ((c_vector_iterator_t*)iter)->pos += (n * iter->type_info->size());
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_vector_iterator(first) || !is_vector_iterator(last)) return 0;
    assert(first->type_info);
    assert(first->type_info->size);
    return (ptrdiff_t)(((c_vector_iterator_t*)last)->pos - ((c_vector_iterator_t*)first)->pos) / first->type_info->size();
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_vector_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_vector_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (is_vector_reverse_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (is_vector_reverse_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_vector_reverse_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)tmp)->pos = ((c_vector_iterator_t*)iter)->pos;
        ((c_vector_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_vector_reverse_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        ((c_vector_iterator_t*)tmp)->pos = ((c_vector_iterator_t*)iter)->pos;
        ((c_vector_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (is_vector_reverse_iterator(iter)) {
        assert(iter->type_info);
        assert(iter->type_info->size);
        return (c_ref_t)(((c_vector_iterator_t*)iter)->pos - iter->type_info->size());
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_vector_reverse_iterator(x) || !is_vector_reverse_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos == ((c_vector_iterator_t*)y)->pos;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static void reverse_iter_advance(c_iterator_t* iter, size_t n)
{
    if (!is_vector_reverse_iterator(iter)) return;
    assert(iter->type_info);
    assert(iter->type_info->size);
    ((c_vector_iterator_t*)iter)->pos -= (n * iter->type_info->size());
}

__c_static size_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_vector_reverse_iterator(first) || !is_vector_reverse_iterator(last)) return 0;
    assert(first->type_info);
    assert(first->type_info->size);
    return (size_t)(((c_vector_iterator_t*)first)->pos - ((c_vector_iterator_t*)last)->pos) / first->type_info->size();
}

__c_static c_vector_iterator_t create_iterator(
    c_containable_t* type_info, c_ref_t pos)
{
    assert(type_info);

    c_vector_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_VECTOR,
            .alloc_and_copy = iter_alloc_and_copy,
            .increment = iter_increment,
            .decrement = iter_decrement,
            .post_increment = iter_post_increment,
            .post_decrement = iter_post_decrement,
            .dereference = iter_dereference,
            .equal = iter_equal,
            .not_equal = iter_not_equal,
            .advance = iter_advance,
            .distance = iter_distance,
            .type_info = type_info
        },
        .pos = pos
    };
    return iter;
}

__c_static c_vector_iterator_t create_reverse_iterator(
    c_containable_t* type_info, c_ref_t pos)
{
    assert(type_info);
    assert(pos);

    c_vector_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_VECTOR_REVERSE,
            .alloc_and_copy = reverse_iter_alloc_and_copy,
            .increment = reverse_iter_increment,
            .decrement = reverse_iter_decrement,
            .post_increment = reverse_iter_post_increment,
            .post_decrement = reverse_iter_post_decrement,
            .dereference = reverse_iter_dereference,
            .equal = reverse_iter_equal,
            .not_equal = reverse_iter_not_equal,
            .advance = reverse_iter_advance,
            .distance = reverse_iter_distance,
            .type_info = type_info
        },
        .pos = pos
    };
    return iter;
}

__c_static __c_inline c_ref_t begin(c_vector_t* vector)
{
    assert(vector);
    return vector->start;
}

__c_static __c_inline c_ref_t end(c_vector_t* vector)
{
    assert(vector);
    return vector->finish;
}

__c_static __c_inline c_ref_t eos(c_vector_t* vector)
{
    assert(vector);
    return vector->end_of_storage;
}

__c_static __c_inline size_t available(c_vector_t* vector)
{
    assert(vector);
    assert(vector->type_info.size);
    return (vector->end_of_storage - vector->finish) / vector->type_info.size();
}

__c_static void backend_destroy(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_destroy(_c->impl);
    __c_free(_c);
}

__c_static c_ref_t backend_front(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    return c_vector_front(_c->impl);
}

__c_static c_ref_t backend_back(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    return c_vector_back(_c->impl);
}

__c_static c_iterator_t* backend_begin(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_iterator_t first = c_vector_begin(_c->impl);
    *iter = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
    if (*iter) {
        C_ITER_COPY(*iter, &first);
    }

    return *iter;
}

__c_static c_iterator_t* backend_end(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_iterator_t last = c_vector_end(_c->impl);
    *iter = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
    if (*iter) {
        C_ITER_COPY(*iter, &last);
    }

    return *iter;
}

__c_static bool backend_empty(c_backend_container_t* c)
{
    if (!c) return true;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    return c_vector_empty(_c->impl);
}

__c_static size_t backend_size(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    return c_vector_size(_c->impl);
}

__c_static size_t backend_max_size(void)
{
    return (-1);
}

__c_static void backend_push_back(c_backend_container_t* c, const c_ref_t data)
{
    if (!c || !data) return;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_push_back(_c->impl, data);
}

__c_static void backend_pop_back(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_pop_back(_c->impl);
}

__c_static void backend_swap(c_backend_container_t* c, c_backend_container_t* other)
{
    if (!c || !other) return;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_backend_vector_t* _other = (c_backend_vector_t*)other;
    c_backend_container_t tmp = _c->ops;
    c_vector_swap(_c->impl, _other->impl);
    _c->ops = _other->ops;
    _other->ops = tmp;
}

__c_static void destroy(c_vector_iterator_t first, c_vector_iterator_t last)
{
    c_containable_t* type_info = first.base_iter.type_info;
    assert(type_info);
    assert(type_info->destroy);
    while (C_ITER_NE(&first, &last)) {
        type_info->destroy(first.pos);
        C_ITER_INC(&first);
    }
}

__c_static void fill(c_vector_iterator_t pos, size_t n, const c_ref_t data)
{
    c_containable_t* type_info = pos.base_iter.type_info;
    assert(type_info);
    assert(type_info->size);
    while (n--) {
        if (data) {
            assert(type_info->copy);
            type_info->copy(pos.pos, data);
        }
        else {
            assert(type_info->create);
            type_info->create(pos.pos);
        }
        C_ITER_INC(&pos);
    }
}

__c_static int reallocate_and_move(c_vector_t* vector, size_t n)
{
    assert(vector);

    c_containable_t* type_info = &vector->type_info;
    assert(type_info);
    assert(type_info->size);
    size_t data_size = type_info->size();

    // double the capacity or make it large enough
    size_t size = c_vector_size(vector);
    size_t cap = __c_max(c_vector_capacity(vector) * 2, n + size);
    c_ref_t start = malloc(cap * data_size);
    if (!start) return -1;

    memcpy(start, vector->start, vector->finish - vector->start);
    destroy(c_vector_begin(vector), c_vector_end(vector));
    __c_free(vector->start);
    vector->start = start;
    vector->finish = start + size * data_size;
    vector->end_of_storage = start + cap * data_size;

    return 0;
}

/**
 * constructor/destructor
 */
c_vector_t* c_vector_create(const c_containable_t* type_info)
{
    if (!type_info) return 0;

    c_vector_t* vector = (c_vector_t*)malloc(sizeof(c_vector_t));
    if (!vector) return 0;

    vector->start = 0;
    vector->finish = 0;
    vector->end_of_storage = 0;
    vector->type_info = *type_info;

    return vector;
}

void c_vector_destroy(c_vector_t* vector)
{
    if (!vector) return;

    c_vector_clear(vector);
    __c_free(vector->start);
    __c_free(vector);
}

/**
 * element access
 */
c_ref_t c_vector_at(c_vector_t* vector, size_t pos)
{
    if (!vector) return 0;
    c_containable_t* type_info = &vector->type_info;
    assert(type_info->size);
    return C_REF_T(begin(vector) + type_info->size() * pos);
}

c_ref_t c_vector_front(c_vector_t* vector)
{
    return c_vector_empty(vector) ? 0 : begin(vector);
}

c_ref_t c_vector_back(c_vector_t* vector)
{
    if (c_vector_empty(vector)) return 0;

    c_containable_t* type_info = &vector->type_info;
    assert(type_info->size);
    return C_REF_T(end(vector) - type_info->size());
}

c_ref_t c_vector_data(c_vector_t* vector)
{
    if (!vector) return 0;
    return begin(vector);
}

/**
 * iterators
 */
c_vector_iterator_t c_vector_begin(c_vector_t* vector)
{
    assert(vector);
    return create_iterator(&vector->type_info, begin(vector));
}

c_vector_iterator_t c_vector_rbegin(c_vector_t* vector)
{
    assert(vector);
    return create_reverse_iterator(&vector->type_info, end(vector));
}

c_vector_iterator_t c_vector_end(c_vector_t* vector)
{
    assert(vector);
    return create_iterator(&vector->type_info, end(vector));
}

c_vector_iterator_t c_vector_rend(c_vector_t* vector)
{
    assert(vector);
    return create_reverse_iterator(&vector->type_info, begin(vector));
}

/**
 * capacity
 */
bool c_vector_empty(c_vector_t* vector)
{
    return vector ? begin(vector) == end(vector) : true;
}

size_t c_vector_size(c_vector_t* vector)
{
    if (!vector) return 0;

    c_containable_t* type_info = &vector->type_info;
    assert(type_info->size);
    return (end(vector) - begin(vector)) / type_info->size();
}

size_t c_vector_max_size(void)
{
    return (-1);
}

void c_vector_reserve(c_vector_t* vector, size_t new_cap)
{
    if (!vector || new_cap <= c_vector_capacity(vector)) return;
    c_vector_resize(vector, new_cap);
}

size_t c_vector_capacity(c_vector_t* vector)
{
    if (!vector) return 0;

    c_containable_t* type_info = &vector->type_info;
    assert(type_info->size);
    return (eos(vector) - begin(vector)) / type_info->size();
}

void c_vector_shrink_to_fit(c_vector_t* vector)
{
    if (!vector || eos(vector) == end(vector)) return;

    size_t size = (size_t)(vector->finish - vector->start);
    if (size == 0) {
        __c_free(vector->start);
        vector->end_of_storage = vector->finish = vector->start = 0;
        return;
    }

    c_ref_t start = malloc(size);
    if (!start) return;

    memcpy(start, vector->start, size);
    destroy(c_vector_begin(vector), c_vector_end(vector));
    __c_free(vector->start);
    vector->start = start;
    vector->finish = start + size;
    vector->end_of_storage = vector->finish;
}

/**
 * modifiers
 */
void c_vector_clear(c_vector_t* vector)
{
    if (c_vector_empty(vector)) return;
    destroy(c_vector_begin(vector), c_vector_end(vector));
    vector->finish = vector->start;
}

c_vector_iterator_t c_vector_insert(c_vector_t* vector, c_vector_iterator_t pos, const c_ref_t data)
{
    return c_vector_insert_n(vector, pos, 1, data);
}

c_vector_iterator_t c_vector_insert_n(
    c_vector_t* vector, c_vector_iterator_t pos, size_t count, const c_ref_t data)
{
    assert(vector);
    assert(data);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_VECTOR);
    if (!is_valid_pos(vector, pos.pos)) return c_vector_end(vector);

    if (available(vector) < count) {
        ptrdiff_t diff = pos.pos - vector->start;
        if (reallocate_and_move(vector, count - available(vector)))
            return c_vector_end(vector);

        pos.pos = vector->start + diff;
    }

    c_containable_t* type_info = &vector->type_info;
    assert(type_info);
    assert(type_info->size);

    memmove(pos.pos + count * type_info->size(), pos.pos, vector->finish - pos.pos);
    fill(pos, count, data);
    vector->finish += count * type_info->size();

    return pos;
}

c_vector_iterator_t c_vector_insert_range(
    c_vector_t* vector, c_vector_iterator_t pos, c_iterator_t first, c_iterator_t last)
{
    assert(vector);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_VECTOR);
    assert(first.iterator_category >= C_ITER_CATE_INPUT);
    assert(last.iterator_category >= C_ITER_CATE_INPUT);
    assert(type_info_equal(pos.base_iter.type_info, first.type_info));
    assert(type_info_equal(first.type_info, last.type_info));

    if (!is_valid_pos(vector, pos.pos)) return c_vector_end(vector);

    while (C_ITER_NE(&first, &last)) {
        pos = c_vector_insert(vector, pos, C_ITER_DEREF(&first));
        if (pos.pos == vector->finish)
            return pos;

        C_ITER_INC(&pos);
        C_ITER_INC(&first);
    }

    return pos;
}

c_vector_iterator_t c_vector_erase(c_vector_t* vector, c_vector_iterator_t pos)
{
    assert(vector);
    assert(pos.base_iter.iterator_type == C_ITER_TYPE_VECTOR);

    if (!is_valid_pos(vector, pos.pos) || pos.pos == vector->finish)
        return c_vector_end(vector);

    c_containable_t* type_info = &vector->type_info;
    assert(type_info);
    assert(type_info->destroy);
    assert(type_info->size);

    type_info->destroy(pos.pos);
    c_ref_t next_pos = pos.pos + type_info->size();
    memmove(pos.pos, next_pos, vector->finish - next_pos);
    vector->finish -= type_info->size();

    return pos;
}

c_vector_iterator_t c_vector_erase_range(
    c_vector_t* vector, c_vector_iterator_t first, c_vector_iterator_t last)
{
    assert(vector);
    assert(first.base_iter.iterator_type == C_ITER_TYPE_VECTOR);
    assert(last.base_iter.iterator_type == C_ITER_TYPE_VECTOR);

    if (!is_valid_pos(vector, first.pos) ||
        !is_valid_pos(vector, last.pos) ||
        (first.pos > last.pos))
        return c_vector_end(vector);

    if (first.pos == last.pos) return last;

    destroy(first, last);
    size_t size = vector->finish - last.pos;
    memmove(first.pos, last.pos, size);
    vector->finish -= (last.pos - first.pos);

    return first;
}

void c_vector_push_back(c_vector_t* vector, const c_ref_t data)
{
    if (!vector || !data) return;

    if (vector->finish == vector->end_of_storage) {
        if (reallocate_and_move(vector, 1))
            return;
    }

    c_containable_t* type_info = &vector->type_info;
    assert(type_info);
    assert(type_info->copy);
    type_info->copy(vector->finish, data);
    assert(type_info->size);
    vector->finish += type_info->size();
}

void c_vector_pop_back(c_vector_t* vector)
{
    if (!c_vector_empty(vector)) {
        c_containable_t* type_info = &vector->type_info;
        assert(type_info);
        assert(type_info->size);
        assert(type_info->destroy);
        type_info->destroy(c_vector_back(vector));
        vector->finish -= type_info->size();
    }
}

void c_vector_resize(c_vector_t* vector, size_t count)
{
    c_vector_resize_with_value(vector, count, 0);
}

void c_vector_resize_with_value(c_vector_t* vector, size_t count, const c_ref_t data)
{
    if (!vector) return;

    c_containable_t* type_info = &vector->type_info;
    assert(type_info->size);
    size_t data_size = type_info->size();

    if (count > c_vector_size(vector)) {
        count -= c_vector_size(vector);
        if (count > available(vector)) {
            if (reallocate_and_move(vector, count))
                return;
        }
        fill(c_vector_end(vector), count, data);
        vector->finish += count * data_size;
    }
    else {
        c_ref_t pos = vector->start + count * data_size;
        destroy(create_iterator(type_info, pos), c_vector_end(vector));
        vector->finish = pos;
    }
}

void c_vector_swap(c_vector_t* vector, c_vector_t* other)
{
    if (!vector || !other) return;

    c_vector_t tmp = *vector;
    *vector = *other;
    *other = tmp;
}

/**
 * backend
 */
c_backend_container_t* c_vector_create_backend(const c_containable_t* type_info)
{
    static const c_backend_container_t backend_vector_ops = {
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
        .push_front = 0,
        .pop_front = 0,
        .swap = backend_swap
    };

    c_backend_vector_t* backend = (c_backend_vector_t*)malloc(sizeof(c_backend_vector_t));
    if (!backend) return 0;

    backend->impl = c_vector_create(type_info);
    if (!backend->impl) {
        __c_free(backend);
        return 0;
    }

    backend->ops = backend_vector_ops;

    return (c_backend_container_t*)backend;
}
