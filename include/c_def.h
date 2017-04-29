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

#ifndef __C_DEF_H__
#define __C_DEF_H__

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define __c_in
#define __c_out
#define __c_in_out

typedef enum __c_iterator_category {
    C_ITER_CATE_OUTPUT,
    C_ITER_CATE_INPUT,
    C_ITER_CATE_FORWARD,
    C_ITER_CATE_BIDIRECTION,
    C_ITER_CATE_RANDOM,
} c_iterator_category_t;

typedef enum __c_iterator_type {
    C_ITER_TYPE_FORWARD_LIST,
    C_ITER_TYPE_LIST,
    C_ITER_TYPE_LIST_REVERSE,
    C_ITER_TYPE_TREE,
    C_ITER_TYPE_TREE_REVERSE,
    C_ITER_TYPE_SET,
    C_ITER_TYPE_SET_REVERSE,
    C_ITER_TYPE_MULTISET,
    C_ITER_TYPE_MULTISET_REVERSE,
    C_ITER_TYPE_MAP,
    C_ITER_TYPE_MAP_REVERSE,
    C_ITER_TYPE_MULTIMAP,
    C_ITER_TYPE_MULTIMAP_REVERSE,
    C_ITER_TYPE_VECTOR,
    C_ITER_TYPE_VECTOR_REVERSE,
    C_ITER_TYPE_DEQUE,
    C_ITER_TYPE_DEQUE_REVERSE,
} c_iterator_type_t;

typedef void* c_ref_t;
typedef void* c_storage_t;

typedef void (*c_unary_func)(c_ref_t __c_in_out);
//typedef void (*c_binary_func)(c_ref_t, c_ref_t);

// generate any type in the input address
typedef void (*c_generator_emplace)(c_ref_t __c_in_out);

// generate random value in the interval [0, n)
typedef unsigned long long c_random_int_t;
typedef c_random_int_t (*c_random_func)(c_random_int_t n);

// return true if data matches condition
typedef bool (*c_unary_predicate)(c_ref_t __c_in);
typedef bool (*c_binary_predicate)(c_ref_t __c_in, c_ref_t __c_in);

// return true if compare(lhs, rhs)
typedef bool (*c_compare)(c_ref_t __c_in lhs, c_ref_t __c_in rhs);

typedef struct __c_containable {
    // size information
    size_t (*size)(void);
    // default constructor
    void (*create)(c_ref_t __c_out obj);
    // copy constructor
    void (*copy)(c_ref_t __c_out dst, c_ref_t __c_in src);
    // destructor
    void (*destroy)(c_ref_t __c_in_out obj);
    // operator=
    c_ref_t (*assign)(c_ref_t __c_out dst, c_ref_t __c_in src);
    // operator<
    bool (*less)(c_ref_t __c_in lhs, c_ref_t __c_in rhs);
    // operator==
    bool (*equal)(c_ref_t __c_in lhs, c_ref_t __c_in rhs);
} c_containable_t;

struct __c_iterator;
typedef struct __c_iterator_operation {
    // copy
    // needed for algorithms, since iterators passed to algorithms are by reference
    // algorithms need to make a copy first
    // however, it does not need a destructor, because iterators are "smart pointers"
    // refer to a position in a container, they don't occupy any resources actually
    void (*alloc_and_copy)(struct __c_iterator** __c_out dst,
                           struct __c_iterator* __c_in src);

    // assign
    struct __c_iterator* (*assign)(struct __c_iterator* __c_in_out self,
                                   struct __c_iterator* __c_in other);

    // operator++
    // return self
    struct __c_iterator* (*increment)(struct __c_iterator* __c_in_out self);

    // operator-- (bidirection and random only)
    // return self
    struct __c_iterator* (*decrement)(struct __c_iterator* __c_in_out self);

    // operator++(int)
    // tmp stores the iterator before increment
    // return *tmp
    struct __c_iterator* (*post_increment)(struct __c_iterator* __c_in_out self,
                                           struct __c_iterator** __c_out tmp);

    // operator--(int)
    // tmp stores the iterator before decrement
    // return *tmp
    struct __c_iterator* (*post_decrement)(struct __c_iterator* __c_in_out self,
                                           struct __c_iterator** __c_out tmp);

    // operator*
    c_ref_t (*dereference)(struct __c_iterator* __c_in self);

    // operator==
    bool (*equal)(struct __c_iterator* __c_in self,
                  struct __c_iterator* __c_in other);

    // operator!=
    bool (*not_equal)(struct __c_iterator* __c_in self,
                      struct __c_iterator* __c_in other);

    // advance
    void (*advance)(struct __c_iterator* __c_in_out self,
                    ptrdiff_t __c_in n);

    // distance
    ptrdiff_t (*distance)(struct __c_iterator* __c_in first,
                          struct __c_iterator* __c_in last);
} c_iterator_operation_t;

