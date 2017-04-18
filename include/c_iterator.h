

#ifndef __C_ITERATOR_H__
#define __C_ITERATOR_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define __c_output_iterator
#define __c_input_iterator
#define __c_forward_iterator
#define __c_bidirection_iterator
#define __c_random_iterator

#define __c_get_less(iter) (C_ITER_T(iter))->type_info->less

bool type_info_equal(c_containable_t* x, c_containable_t* y);
void c_iter_copy(c_iterator_t** iter, c_iterator_t* other);
c_iterator_t* c_iter_increment(c_iterator_t* iter);
c_iterator_t* c_iter_decrement(c_iterator_t* iter);
c_ref_t c_iter_dereference(c_iterator_t* iter);
bool c_iter_equal(c_iterator_t* x, c_iterator_t* y);
bool c_iter_not_equal(c_iterator_t* x, c_iterator_t* y);
size_t c_iter_distance(c_iterator_t* first, c_iterator_t* last);
bool c_iter_category_at_least(c_iterator_t* iter, c_iterator_category_t cate);
bool c_iter_category_exact(c_iterator_t* iter, c_iterator_category_t cate);

#define C_ITER_COPY(x, y) c_iter_copy(&(x), (c_iterator_t*)(y))
#define C_ITER_INC(x) c_iter_increment((c_iterator_t*)(x))
#define C_ITER_DEC(x) c_iter_decrement((c_iterator_t*)(x))
#define C_ITER_DEREF(x) c_iter_dereference((c_iterator_t*)(x))
#define C_ITER_EQ(x, y) c_iter_equal((c_iterator_t*)(x), (c_iterator_t*)(y))
#define C_ITER_NE(x, y) c_iter_not_equal((c_iterator_t*)(x), (c_iterator_t*)(y))
#define C_ITER_DISTANCE(x, y) c_iter_distance((c_iterator_t*)(x), (c_iterator_t*)(y))
#define C_ITER_AT_LEAST(x, c) c_iter_category_at_least((c_iterator_t*)(x), (c_iterator_category_t)(c))
#define C_ITER_EXACT(x, c) c_iter_category_exact((c_iterator_t*)(x), (c_iterator_category_t)(c))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ITERATOR_H__
