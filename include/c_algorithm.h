

#ifndef __C_ALGORITHM_H__
#define __C_ALGORITHM_H__

#include "c_def.h"
#include "c_iterator.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// non-modifying sequence operations
size_t c_algo_count(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_ref_t data);
size_t c_algo_count_if(c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last, c_unary_predicate pred);

// modifying sequence operations
void c_algo_swap(const c_containable_t* type_info, c_ref_t x, c_ref_t y);
void c_algo_iter_swap(c_iterator_t* __c_forward_iterator x, c_iterator_t* __c_forward_iterator y);

// partition operations
// sorting operations
// binary search operations (on sorted ranges)
// set operations (on sorted ranges)
// heap operations
bool c_algo_is_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void c_algo_is_heap_until_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last,
                             c_iterator_t** __c_random_iterator until, c_compare comp);
void c_algo_make_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void c_algo_push_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void c_algo_pop_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);
void c_algo_sort_heap_by(c_iterator_t* __c_random_iterator first, c_iterator_t* __c_random_iterator last, c_compare comp);

// heap helpers
#define c_algo_is_heap(first, last) c_algo_is_heap_by(C_ITER_T(first), C_ITER_T(last), __c_get_less(first))
#define c_algo_is_heap_until(first, last, until) c_algo_is_heap_until_by(C_ITER_T(first), C_ITER_T(last), (until), __c_get_less(first))
#define c_algo_make_heap(first, last) c_algo_make_heap_by(C_ITER_T(first), C_ITER_T(last), __c_get_less(first))
#define c_algo_push_heap(first, last) c_algo_push_heap_by(C_ITER_T(first), C_ITER_T(last), __c_get_less(first))
#define c_algo_pop_heap(first, last) c_algo_pop_heap_by(C_ITER_T(first), C_ITER_T(last), __c_get_less(first))
#define c_algo_sort_heap(first, last) c_algo_sort_heap_by(C_ITER_T(first), C_ITER_T(last), __c_get_less(first))

// minimum/maximum operations
// numeric operations
// operations on uninitialized memory

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ALGORITHM_H__
