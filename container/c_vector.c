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
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "c_internal.h"
#include "c_util.h"
#include "c_algorithm.h"
#include "c_vector.h"

struct __c_vector {
    c_ref_t start;
    c_ref_t finish;
    c_storage_t end_of_storage;
    const c_type_info_t* value_type;
};

struct __c_backend_vector {
    c_backend_container_t interface;
    c_vector_t* impl;
};

__c_static __c_inline bool __is_valid_pos(c_vector_t* vector, c_ref_t pos)
{
    if (!vector) return false;

    if (pos >= vector->start && pos <= vector->finish) {
        ptrdiff_t diff = pos - vector->start;
        if (diff % vector->value_type->size() == 0)
            return true;
    }

    return false;
}

__c_static __c_inline bool __is_vector_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_VECTOR);
}

__c_static __c_inline bool __is_vector_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_VECTOR_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_vector_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_vector_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_vector_iterator(dst) && __is_vector_iterator(src) && dst != src) {
        ((c_vector_iterator_t*)dst)->pos = ((c_vector_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (__is_vector_iterator(iter)) {
        ((c_vector_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (__is_vector_iterator(iter)) {
        ((c_vector_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_vector_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_vector_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_vector_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return *tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_vector_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_vector_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_vector_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return *tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (__is_vector_iterator(iter)) {
        return ((c_vector_iterator_t*)iter)->pos;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_vector_iterator(x) || !__is_vector_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos == ((c_vector_iterator_t*)y)->pos;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static bool iter_less(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_vector_iterator(x) || !__is_vector_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos < ((c_vector_iterator_t*)y)->pos;
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!__is_vector_iterator(iter) || n == 0) return;
    ((c_vector_iterator_t*)iter)->pos += (n * iter->value_type->size());
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_vector_iterator(first) || !__is_vector_iterator(last)) return 0;
    return (ptrdiff_t)(((c_vector_iterator_t*)last)->pos - ((c_vector_iterator_t*)first)->pos) / first->value_type->size();
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_vector_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_vector_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_vector_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_vector_reverse_iterator(dst) && __is_vector_reverse_iterator(src) && dst != src) {
        ((c_vector_iterator_t*)dst)->pos = ((c_vector_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (__is_vector_reverse_iterator(iter)) {
        ((c_vector_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (__is_vector_reverse_iterator(iter)) {
        ((c_vector_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_vector_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_vector_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_vector_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return *tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_vector_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(__is_vector_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_vector_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return *tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (__is_vector_reverse_iterator(iter)) {
        return (c_ref_t)(((c_vector_iterator_t*)iter)->pos - iter->value_type->size());
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_vector_reverse_iterator(x) || !__is_vector_reverse_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos == ((c_vector_iterator_t*)y)->pos;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static bool reverse_iter_less(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_vector_reverse_iterator(x) || !__is_vector_reverse_iterator(y)) return false;
    return ((c_vector_iterator_t*)x)->pos > ((c_vector_iterator_t*)y)->pos;
}

__c_static void reverse_iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!__is_vector_reverse_iterator(iter)) return;
    ((c_vector_iterator_t*)iter)->pos -= (n * iter->value_type->size());
}

__c_static ptrdiff_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_vector_reverse_iterator(first) || !__is_vector_reverse_iterator(last)) return 0;
    return (ptrdiff_t)(((c_vector_iterator_t*)first)->pos - ((c_vector_iterator_t*)last)->pos) / first->value_type->size();
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
    .less = iter_less,
    .advance = iter_advance,
    .distance = iter_distance
};

__c_static __c_inline c_vector_iterator_t __create_iterator(
    const c_type_info_t* value_type, c_ref_t pos)
{
    assert(value_type);
    c_vector_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_VECTOR,
            .iterator_ops = &s_iter_ops,
            .value_type = value_type
        },
        .pos = pos
    };
    return iter;
}

static c_iterator_operation_t s_reverse_ops = {
    .alloc_and_copy = reverse_iter_alloc_and_copy,
    .assign = reverse_iter_assign,
    .increment = reverse_iter_increment,
    .decrement = reverse_iter_decrement,
    .post_increment = reverse_iter_post_increment,
    .post_decrement = reverse_iter_post_decrement,
    .dereference = reverse_iter_dereference,
    .equal = reverse_iter_equal,
    .not_equal = reverse_iter_not_equal,
    .less = reverse_iter_less,
    .advance = reverse_iter_advance,
    .distance = reverse_iter_distance
};

__c_static __c_inline c_vector_iterator_t __create_reverse_iterator(
    const c_type_info_t* value_type, c_ref_t pos)
{
    assert(value_type);
    c_vector_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_VECTOR_REVERSE,
            .iterator_ops = &s_reverse_ops,
            .value_type = value_type
        },
        .pos = pos
    };
    return iter;
}

__c_static __c_inline c_ref_t __begin(c_vector_t* vector)
{
    assert(vector);
    return vector->start;
}

__c_static __c_inline c_ref_t __end(c_vector_t* vector)
{
    assert(vector);
    return vector->finish;
}

__c_static __c_inline c_storage_t __eos(c_vector_t* vector)
{
    assert(vector);
    return vector->end_of_storage;
}

__c_static __c_inline size_t __available(c_vector_t* vector)
{
    assert(vector);
    assert(vector->value_type->size);
    return (vector->end_of_storage - vector->finish) / vector->value_type->size();
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
    if (*iter == 0)
        C_ITER_COPY(iter, &first);
    else
        C_ITER_ASSIGN(*iter, &first);

    return *iter;
}

__c_static c_iterator_t* backend_end(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_iterator_t last = c_vector_end(_c->impl);
    if (*iter == 0)
        C_ITER_COPY(iter, &last);
    else
        C_ITER_ASSIGN(*iter, &last);

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

__c_static void backend_push_back(c_backend_container_t* c, c_ref_t value)
{
    if (!c || !value) return;

    c_backend_vector_t* _c = (c_backend_vector_t*)c;
    c_vector_push_back(_c->impl, value);
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
    c_backend_container_t tmp = _c->interface;
    c_vector_swap(_c->impl, _other->impl);
    _c->interface = _other->interface;
    _other->interface = tmp;
}

__c_static __c_inline void __destroy(c_vector_iterator_t first, c_vector_iterator_t last)
{
    while (C_ITER_NE(&first, &last)) {
        first.base_iter.value_type->destroy(first.pos);
        C_ITER_INC(&first);
    }
}

__c_static __c_inline void __fill(c_vector_iterator_t pos, size_t n, c_ref_t value)
{
    while (n--) {
        if (value) {
            assert(pos.base_iter.value_type->copy);
            pos.base_iter.value_type->copy(pos.pos, value);
        }
        else {
            assert(pos.base_iter.value_type->create);
            pos.base_iter.value_type->create(pos.pos);
        }
        C_ITER_INC(&pos);
    }
}

__c_static __c_inline int __reallocate_and_move(c_vector_t* vector, size_t n)
{
    assert(vector);

    size_t value_size = vector->value_type->size();

    // double the capacity or make it large enough
    size_t size = c_vector_size(vector);
    size_t cap = c_vector_capacity(vector);
    cap = ((cap * 2) < (n + size) ? (n + size) : (cap * 2));
    c_ref_t start = malloc(cap * value_size);
    if (!start) return -1;

    memcpy(start, vector->start, vector->finish - vector->start);
    __destroy(c_vector_begin(vector), c_vector_end(vector));
    __c_free(vector->start);
    vector->start = start;
    vector->finish = start + size * value_size;
    vector->end_of_storage = start + cap * value_size;

    return 0;
}

/**
 * constructor/destructor
 */
c_vector_t* c_vector_create(const c_type_info_t* value_type)
{
    if (!value_type) return 0;
    validate_type_info(value_type);

    c_vector_t* vector = (c_vector_t*)malloc(sizeof(c_vector_t));
    if (!vector) return 0;

    vector->start = 0;
    vector->finish = 0;
    vector->end_of_storage = 0;
    vector->value_type = value_type;

    return vector;
}

c_vector_t* c_vector_create_from(const c_type_info_t* value_type, c_ref_t values, size_t length)
{
    if (!value_type || !values || length == 0) return 0;

    c_vector_t* vector = c_vector_create(value_type);
    if (!vector) return 0;

    c_vector_reserve(vector, length);
    memcpy(vector->start, values, value_type->size() * length);

    return vector;
}

c_vector_t* c_vector_copy(c_vector_t* other)
{
    if (!other) return 0;

    c_vector_t* vector = c_vector_create(other->value_type);
    if (!vector) return 0;

    c_vector_reserve(vector, c_vector_capacity(other));
    memcpy(vector->start, other->start, vector->value_type->size() * c_vector_size(other));

    return vector;
}

c_vector_t* c_vector_assign(c_vector_t* self, c_vector_t* other)
{
    if (!self || !other) return self;

    if (self != other) {
        c_vector_clear(self);
        self->value_type = other->value_type;
        c_vector_reserve(self, c_vector_capacity(other));
        memcpy(self->start, other->start, self->value_type->size() * c_vector_size(other));
    }

    return self;
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
    return C_REF_T(__begin(vector) + vector->value_type->size() * pos);
}

c_ref_t c_vector_front(c_vector_t* vector)
{
    return c_vector_empty(vector) ? 0 : __begin(vector);
}

c_ref_t c_vector_back(c_vector_t* vector)
{
    if (c_vector_empty(vector)) return 0;

    return C_REF_T(__end(vector) - vector->value_type->size());
}

c_ref_t c_vector_data(c_vector_t* vector)
{
    if (!vector) return 0;
    return __begin(vector);
}

/**
 * iterators
 */
c_vector_iterator_t c_vector_begin(c_vector_t* vector)
{
    assert(vector);
    return __create_iterator(vector->value_type, __begin(vector));
}

c_vector_iterator_t c_vector_rbegin(c_vector_t* vector)
{
    assert(vector);
    return __create_reverse_iterator(vector->value_type, __end(vector));
}

c_vector_iterator_t c_vector_end(c_vector_t* vector)
{
    assert(vector);
    return __create_iterator(vector->value_type, __end(vector));
}

c_vector_iterator_t c_vector_rend(c_vector_t* vector)
{
    assert(vector);
    return __create_reverse_iterator(vector->value_type, __begin(vector));
}

/**
 * capacity
 */
bool c_vector_empty(c_vector_t* vector)
{
    return vector ? __begin(vector) == __end(vector) : true;
}

size_t c_vector_size(c_vector_t* vector)
{
    if (!vector) return 0;

    return (__end(vector) - __begin(vector)) / vector->value_type->size();
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

    return (__eos(vector) - __begin(vector)) / vector->value_type->size();
}

void c_vector_shrink_to_fit(c_vector_t* vector)
{
    if (!vector || __eos(vector) == __end(vector)) return;

    size_t size = (size_t)(vector->finish - vector->start);
    if (size == 0) {
        __c_free(vector->start);
        vector->end_of_storage = vector->finish = vector->start = 0;
        return;
    }

    c_ref_t start = malloc(size);
    if (!start) return;

    memcpy(start, vector->start, size);
    __destroy(c_vector_begin(vector), c_vector_end(vector));
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
    __destroy(c_vector_begin(vector), c_vector_end(vector));
    vector->finish = vector->start;
}

c_vector_iterator_t c_vector_insert(c_vector_t* vector, c_vector_iterator_t pos, c_ref_t value)
{
    return c_vector_insert_n(vector, pos, 1, value);
}

c_vector_iterator_t c_vector_insert_n(
    c_vector_t* vector, c_vector_iterator_t pos, size_t count, c_ref_t value)
{
    if (!vector || !value) return pos;

    if (!__is_valid_pos(vector, pos.pos)) return c_vector_end(vector);

    if (__available(vector) < count) {
        ptrdiff_t diff = pos.pos - vector->start;
        if (__reallocate_and_move(vector, count - __available(vector)))
            return c_vector_end(vector);

        pos.pos = vector->start + diff;
    }

    memmove(pos.pos + count * vector->value_type->size(), pos.pos, vector->finish - pos.pos);
    __fill(pos, count, value);
    vector->finish += count * vector->value_type->size();

    return pos;
}

c_vector_iterator_t c_vector_insert_range(
    c_vector_t* vector, c_vector_iterator_t pos, c_iterator_t first, c_iterator_t last)
{
    if (!vector) return pos;

    if (!__is_valid_pos(vector, pos.pos)) return c_vector_end(vector);

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
    if (!vector) return pos;

    if (!__is_valid_pos(vector, pos.pos) || pos.pos == vector->finish)
        return c_vector_end(vector);

    vector->value_type->destroy(pos.pos);
    c_ref_t next_pos = pos.pos + vector->value_type->size();
    memmove(pos.pos, next_pos, vector->finish - next_pos);
    vector->finish -= vector->value_type->size();

    return pos;
}

c_vector_iterator_t c_vector_erase_range(
    c_vector_t* vector, c_vector_iterator_t first, c_vector_iterator_t last)
{
    if (!vector) return last;

    if (!__is_valid_pos(vector, first.pos) ||
        !__is_valid_pos(vector, last.pos) ||
        (first.pos > last.pos))
        return c_vector_end(vector);

    if (first.pos == last.pos) return last;

    __destroy(first, last);
    size_t size = vector->finish - last.pos;
    memmove(first.pos, last.pos, size);
    vector->finish -= (last.pos - first.pos);

    return first;
}

void c_vector_push_back(c_vector_t* vector, c_ref_t value)
{
    if (!vector || !value) return;

    if (vector->finish == vector->end_of_storage) {
        if (__reallocate_and_move(vector, 1))
            return;
    }

    vector->value_type->copy(vector->finish, value);
    vector->finish += vector->value_type->size();
}

void c_vector_pop_back(c_vector_t* vector)
{
    if (!c_vector_empty(vector)) {
        vector->value_type->destroy(c_vector_back(vector));
        vector->finish -= vector->value_type->size();
    }
}

void c_vector_resize(c_vector_t* vector, size_t count)
{
    c_vector_resize_with_value(vector, count, 0);
}

void c_vector_resize_with_value(c_vector_t* vector, size_t count, c_ref_t value)
{
    if (!vector) return;

    size_t value_size = vector->value_type->size();

    if (count > c_vector_size(vector)) {
        count -= c_vector_size(vector);
        if (count > __available(vector)) {
            if (__reallocate_and_move(vector, count))
                return;
        }
        __fill(c_vector_end(vector), count, value);
        vector->finish += count * value_size;
    }
    else {
        c_ref_t pos = vector->start + count * value_size;
        __destroy(__create_iterator(vector->value_type, pos), c_vector_end(vector));
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
c_backend_container_t* c_vector_create_backend(const c_type_info_t* value_type)
{
    static c_backend_operation_t backend_vector_ops = {
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

    backend->impl = c_vector_create(value_type);
    if (!backend->impl) {
        __c_free(backend);
        return 0;
    }

    backend->interface.ops = &backend_vector_ops;

    return (c_backend_container_t*)backend;
}