typedef struct __c_iterator {
    c_iterator_category_t iterator_category;
    c_iterator_type_t iterator_type;
    c_iterator_operation_t* iterator_ops;
    c_containable_t* type_info;
} c_iterator_t;

struct __c_backend_container;
typedef struct __c_backend_operation {
    // destructor
    void (*destroy)(struct __c_backend_container* __c_in_out self);

    // element access
    c_ref_t (*front)(struct __c_backend_container* __c_in self);
    c_ref_t (*back)(struct __c_backend_container* __c_in self);

    // iterator
    // set begin of self to *iter and return *iter
    c_iterator_t* (*begin)(struct __c_backend_container* __c_in self, c_iterator_t** __c_out iter);
    // set end of self to *iter and return *iter
    c_iterator_t* (*end)(struct __c_backend_container* __c_in self, c_iterator_t** __c_out iter);

    // capacity
    bool (*empty)(struct __c_backend_container* __c_in self);
    size_t (*size)(struct __c_backend_container* __c_in self);
    size_t (*max_size)(void);

    // modifier
    void (*push_back)(struct __c_backend_container* __c_in_out self, c_ref_t __c_in data);
    void (*pop_back)(struct __c_backend_container* __c_in_out self);
    void (*push_front)(struct __c_backend_container* __c_in_out self, c_ref_t __c_in data);
    void (*pop_front)(struct __c_backend_container* __c_in_out self);
    void (*swap)(struct __c_backend_container* __c_in_out self, struct __c_backend_container* __c_in_out other);
} c_backend_operation_t;

typedef struct __c_backend_container {
    c_backend_operation_t* ops;
} c_backend_container_t;

typedef c_backend_container_t* (*BackendContainerCreator)(c_containable_t* type_info);

c_containable_t* c_get_sint_type_info(void);
c_containable_t* c_get_uint_type_info(void);
c_containable_t* c_get_int_type_info(void);
c_containable_t* c_get_sshort_type_info(void);
c_containable_t* c_get_ushort_type_info(void);
c_containable_t* c_get_short_type_info(void);
c_containable_t* c_get_slong_type_info(void);
c_containable_t* c_get_ulong_type_info(void);
c_containable_t* c_get_long_type_info(void);
c_containable_t* c_get_schar_type_info(void);
c_containable_t* c_get_uchar_type_info(void);
c_containable_t* c_get_char_type_info(void);
c_containable_t* c_get_float_type_info(void);
c_containable_t* c_get_double_type_info(void);

#define C_REF_T(x)      ((c_ref_t)(x))
#define C_ITER_T(x)     ((c_iterator_t*)(x))
#define C_ITER_PTR(x)   ((c_iterator_t**)(x))

#define C_CONV_TYPE(type, x)    ((type*)(x))
#define C_DEREF_TYPE(type, x)   (*(type*)(x))

#define C_DEREF_INT(x)      C_DEREF_TYPE(int, (x))
#define C_DEREF_SINT(x)     C_DEREF_TYPE(signed int, (x))
#define C_DEREF_UINT(x)     C_DEREF_TYPE(unsigned int, (x))
#define C_DEREF_SHORT(x)    C_DEREF_TYPE(short, (x))
#define C_DEREF_SSHORT(x)   C_DEREF_TYPE(signed short, (x))
#define C_DEREF_USHORT(x)   C_DEREF_TYPE(unsigned short, (x))
#define C_DEREF_LONG(x)     C_DEREF_TYPE(long, (x))
#define C_DEREF_SLONG(x)    C_DEREF_TYPE(signed long, (x))
#define C_DEREF_ULONG(x)    C_DEREF_TYPE(unsigned long, (x))
#define C_DEREF_CHAR(x)     C_DEREF_TYPE(char, (x))
#define C_DEREF_SCHAR(x)    C_DEREF_TYPE(signed char, (x))
#define C_DEREF_UCHAR(x)    C_DEREF_TYPE(unsigned char, (x))
#define C_DEREF_FLOAT(x)    C_DEREF_TYPE(float, (x))
#define C_DEREF_DOUBLE(x)   C_DEREF_TYPE(double, (x))

