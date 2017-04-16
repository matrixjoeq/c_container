
#include <assert.h>
#include <stdlib.h>
#include "c_def.h"
#include "c_internal.h"
#include "c_iterator.h"
#include "c_algorithm.h"

__c_static void adjust_heap(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
	__c_unuse(first);
	__c_unuse(last);
	__c_unuse(comp);
}

void c_algo_push_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
	__c_unuse(first);
	__c_unuse(last);
	__c_unuse(comp);
}

void c_algo_pop_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
	adjust_heap(first, last, comp);
}

void c_algo_make_heap(c_iterator_t* first, c_iterator_t* last)
{
	if (!first || !last) return;
	assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
	assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
	assert(first->type_info);
	c_algo_make_heap_by(first, last, first->type_info->less);
}

void c_algo_make_heap_by(c_iterator_t* first, c_iterator_t* last, c_compare comp)
{
	if (!first || !last || !comp) return;
	assert(C_ITER_EXACT(first, C_ITER_CATE_RANDOM));
	assert(C_ITER_EXACT(last, C_ITER_CATE_RANDOM));
	
	__C_ALGO_BEGIN
	__C_ALGO_END
}
