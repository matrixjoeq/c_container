

#ifndef __C_ALGORITHM_H__
#define __C_ALGORITHM_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// non-modifying sequence operations
size_t c_algo_count(c_iterator_t* input_first, c_iterator_t* input_last, c_ref_t data);
size_t c_algo_count_if(c_iterator_t* input_first, c_iterator_t* input_last, c_unary_predicate pred);

// modifying sequence operations
void c_algo_swap(const c_containable_t* type_info, c_ref_t x, c_ref_t y);
void c_algo_iter_swap(c_iterator_t* forward_x, c_iterator_t* forward_y);

// partition operations
// sorting operations
// binary search operations (on sorted ranges)
// set operations (on sorted ranges)
// heap operations
void c_algo_make_heap(c_iterator_t* random_first, c_iterator_t* random_last);
void c_algo_make_heap_by(c_iterator_t* random_first, c_iterator_t* random_last, c_compare comp);

// minimum/maximum operations
// numeric operations
// operations on uninitialized memory

#ifdef __cplusplus
}
#endif // __cplusplus
	
#endif // __C_ALGORITHM_H__
