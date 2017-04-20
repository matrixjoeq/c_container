

#ifndef __C_ALGORITHM_H__
#define __C_ALGORITHM_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// non-modifying sequence operations
bool algo_all_of(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_predicate pred);
bool algo_any_of(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_predicate pred);
bool algo_none_of(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_predicate pred);
void algo_for_each(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_func func);
void algo_for_each_n(c_iterator_t* __c_input_iterator first, size_t n, c_iterator_t** __c_input_iterator reach, c_unary_func func);
size_t algo_count(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_ref_t data);
size_t algo_count_if(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_predicate pred);
// TODO: algo_mismatch()
bool algo_equal_by(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last,
                   c_iterator_t* __c_input_iterator first2, c_binary_predicate pred);
bool algo_find_by(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found, const c_ref_t value,
                  c_binary_predicate pred);
bool algo_find_if(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found, c_unary_predicate pred);
bool algo_find_if_not(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last,
                      c_iterator_t** __c_input_iterator found, c_unary_predicate pred);
// TODO: algo_find_end()
bool algo_find_first_of_by(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last,
                           c_iterator_t* __c_forward_iterator s_first, c_iterator_t* __c_forward_iterator s_last,
                           c_iterator_t** __c_input_iterator found, c_binary_predicate pred);
bool algo_adjacent_find_by(c_iterator_t* __c_forward_iterator first, c_iterator_t* __c_forward_iterator last,
                           c_iterator_t** __c_forward_iterator found, c_binary_predicate pred);
bool algo_search_by(c_iterator_t* __c_forward_iterator first, c_iterator_t* __c_forward_iterator last,
                    c_iterator_t* __c_forward_iterator s_first, c_iterator_t* __c_forward_iterator s_last,
                    c_iterator_t** __c_forward_iterator found, c_binary_predicate pred);
bool algo_search_n_by(c_iterator_t* __c_forward_iterator first, c_iterator_t* __c_forward_iterator last,
                      size_t n, const c_ref_t value, c_iterator_t** __c_forward_iterator found,
                      c_binary_predicate pred);

// non-modifying helpers
#define c_algo_all_of(x, y, p)              algo_all_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_any_of(x, y, p)              algo_any_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_none_of(x, y, p)             algo_none_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_for_each(x, y, f)            algo_for_each(C_ITER_T(x), C_ITER_T(y), (f))
#define c_algo_for_each_n(x, n, r, f)       algo_for_each_n(C_ITER_T(x), (n), C_ITER_PTR(r), (f))
#define c_algo_count(x, y, v)               algo_count(C_ITER_T(x), C_ITER_T(y), C_REF_T(v))
#define c_algo_count_if(x, y, p)            algo_count_if(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_equal_by(x, y, z, p)         algo_equal_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(z), (p))
#define c_algo_find_by(x, y, v, p)          algo_find_by(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), (p))
#define c_algo_find_if(x, y, f, p)          algo_find_if(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_find_if_not(x, y, f, p)      algo_find_if_not(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_find_first_of_by(x, y, sx, sy, f, p) \
    algo_find_first_of_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_adjacent_find_by(x, y, f, p) algo_adjacent_find_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_search_by(x, y, sx, sy, f, p) \
    algo_search_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_search_n_by(x, y, n, v, f, p) \
    algo_search_n_by(C_ITER_T(x), C_ITER_T(y), (n), C_REF_T(v), C_ITER_PTR(f), (p))

#define c_algo_equal(x, y, z)                   c_algo_equal_by((x), (y), (z), __c_get_equal(x))
#define c_algo_find(x, y, v)                    c_algo_find_by((x), (y), (v), __c_get_equal(x))
#define c_algo_find_first_of(x, y, sx, sy, f)   c_algo_find_first_of_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_adjacent_find(x, y, f)           c_algo_adjacent_find_by((x), (y), (f), __c_get_equal(x))
#define c_algo_search(x, y, sx, sy, f)          c_algo_search_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_search_n(x, y, n, v, f)          c_algo_search_n_by((x), (y), (n), (v), (f), __c_get_equal(x))

// modifying sequence operations
void algo_swap(c_containable_t* type_info, c_ref_t x, c_ref_t y);
void algo_iter_swap(c_iterator_t* __c_forward_iterator x, c_iterator_t* __c_forward_iterator y);

// modifying helpers
#define c_algo_swap(t, x, y)    algo_swap((t), C_REF_T(x), C_REF_T(y))
#define c_algo_iter_swap(x, y)  algo_iter_swap(C_ITER_T(x), C_ITER_T(y))

// partition operations
// sorting operations
// binary search operations (on sorted ranges)
// set operations (on sorted ranges)
// heap operations
bool algo_is_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void algo_is_heap_until_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last,
                           c_iterator_t** __c_random_iterator until, c_compare comp);
void algo_make_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void algo_push_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void algo_pop_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void algo_sort_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);

// heap helpers
#define c_algo_is_heap_by(x, y, c)              algo_is_heap_by(C_ITER_T(x), C_ITER_T(y), c)
#define c_algo_is_heap_until_by(x, y, z, c)     algo_is_heap_until_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(z), (c))
#define c_algo_make_heap_by(x, y, c)            algo_make_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_push_heap_by(x, y, c)            algo_push_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_pop_heap_by(x, y, c)             algo_pop_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_sort_heap_by(x, y, c)            algo_sort_heap_by(C_ITER_T(x), C_ITER_T(y), (c))

#define c_algo_is_heap(x, y)                    c_algo_is_heap_by((x), (y), __c_get_less(x))
#define c_algo_is_heap_until(x, y, z)           c_algo_is_heap_until_by((x), (y), (z), __c_get_less(x))
#define c_algo_make_heap(x, y)                  c_algo_make_heap_by((x), (y), __c_get_less(x))
#define c_algo_push_heap(x, y)                  c_algo_push_heap_by((x), (y), __c_get_less(x))
#define c_algo_pop_heap(x, y)                   c_algo_pop_heap_by((x), (y), __c_get_less(x))
#define c_algo_sort_heap(x, y)                  c_algo_sort_heap_by((x), (y), __c_get_less(x))

// minimum/maximum operations
// numeric operations
// operations on uninitialized memory

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ALGORITHM_H__