#define C_ITER_COPY(x, y)       C_ITER_T(y)->iterator_ops->alloc_and_copy(C_ITER_PTR(x), C_ITER_T(y))
#define C_ITER_ASSIGN(x, y)     C_ITER_T(x)->iterator_ops->assign(C_ITER_T(x), C_ITER_T(y))
#define C_ITER_INC(x)           C_ITER_T(x)->iterator_ops->increment(C_ITER_T(x))
#define C_ITER_DEC(x)           C_ITER_T(x)->iterator_ops->decrement(C_ITER_T(x))
#define C_ITER_DEREF(x)         C_ITER_T(x)->iterator_ops->dereference(C_ITER_T(x))
#define C_ITER_EQ(x, y)         C_ITER_T(x)->iterator_ops->equal(C_ITER_T(x), C_ITER_T(y))
#define C_ITER_NE(x, y)         C_ITER_T(x)->iterator_ops->not_equal(C_ITER_T(x), C_ITER_T(y))
#define C_ITER_ADVANCE(x, n)    C_ITER_T(x)->iterator_ops->advance(C_ITER_T(x), (n))
#define C_ITER_DISTANCE(x, y)   C_ITER_T(x)->iterator_ops->distance(C_ITER_T(x), C_ITER_T(y))

#define C_ITER_AT_LEAST(x, c)   (C_ITER_T(x)->iterator_category >= (c_iterator_category_t)(c))
#define C_ITER_EXACT(x, c)      (C_ITER_T(x)->iterator_category == (c_iterator_category_t)(c))

#define C_ITER_V_ASSIGN_DEREF(v, x) C_ITER_T(x)->type_info->assign(C_REF_T(v), C_ITER_DEREF(C_ITER_T(x)))
#define C_ITER_DEREF_ASSIGN_V(x, v) C_ITER_T(x)->type_info->assign(C_ITER_DEREF(C_ITER_T(x)), C_REF_T(v))
#define C_ITER_DEREF_ASSIGN(x, y)   C_ITER_T(x)->type_info->assign(C_ITER_DEREF(C_ITER_T(x)), C_ITER_DEREF(C_ITER_T(y)))

#define C_ITER_DEREF_EQUAL_V(x, v)  C_ITER_T(x)->type_info->equal(C_ITER_DEREF(C_ITER_T(x)), C_REF_T(v))
#define C_ITER_DEREF_EQUAL(x, y)    C_ITER_T(x)->type_info->equal(C_ITER_DEREF(C_ITER_T(x)), C_ITER_DEREF(C_ITER_T(y)))

#define C_ITER_V_LESS_DEREF(v, x)   C_ITER_T(x)->type_info->less(C_REF_T(v), C_ITER_DEREF(C_ITER_T(x)))
#define C_ITER_DEREF_LESS_V(x, v)   C_ITER_T(x)->type_info->less(C_ITER_DEREF(C_ITER_T(x)), C_REF_T(v))
#define C_ITER_DEREF_LESS(x, y)     C_ITER_T(x)->type_info->less(C_ITER_DEREF(C_ITER_T(x)), C_ITER_DEREF(C_ITER_T(y)))

#define __c_output_iterator
#define __c_input_iterator
#define __c_forward_iterator
#define __c_bidirection_iterator
#define __c_random_iterator

#define __c_is_same(x, y)       ((!(x) || !(y)) ? false : memcmp((x), (y), sizeof(*(x))) == 0)
#define __c_get_less(iter)      C_ITER_T(iter)->type_info->less
#define __c_get_equal(iter)     C_ITER_T(iter)->type_info->equal

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_DEF_H__
