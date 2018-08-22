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
#include "c_deque.h"

struct __c_deque {
    c_storage_t start_of_storage;
    c_ref_t start;
    c_ref_t finish;
    c_storage_t end_of_storage;
    const c_type_info_t* value_type;
};

struct __c_backend_deque {
    c_backend_container_t interface;
    c_deque_t* impl;
};

__c_static __c_inline bool __check_deque_state(c_deque_t* deque)
{
#ifdef ENABLE_CHECK_DEQUE_STATE
    return ((deque) &&
            (deque->end_of_storage >= deque->finish) &&
            (deque->finish >= deque->start) &&
            (deque->start >= deque->start_of_storage));
#else
    __c_unuse(deque);
    return true;
#endif /* ENABLE_CHECK_DEQUE_STATE */
}

__c_static __c_inline bool __is_valid_pos(c_deque_t* deque, c_ref_t pos)
{
    if (!deque) return false;

    if (pos >= deque->start && pos <= deque->finish) {
        ptrdiff_t diff = pos - deque->start;
        if (diff % deque->value_type->size() == 0) return true;
    }

    return false;
}

__c_static __c_inline bool __is_deque_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_DEQUE);
}

__c_static __c_inline bool __is_deque_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_RANDOM &&
            iter->iterator_type == C_ITER_TYPE_DEQUE_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_deque_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_deque_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_deque_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_deque_iterator(dst) && __is_deque_iterator(src) && dst != src) {
        ((c_deque_iterator_t*)dst)->pos = ((c_deque_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (__is_deque_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (__is_deque_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_deque_iterator(iter)) {
        if (*tmp == 0) {
            iter_alloc_and_copy(tmp, iter);
        }
        else {
            assert(__is_deque_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_deque_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return *tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_deque_iterator(iter)) {
        if (*tmp == 0) {
            iter_alloc_and_copy(tmp, iter);
        }
        else {
            assert(__is_deque_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        ((c_deque_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return *tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (__is_deque_iterator(iter)) {
        return ((c_deque_iterator_t*)iter)->pos;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_deque_iterator(x) || !__is_deque_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos == ((c_deque_iterator_t*)y)->pos;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static bool iter_less(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_deque_iterator(x) || !__is_deque_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos < ((c_deque_iterator_t*)y)->pos;
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!__is_deque_iterator(iter) || n == 0) return;
    ((c_deque_iterator_t*)iter)->pos += (n * iter->value_type->size());
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_deque_iterator(first) || !__is_deque_iterator(last)) return 0;
    return (ptrdiff_t)(((c_deque_iterator_t*)last)->pos - ((c_deque_iterator_t*)first)->pos) / first->value_type->size();
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && __is_deque_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_deque_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_deque_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (__is_deque_reverse_iterator(dst) && __is_deque_reverse_iterator(src) && dst != src) {
        ((c_deque_iterator_t*)dst)->pos = ((c_deque_iterator_t*)src)->pos;
    }
    return dst;
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (__is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (__is_deque_reverse_iterator(iter)) {
        ((c_deque_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_deque_reverse_iterator(iter)) {
        if (*tmp == 0) {
            reverse_iter_alloc_and_copy(tmp, iter);
        }
        else {
            assert(__is_deque_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_deque_iterator_t*)iter)->pos -= iter->value_type->size();
    }
    return *tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (__is_deque_reverse_iterator(iter)) {
        if (*tmp == 0) {
            reverse_iter_alloc_and_copy(tmp, iter);
        }
        else {
            assert(__is_deque_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        ((c_deque_iterator_t*)iter)->pos += iter->value_type->size();
    }
    return *tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (__is_deque_reverse_iterator(iter)) {
        return (c_ref_t)(((c_deque_iterator_t*)iter)->pos - iter->value_type->size());
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_deque_reverse_iterator(x) || !__is_deque_reverse_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos == ((c_deque_iterator_t*)y)->pos;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static bool reverse_iter_less(c_iterator_t* x, c_iterator_t* y)
{
    if (!__is_deque_reverse_iterator(x) || !__is_deque_reverse_iterator(y)) return false;
    return ((c_deque_iterator_t*)x)->pos > ((c_deque_iterator_t*)y)->pos;
}

__c_static void reverse_iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (!__is_deque_reverse_iterator(iter)) return;
    ((c_deque_iterator_t*)iter)->pos -= (n * iter->value_type->size());
}

__c_static ptrdiff_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!__is_deque_reverse_iterator(first) || !__is_deque_reverse_iterator(last)) return 0;
    return (ptrdiff_t)(((c_deque_iterator_t*)first)->pos - ((c_deque_iterator_t*)last)->pos) / first->value_type->size();
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

__c_static __c_inline c_deque_iterator_t __create_iterator(
    const c_type_info_t* value_type, c_ref_t pos)
{
    assert(value_type);

    c_deque_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_DEQUE,
            .iterator_ops = &s_iter_ops,
            .value_type = value_type
        },
        .pos = pos
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
    .less = reverse_iter_less,
    .advance = reverse_iter_advance,
    .distance = reverse_iter_distance
};

__c_static __c_inline c_deque_iterator_t __create_reverse_iterator(
    const c_type_info_t* value_type, c_ref_t pos)
{
    assert(value_type);
    c_deque_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_RANDOM,
            .iterator_type = C_ITER_TYPE_DEQUE_REVERSE,
            .iterator_ops = &s_reverse_iter_ops,
            .value_type = value_type
        },
        .pos = pos
    };
    return iter;
}

__c_static __c_inline c_ref_t __begin(c_deque_t* deque)
{
    assert(deque);
    return deque->start;
}

__c_static __c_inline c_ref_t __end(c_deque_t* deque)
{
    assert(deque);
    return deque->finish;
}

__c_static __c_inline c_ref_t __sos(c_deque_t* deque)
{
    assert(deque);
    return deque->start_of_storage;
}

__c_static __c_inline c_ref_t __eos(c_deque_t* deque)
{
    assert(deque);
    return deque->end_of_storage;
}

__c_static __c_inline size_t __available_start(c_deque_t* deque)
{
    assert(deque);
    assert(deque->value_type->size);
    return (deque->start - deque->start_of_storage) / deque->value_type->size();
}

__c_static __c_inline size_t __available_end(c_deque_t* deque)
{
    assert(deque);
    assert(deque->value_type->size);
    return (deque->end_of_storage - deque->finish) / deque->value_type->size();
}

__c_static __c_inline size_t __available(c_deque_t* deque)
{
    return __available_start(deque) + __available_end(deque);
}

__c_static __c_inline size_t __capacity(c_deque_t* deque)
{
    assert(deque);
    assert(deque->value_type->size);
    return (__eos(deque) - __sos(deque)) / deque->value_type->size();
}

__c_static __c_inline void __reserve(c_deque_t* deque, size_t new_cap)
{
    assert(deque);
    if (new_cap > __capacity(deque)) c_deque_resize(deque, new_cap);
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
    if (*iter == 0) {
        C_ITER_COPY(iter, &first);
    }
    else {
        C_ITER_ASSIGN(*iter, &first);
    }

    return *iter;
}

__c_static c_iterator_t* backend_end(c_backend_container_t* c, c_iterator_t** iter)
{
    if (!c || !iter) return 0;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_iterator_t last = c_deque_end(_c->impl);
    if (*iter == 0) {
        C_ITER_COPY(iter, &last);
    }
    else {
        C_ITER_ASSIGN(*iter, &last);
    }

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

__c_static void backend_push_back(c_backend_container_t* c, c_ref_t value)
{
    if (!c || !value) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_push_back(_c->impl, value);
}

__c_static void backend_pop_back(c_backend_container_t* c)
{
    if (!c) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_pop_back(_c->impl);
}

__c_static void backend_push_front(c_backend_container_t* c, c_ref_t value)
{
    if (!c || !value) return;

    c_backend_deque_t* _c = (c_backend_deque_t*)c;
    c_deque_push_front(_c->impl, value);
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

__c_static __c_inline void __destroy(c_deque_iterator_t first, c_deque_iterator_t last)
{
    while (C_ITER_NE(&first, &last)) {
        first.base_iter.value_type->destroy(first.pos);
        C_ITER_INC(&first);
    }
}

__c_static __c_inline c_deque_iterator_t __fill(
    c_deque_iterator_t pos, size_t n, c_ref_t value)
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

    return pos;
}

__c_static __c_inline c_deque_iterator_t __insert_aux(
    c_deque_t* deque, c_deque_iterator_t pos, size_t n, c_ref_t value)
{
    assert(deque);

    const c_type_info_t* value_type = deque->value_type;
    size_t value_size = value_type->size();
    size_t shift_size = 0;

    if (n <= __available_end(deque)) {
        shift_size = n * value_size;
        memmove(pos.pos + shift_size, pos.pos, deque->finish - pos.pos);
        __fill(pos, n, value);
        deque->finish += shift_size;
        assert(__check_deque_state(deque));
    }
    else if (n <= __available_start(deque)) {
        shift_size = n * value_size;
        memmove(deque->start - shift_size, deque->start, pos.pos - deque->start);
        deque->start -= shift_size;
        pos.pos -= shift_size;
        __fill(pos, n, value);
        assert(__check_deque_state(deque));
    }
    else {
        size_t first_half_size = pos.pos - deque->start;
        size_t second_half_size = deque->finish - pos.pos;
        memmove(deque->start_of_storage, deque->start, first_half_size);
        deque->start = deque->start_of_storage;

        memmove(deque->start + first_half_size + n * value_size, pos.pos, second_half_size);

        pos.pos = deque->start + first_half_size;
        __fill(pos, n, value);

        deque->finish = pos.pos + n * value_size + second_half_size;
        assert(__check_deque_state(deque));
    }

    return pos;
}

__c_static __c_inline int __reallocate_and_move(c_deque_t* deque, size_t n)
{
    assert(deque);

    size_t value_size = deque->value_type->size();

    // double the capacity or make it large enough
    size_t size = c_deque_size(deque);
    size_t cap = __capacity(deque);
    cap = ((cap * 2) < (n + size) ? (n + size) : (cap * 2));
    c_storage_t start_of_storage = malloc(cap * value_size);
    if (!start_of_storage) return -1;

    c_ref_t start = start_of_storage + (cap - size) / 2 * value_size;
    memcpy(start, deque->start, deque->finish - deque->start);
    __destroy(c_deque_begin(deque), c_deque_end(deque));
    __c_free(deque->start_of_storage);
    deque->start_of_storage = start_of_storage;
    deque->start = start;
    deque->finish = start + size * value_size;
    deque->end_of_storage = start_of_storage + cap * value_size;
    assert(__check_deque_state(deque));

    return 0;
}

/**
 * constructor/destructor
 */
c_deque_t* c_deque_create(const c_type_info_t* value_type)
{
    if (!value_type) return 0;
    validate_type_info(value_type);

    c_deque_t* deque = (c_deque_t*)malloc(sizeof(c_deque_t));
    if (!deque) return 0;

    deque->start_of_storage = 0;
    deque->start = 0;
    deque->finish = 0;
    deque->end_of_storage = 0;
    deque->value_type = value_type;

    return deque;
}

c_deque_t* c_deque_create_from(const c_type_info_t* value_type, c_ref_t values, size_t length)
{
    if (!value_type || !values || length == 0) return 0;

    c_deque_t* deque = c_deque_create(value_type);
    if (!deque) return 0;

    __reserve(deque, length);
    memcpy(deque->start, values, value_type->size() * length);

    return deque;
}

c_deque_t* c_deque_copy(c_deque_t* other)
{
    if (!other) return 0;

    c_deque_t* deque = c_deque_create(other->value_type);
    if (!deque) return 0;

    __reserve(deque, __capacity(other));
    memcpy(deque->start, other->start, deque->value_type->size() * c_deque_size(other));

    return deque;
}

c_deque_t* c_deque_assign(c_deque_t* self, c_deque_t* other)
{
    if (!self || !other) return self;

    if (self != other) {
        c_deque_clear(self);
        self->value_type = other->value_type;
        __reserve(self, __capacity(other));
        memcpy(self->start, other->start, self->value_type->size() * c_deque_size(other));
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
    return C_REF_T(__begin(deque) + deque->value_type->size() * pos);
}

c_ref_t c_deque_front(c_deque_t* deque)
{
    return c_deque_empty(deque) ? 0 : __begin(deque);
}

c_ref_t c_deque_back(c_deque_t* deque)
{
    if (c_deque_empty(deque)) return 0;

    return C_REF_T(__end(deque) - deque->value_type->size());
}

/**
 * iterators
 */
c_deque_iterator_t c_deque_begin(c_deque_t* deque)
{
    assert(deque);
    return __create_iterator(deque->value_type, __begin(deque));
}

c_deque_iterator_t c_deque_rbegin(c_deque_t* deque)
{
    assert(deque);
    return __create_reverse_iterator(deque->value_type, __end(deque));
}

c_deque_iterator_t c_deque_end(c_deque_t* deque)
{
    assert(deque);
    return __create_iterator(deque->value_type, __end(deque));
}

c_deque_iterator_t c_deque_rend(c_deque_t* deque)
{
    assert(deque);
    return __create_reverse_iterator(deque->value_type, __begin(deque));
}

/**
 * capacity
 */
bool c_deque_empty(c_deque_t* deque)
{
    return deque ? __begin(deque) == __end(deque) : true;
}

size_t c_deque_size(c_deque_t* deque)
{
    if (!deque) return 0;

    return (__end(deque) - __begin(deque)) / deque->value_type->size();
}

size_t c_deque_max_size(void)
{
    return (-1);
}

void c_deque_shrink_to_fit(c_deque_t* deque)
{
    if (!deque || (__eos(deque) == __end(deque) && __sos(deque) == __begin(deque))) return;

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
    __destroy(c_deque_begin(deque), c_deque_end(deque));
    __c_free(deque->start_of_storage);
    deque->start_of_storage = start_of_storage;
    deque->start = deque->start_of_storage;
    deque->finish = deque->start + size;
    deque->end_of_storage = deque->finish;
    assert(__check_deque_state(deque));
}

/**
 * modifiers
 */
void c_deque_clear(c_deque_t* deque)
{
    if (c_deque_empty(deque)) return;

    __destroy(c_deque_begin(deque), c_deque_end(deque));
    size_t value_size = deque->value_type->size();
    size_t cap = (__eos(deque) - __sos(deque)) / value_size;
    deque->start = __sos(deque) + (cap / 2) * value_size;
    deque->finish = deque->start;
    assert(__check_deque_state(deque));
}

c_deque_iterator_t c_deque_insert(c_deque_t* deque, c_deque_iterator_t pos, c_ref_t value)
{
    return c_deque_insert_n(deque, pos, 1, value);
}

c_deque_iterator_t c_deque_insert_n(
    c_deque_t* deque, c_deque_iterator_t pos, size_t count, c_ref_t value)
{
    if (!deque || !value) return pos;

    if (!__is_valid_pos(deque, pos.pos)) return c_deque_end(deque);

    if (__available(deque) < count) {
        ptrdiff_t diff = pos.pos - deque->start;
        if (__reallocate_and_move(deque, (count - __available(deque)) * 2)) {
            return c_deque_end(deque);
        }

        pos.pos = deque->start + diff;
    }

    return __insert_aux(deque, pos, count, value);
}

c_deque_iterator_t c_deque_insert_range(
    c_deque_t* deque, c_deque_iterator_t pos, c_iterator_t first, c_iterator_t last)
{
    if (!deque) return pos;

    if (!__is_valid_pos(deque, pos.pos)) return c_deque_end(deque);

    while (C_ITER_NE(&first, &last)) {
        pos = c_deque_insert(deque, pos, C_ITER_DEREF(&first));
        if (pos.pos == deque->finish) return pos;

        C_ITER_INC(&pos);
        C_ITER_INC(&first);
    }

    return pos;
}

c_deque_iterator_t c_deque_erase(c_deque_t* deque, c_deque_iterator_t pos)
{
    if (!deque) return pos;

    if (!__is_valid_pos(deque, pos.pos) || pos.pos == deque->finish) {
        return c_deque_end(deque);
    }

    c_ref_t next_pos = pos.pos + deque->value_type->size();
    deque->value_type->destroy(pos.pos);
    memmove(pos.pos, next_pos, deque->finish - next_pos);
    deque->finish -= deque->value_type->size();
    assert(__check_deque_state(deque));

    return pos;
}

c_deque_iterator_t c_deque_erase_range(
    c_deque_t* deque, c_deque_iterator_t first, c_deque_iterator_t last)
{
    if (!deque) return last;

    if (!__is_valid_pos(deque, first.pos) ||
        !__is_valid_pos(deque, last.pos) ||
        (first.pos > last.pos)) {
        return c_deque_end(deque);
    }

    if (first.pos == last.pos) return last;

    __destroy(first, last);
    size_t size = deque->finish - last.pos;
    memmove(first.pos, last.pos, size);
    deque->finish -= (last.pos - first.pos);
    assert(__check_deque_state(deque));

    return first;
}

void c_deque_push_back(c_deque_t* deque, c_ref_t value)
{
    if (!deque || !value) return;

    if (deque->finish == deque->end_of_storage) {
        if (__reallocate_and_move(deque, 1 * 2)) return;
    }

    deque->value_type->copy(deque->finish, value);
    deque->finish += deque->value_type->size();
    assert(__check_deque_state(deque));
}

void c_deque_pop_back(c_deque_t* deque)
{
    if (!c_deque_empty(deque)) {
        deque->value_type->destroy(c_deque_back(deque));
        deque->finish -= deque->value_type->size();
        assert(__check_deque_state(deque));
    }
}

void c_deque_push_front(c_deque_t* deque, c_ref_t value)
{
    if (!deque || !value) return;

    if (deque->start == deque->start_of_storage) {
        if (__reallocate_and_move(deque, 1 * 2)) return;
    }

    deque->start -= deque->value_type->size();
    deque->value_type->copy(deque->start, value);
    assert(__check_deque_state(deque));
}

void c_deque_pop_front(c_deque_t* deque)
{
    if (!c_deque_empty(deque)) {
        deque->value_type->destroy(c_deque_front(deque));
        deque->start += deque->value_type->size();
        assert(__check_deque_state(deque));
    }
}

void c_deque_resize(c_deque_t* deque, size_t count)
{
    c_deque_resize_with_value(deque, count, 0);
}

void c_deque_resize_with_value(c_deque_t* deque, size_t count, c_ref_t value)
{
    if (!deque) return;

    size_t value_size = deque->value_type->size();

    if (count > c_deque_size(deque)) {
        count -= c_deque_size(deque);
        if (count > __available(deque)) {
            // make sure the end available part is large enough
            if (__reallocate_and_move(deque, count * 2)) return;
        }

        __insert_aux(deque, c_deque_end(deque), count, value);
    }
    else {
        c_ref_t pos = deque->start + count * value_size;
        __destroy(__create_iterator(deque->value_type, pos), c_deque_end(deque));
        deque->finish = pos;
        assert(__check_deque_state(deque));
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
c_backend_container_t* c_deque_create_backend(const c_type_info_t* value_type)
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

    backend->impl = c_deque_create(value_type);
    if (!backend->impl) {
        __c_free(backend);
        return 0;
    }

    backend->interface.ops = &backend_deque_ops;

    return (c_backend_container_t*)backend;
}
