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
#include "c_algorithm.h"
#include "c_deque.h"

struct __c_deque {
    c_storage_t start_of_storage;
    c_ref_t start;
    c_ref_t finish;
    c_storage_t end_of_storage;
    c_containable_t* type_info;
};

struct __c_backend_deque {
    c_backend_container_t interface;
    c_deque_t* impl;
};

__c_static bool is_valid_pos(c_deque_t* deque, c_ref_t pos)
{
    if (!deque) return false;

    if (pos >= deque->start && pos <= deque->finish) {
        ptrdiff_t diff = pos - deque->start;
        if (diff % deque->type_info->size() == 0)
            return true;
    }

    return false;
}

__c_static __c_inline bool is_deque_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_DEQUE);
}

__c_static __c_inline bool is_deque_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_DEQUE_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_deque_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_deque_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_deque_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (is_deque_iterator(dst) && is_deque_iterator(src) && dst != src) {
        ((c_deque_iterator_t*)dst)->pos = ((c_deque_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (is_deque_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (is_deque_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_deque_iterator(iter) && is_deque_iterator(tmp)) {
        ((c_deque_iterator_t*)tmp)->pos = ((c_deque_iterator_t*)iter)->pos;
        ((c_deque_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_deque_iterator(iter)) {
        ((c_deque_iterator_t*)tmp)->pos = ((c_deque_iterator_t*)iter)->pos;
        ((c_deque_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (is_deque_iterator(iter)) {
        return ((c_deque_iterator_t*)iter)->pos;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_deque_iterator(x) || !is_deque_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos == ((c_deque_iterator_t*)y)->pos;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!is_deque_iterator(iter)) return;
    ((c_deque_iterator_t*)iter)->pos += (n * iter->type_info->size());
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_deque_iterator(first) || !is_deque_iterator(last)) return 0;
    return (ptrdiff_t)(((c_deque_iterator_t*)last)->pos - ((c_deque_iterator_t*)first)->pos) / first->type_info->size();
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_deque_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_deque_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_deque_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (is_deque_reverse_iterator(dst) && is_deque_reverse_iterator(src) && dst != src) {
        ((c_deque_iterator_t*)dst)->pos = ((c_deque_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)tmp)->pos = ((c_deque_iterator_t*)iter)->pos;
        ((c_deque_iterator_t*)iter)->pos -= iter->type_info->size();
    }
    return tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t* tmp)
{
    if (is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)tmp)->pos = ((c_deque_iterator_t*)iter)->pos;
        ((c_deque_iterator_t*)iter)->pos += iter->type_info->size();
    }
    return tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (is_deque_reverse_iterator(iter)) {
        return (c_ref_t)(((c_deque_iterator_t*)iter)->pos - iter->type_info->size());
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_deque_reverse_iterator(x) || !is_deque_reverse_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos == ((c_deque_iterator_t*)y)->pos;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static void reverse_iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!is_deque_reverse_iterator(iter)) return;
    ((c_deque_iterator_t*)iter)->pos -= (n * iter->type_info->size());
}

__c_static ptrdiff_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_deque_reverse_iterator(first) || !is_deque_reverse_iterator(last)) return 0;
    return (ptrdiff_t)(((c_deque_iterator_t*)first)->pos - ((c_deque_iterator_t*)last)->pos) / first->type_info->size();
}

__c_static c_deque_iterator_t create_iterator(
    c_containable_t* type_info, c_ref_t pos)
{
    assert(type_info);

    static c_iterator_operation_t ops = {
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

    c_deque_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_DEQUE,
            .iterator_ops = &ops,
            .type_info = type_info
        },
        .pos = pos
    };
    return iter;
}

__c_static c_deque_iterator_t create_reverse_iterator(
    c_containable_t* type_info, c_ref_t pos)
{
    assert(type_info);

    static c_iterator_operation_t ops = {
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

    c_deque_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_DEQUE_REVERSE,
            .iterator_ops = &ops,
            .type_info = type_info
        },
        .pos = pos
    };
    return iter;
}

__c_static __c_inline c_ref_t begin(c_deque_t* deque)
{
    assert(deque);
    return deque->start;
}

__c_static __c_inline c_ref_t end(c_deque_t* deque)
{
    assert(deque);
    return deque->finish;
}

__c_static __c_inline c_ref_t sos(c_deque_t* deque)
{
    assert(deque);
    return deque->start_of_storage;
}

__c_static __c_inline c_ref_t eos(c_deque_t* deque)
{
    assert(deque);
    return deque->end_of_storage;
}

__c_static __c_inline size_t available_start(c_deque_t* deque)
{
    assert(deque);
    assert(deque->type_info->size);
    return (deque->start_of_storage - deque->start) / deque->type_info->size();
}

__c_static __c_inline size_t available_end(c_deque_t* deque)
{
    assert(deque);
    assert(deque->type_info->size);
    return (deque->end_of_storage - deque->finish) / deque->type_info->size();
}

__c_static __c_inline size_t available(c_deque_t* deque)
{
    return available_start(deque) + available_end(deque);
}

__c_static __c_inline size_t capacity(c_deque_t* deque)
{
    assert(deque);
    assert(deque->type_info->size);
    return (eos(deque) - sos(deque)) / deque->type_info->size();
}

__c_static __c_inline void reserve(c_deque_t* deque, size_t new_cap)
{
    assert(deque);
    if (new_cap > capacity(deque)) c_deque_resize(deque, new_cap);
}

__c_static void backend_destroy(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_destroy(_c->impl);
    __c_free(_c);
}

__c_static c_ref_t backend_front(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    return c_deque_front(_c->impl);
}

__c_static c_ref_t backend_back(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    return c_deque_back(_c->impl);
}

__c_static c_iterator_t* backend_begin(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_iterator_t first = c_deque_begin(_c->impl);
    if (*iter == 0)
        C_ITER_COPY(iter, &first);
    else
        C_ITER_ASSIGN(*iter, &first);

    return *iter;
}

__c_static c_iterator_t* backend_end(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_iterator_t last = c_deque_end(_c->impl);
    if (*iter == 0)
        C_ITER_COPY(iter, &last);
    else
        C_ITER_ASSIGN(*iter, &last);

    return *iter;
}

__c_static bool backend_empty(c_backend_container_t* c)
{
    if (!c) return true;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    return c_deque_empty(_c->impl);
}

__c_static size_t backend_size(c_backend_container_t* c)
{
    if (!c) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    return c_deque_size(_c->impl);
}

__c_static size_t backend_max_size(void)
{
    return (-1);
}

__c_static void backend_push_back(c_backend_container_t* c, c_ref_t data)
{
    if (!c || !data) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_push_back(_c->impl, data);
}

__c_static void backend_pop_back(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_pop_back(_c->impl);
}

__c_static void backend_push_front(c_backend_container_t* c, c_ref_t data)
{
    if (!c || !data) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_push_front(_c->impl, data);
}

__c_static void backend_pop_front(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_pop_front(_c->impl);
}

__c_static void backend_swap(c_backend_container_t* c, c_backend_container_t* other)
{
    if (!c || !other) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_backend_deque_t* _other = (c_backend_deque_t*)other;
    c_backend_container_t tmp = _c->interface;
    c_deque_swap(_c->impl, _other->impl);
    _c->interface = _other->interface;
    _other->interface = tmp;
}

__c_static void destroy(c_deque_iterator_t first, c_deque_iterator_t last)
{
    while (C_ITER_NE(&first, &last)) {
        first.base_iter.type_info->destroy(first.pos);
        C_ITER_INC(&first);
    }
}

__c_static c_deque_iterator_t fill(c_deque_iterator_t pos, size_t n, c_ref_t data)
{
    while (n--) {
        if (data) {
            assert(pos.base_iter.type_info->copy);
            pos.base_iter.type_info->copy(pos.pos, data);
        }
        else {
            assert(pos.base_iter.type_info->create);
            pos.base_iter.type_info->create(pos.pos);
        }
        C_ITER_INC(&pos);
    }

    return pos;
}

__c_static c_iterator_t insert_aux(c_deque_t* deque, c_deque_iterator_t pos, size_t n, c_ref_t data)
{
    assert(deque);

    c_containable_t* type_info = deque->type_info;
    size_t data_size = type_info->size();
    size_t shift_size = 0;

    if (n <= available_end(deque)) {
        shift_size = n * data_size;
        memmove(pos.pos + shift_size, pos.pos, deque->finish - pos.pos);
        fill(pos, n, data);
        deque->finish += shift_size;
    }
    else if (n <= available_start(deque)) {
        shift_size = n * data_size;
        memmove(deque->start - shift_size, deque->start, pos.pos - deque->start);
        deque->start -= shift_size;
        fill(create_iterator(type_info, pos.pos - shift_size), n, data);
    }
    else {
        size_t first_half_size = pos.pos - deque->start;
        size_t second_half_size = deque->finish - pos.pos;
        memmove(deque->start_of_storage, deque->start, first_half_size);
        deque->start = deque->start_of_storage;

        memmove(deque->start + first_half_size + n * data_size, pos.pos, second_half_size);
        fill(create_iterator(type_info, deque->start + first_half_size), n, data);

        deque->finish = deque->start + first_half_size + n * data_size + second_half_size;
    }
}

__c_static int reallocate_and_move(c_deque_t* deque, size_t n)
{
    assert(deque);

    size_t data_size = deque->type_info->size();

    // double the capacity or make it large enough
    size_t size = c_deque_size(deque);
    size_t cap = capacity(deque);
    cap = ((cap * 2) < (n + size) ? (n + size) : (cap * 2));
    c_storage_t start_of_storage = malloc(cap * data_size);
    if (!start_of_storage) return -1;

    c_ref_t start = start_of_storage + (cap - size) / 2 * data_size;
    memcpy(start, deque->start, deque->finish - deque->start);
    destroy(c_deque_begin(deque), c_deque_end(deque));
    __c_free(deque->start_of_storage);
    deque->start_of_storage = start_of_storage;
    deque->start = start;
    deque->finish = start + size * data_size;
    deque->end_of_storage = start_of_storage + cap * data_size;

    return 0;
}

/**
 * constructor/destructor
 */
c_deque_t* c_deque_create(c_containable_t* type_info)
{
    if (!type_info) return 0;

    c_deque_t* deque = (c_deque_t*)malloc(sizeof(c_deque_t));
    if (!deque) return 0;

    deque->start_of_storage = 0;
    deque->start = 0;
    deque->finish = 0;
    deque->end_of_storage = 0;
    deque->type_info = type_info;

    return deque;
}

c_deque_t* c_deque_create_from(c_containable_t* type_info, c_ref_t datas, size_t length)
{
    if (!type_info || !datas || length == 0) return 0;

    c_deque_t* deque = c_deque_create(type_info);
    if (!deque) return 0;

    reserve(deque, length);
    memcpy(deque->start, datas, type_info->size() * length);

    return deque;
}

c_deque_t* c_deque_copy(c_deque_t* other)
{
    if (!other) return 0;

    c_deque_t* deque = c_deque_create(other->type_info);
    if (!deque) return 0;

    reserve(deque, capacity(other));
    memcpy(deque->start, other->start, deque->type_info->size() * c_deque_size(other));

    return deque;
}

c_deque_t* c_deque_assign(c_deque_t* self, c_deque_t* other)
{
    if (!self || !other) return self;

    if (self != other) {
        c_deque_clear(self);
        self->type_info = other->type_info;
        reserve(self, capacity(other));
        memcpy(self->start, other->start, self->type_info->size() * c_deque_size(other));
    }

    return self;
}

void c_deque_destroy(c_deque_t* deque)
{
    if (!deque) return;

    c_deque_clear(deque);
    __c_free(deque->start_of_storage);
    __c_free(deque);
}

/**
 * element access
 */
c_ref_t c_deque_at(c_deque_t* deque, size_t pos)
{
    if (!deque) return 0;
    return C_REF_T(begin(deque) + deque->type_info->size() * pos);
}

c_ref_t c_deque_front(c_deque_t* deque)
{
    return c_deque_empty(deque) ? 0 : begin(deque);
}

c_ref_t c_deque_back(c_deque_t* deque)
{
    if (c_deque_empty(deque)) return 0;

    return C_REF_T(end(deque) - deque->type_info->size());
}

/**
 * iterators
 */
c_deque_iterator_t c_deque_begin(c_deque_t* deque)
{
    assert(deque);
    return create_iterator(deque->type_info, begin(deque));
}

c_deque_iterator_t c_deque_rbegin(c_deque_t* deque)
{
    assert(deque);
    return create_reverse_iterator(deque->type_info, end(deque));
}

c_deque_iterator_t c_deque_end(c_deque_t* deque)
{
    assert(deque);
    return create_iterator(deque->type_info, end(deque));
}

c_deque_iterator_t c_deque_rend(c_deque_t* deque)
{
    assert(deque);
    return create_reverse_iterator(deque->type_info, begin(deque));
}

/**
 * capacity
 */
bool c_deque_empty(c_deque_t* deque)
{
    return deque ? begin(deque) == end(deque) : true;
}

size_t c_deque_size(c_deque_t* deque)
{
    if (!deque) return 0;

    return (end(deque) - begin(deque)) / deque->type_info->size();
}

size_t c_deque_max_size(void)
{
    return (-1);
}

void c_deque_shrink_to_fit(c_deque_t* deque)
{
    if (!deque || (eos(deque) == end(deque) && sos(deque) == begin(deque))) return;

    size_t size = (size_t)(deque->finish - deque->start);
    if (size == 0) {
        __c_free(deque->start_of_storage);
        deque->start_of_storage = 0;
        deque->start = 0;
        deque->finish = 0;
        deque->end_of_storage = 0;
        return;
    }

    c_storage_t start_of_storage = malloc(size);
    if (!start_of_storage) return;

    memcpy(start_of_storage, deque->start, size);
    destroy(c_deque_begin(deque), c_deque_end(deque));
    __c_free(deque->start_of_storage);
    deque->start_of_storage = start_of_storage;
    deque->start = deque->start_of_storage;
    deque->finish = deque->start + size;
    deque->end_of_storage = deque->finish;
}

/**
 * modifiers
 */
void c_deque_clear(c_deque_t* deque)
{
    if (c_deque_empty(deque)) return;

    destroy(c_deque_begin(deque), c_deque_end(deque));
    size_t data_size = deque->type_info->size();
    size_t cap = (eos(deque) - sos(deque)) / data_size;
    deque->start = sos(deque) + (cap / 2) * data_size;
    deque->finish = deque->start;
}

c_deque_iterator_t c_deque_insert(c_deque_t* deque, c_deque_iterator_t pos, c_ref_t data)
{
    return c_deque_insert_n(deque, pos, 1, data);
}

c_deque_iterator_t c_deque_insert_n(
    c_deque_t* deque, c_deque_iterator_t pos, size_t count, c_ref_t data)
{
    if (!deque || !data) return pos;

    if (!is_valid_pos(deque, pos.pos)) return c_deque_end(deque);

    if (available(deque) < count) {
        ptrdiff_t diff = pos.pos - deque->start;
        if (reallocate_and_move(deque, (count - available(deque)) * 2))
            return c_deque_end(deque);

        pos.pos = deque->start + diff;
    }

    insert_aux(deque, pos, count, data);

    return pos;
}

c_deque_iterator_t c_deque_insert_range(
    c_deque_t* deque, c_deque_iterator_t pos, c_iterator_t first, c_iterator_t last)
{
    if (!deque) return pos;

    if (!is_valid_pos(deque, pos.pos)) return c_deque_end(deque);

    while (C_ITER_NE(&first, &last)) {
        pos = c_deque_insert(deque, pos, C_ITER_DEREF(&first));
        if (pos.pos == deque->finish)
            return pos;

        C_ITER_INC(&pos);
        C_ITER_INC(&first);
    }

    return pos;
}

c_deque_iterator_t c_deque_erase(c_deque_t* deque, c_deque_iterator_t pos)
{
    if (!deque) return pos;

    if (!is_valid_pos(deque, pos.pos) || pos.pos == deque->finish)
        return c_deque_end(deque);

    c_ref_t next_pos = pos.pos + deque->type_info->size();
    deque->type_info->destroy(pos.pos);
    memmove(pos.pos, next_pos, deque->finish - next_pos);
    deque->finish -= deque->type_info->size();

    return pos;
}

c_deque_iterator_t c_deque_erase_range(
    c_deque_t* deque, c_deque_iterator_t first, c_deque_iterator_t last)
{
    if (!deque) return last;

    if (!is_valid_pos(deque, first.pos) ||
        !is_valid_pos(deque, last.pos) ||
        (first.pos > last.pos))
        return c_deque_end(deque);

    if (first.pos == last.pos) return last;

    destroy(first, last);
    size_t size = deque->finish - last.pos;
    memmove(first.pos, last.pos, size);
    deque->finish -= (last.pos - first.pos);

    return first;
}

void c_deque_push_back(c_deque_t* deque, c_ref_t data)
{
    if (!deque || !data) return;

    if (deque->finish == deque->end_of_storage) {
        if (reallocate_and_move(deque, 1 * 2))
            return;
    }

    deque->type_info->copy(deque->finish, data);
    deque->finish += deque->type_info->size();
}

void c_deque_pop_back(c_deque_t* deque)
{
    if (!c_deque_empty(deque)) {
        deque->type_info->destroy(c_deque_back(deque));
        deque->finish -= deque->type_info->size();
    }
}

void c_deque_push_front(c_deque_t* deque, c_ref_t data)
{
    if (!deque || !data) return;

    if (deque->start == deque->start_of_storage) {
        if (reallocate_and_move(deque, 1 * 2))
            return;
    }

    deque->start -= deque->type_info->size();
    deque->type_info->copy(deque->start, data);
}

void c_deque_pop_front(c_deque_t* deque)
{
    if (!c_deque_empty(deque)) {
        deque->type_info->destroy(c_deque_front(deque));
        deque->start += deque->type_info->size();
    }
}

void c_deque_resize(c_deque_t* deque, size_t count)
{
    c_deque_resize_with_value(deque, count, 0);
}

void c_deque_resize_with_value(c_deque_t* deque, size_t count, c_ref_t data)
{
    if (!deque) return;

    size_t data_size = deque->type_info->size();

    if (count > c_deque_size(deque)) {
        count -= c_deque_size(deque);
        if (count > available(deque)) {
            if (reallocate_and_move(deque, count * 2)) // make sure the end available part is large enough
                return;
        }
/*
        if (count <= available_end(deque)) {
            fill(c_deque_end(deque), count, data);
            deque->finish += count * data_size;
        }
        else if (count <= available_start(deque)) {
            size_t shift_size = count * data_size;
            memmove(deque->start - shift_size, deque->start, deque->finish - deque->start);
            deque->start -= shift_size;
            fill(create_iterator(deque->type_info, deque->finish - shift_size), count, data);
        }
        else {
            size_t shift_size = available_start(deque);
            memmove(deque->start_of_storage, deque->start, deque->finish - deque->start);
            deque->start = deque->start_of_storage;
            deque->finish -= shift_size;
            fill(create_iterator(deque->type_info, deque->finish), count, data);
            deque->finish += count * data_size;
        }
         */
        insert_aux(deque, c_deque_end(deque), count, data);
    }
    else {
        c_ref_t pos = deque->start + count * data_size;
        destroy(create_iterator(deque->type_info, pos), c_deque_end(deque));
        deque->finish = pos;
    }
}

void c_deque_swap(c_deque_t* deque, c_deque_t* other)
{
    if (!deque || !other) return;

    c_deque_t tmp = *deque;
    *deque = *other;
    *other = tmp;
}

/**
 * backend
 */
c_backend_container_t* c_deque_create_backend(c_containable_t* type_info)
{
    static c_backend_operation_t backend_deque_ops = {
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

    c_backend_deque_t* backend = (c_backend_deque_t*)malloc(sizeof(c_backend_deque_t));
    if (!backend) return 0;

    backend->impl = c_deque_create(type_info);
    if (!backend->impl) {
        __c_free(backend);
        return 0;
    }

    backend->interface.ops = &backend_deque_ops;

    return (c_backend_container_t*)backend;
}
