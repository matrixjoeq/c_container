
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

void c_algo_swap(const c_containable_t* type_info, c_ref_t x, c_ref_t y)
{
	if (!type_info || !x || !y) return;
	assert(type_info->size);
	assert(type_info->copy);
	
	c_ref_t tmp = malloc(type_info->size());
	if (tmp) {
		type_info->copy(tmp, x);
		type_info->copy(x, y);
		type_info->copy(y, tmp);
	}
	__c_free(tmp);
}

void c_algo_iter_swap(c_iterator_t* x, c_iterator_t* y)
{
	if (!x || !y) return;
	assert(C_ITER_AT_LEAST(x, C_ITER_CATE_FORWARD));
	assert(C_ITER_AT_LEAST(y, C_ITER_CATE_FORWARD));
	c_algo_swap(x->type_info, C_ITER_DEREF(x), C_ITER_DEREF(y));
}
