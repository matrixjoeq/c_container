

#ifndef __C_ITERATOR_H__
#define __C_ITERATOR_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

c_iterator_t* c_iter_increment(c_iterator_t* iter);
c_iterator_t* c_iter_decrement(c_iterator_t* iter);
c_ref_t c_iter_dereference(c_iterator_t* iter);
bool c_iter_equal(c_iterator_t* x, c_iterator_t* y);
bool c_iter_not_equal(c_iterator_t* x, c_iterator_t* y);

#define C_ITER_INC(x) c_iter_increment((c_iterator_t*)(x))
#define C_ITER_DEC(x) c_iter_decrement((c_iterator_t*)(x))
#define C_ITER_DEREF(x) c_iter_dereference((c_iterator_t*)(x))
#define C_ITER_EQ(x, y) c_iter_equal((c_iterator_t*)(x), (c_iterator_t*)(y))
#define C_ITER_NE(x, y) c_iter_not_equal((c_iterator_t*)(x), (c_iterator_t*)(y))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ITERATOR_H__
