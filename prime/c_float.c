
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_float_size(void)
{
	return sizeof(float);
}

__c_static __c_inline void c_float_create(c_ref_t obj)
{
	*((float*)obj) = 0.0f;
}

__c_static __c_inline void c_float_copy(c_ref_t dst, const c_ref_t src)
{
	*((float*)dst) = *((float*)src);
}

__c_static __c_inline void c_float_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_float_assign(c_ref_t dst, const c_ref_t src)
{
	*((float*)dst) = *((float*)src);
	return dst;
}

__c_static __c_inline bool c_float_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(float*)lhs) < *((float*)rhs);
}

__c_static __c_inline bool c_float_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(float*)lhs) == (*(float*)rhs);
}

const c_containable_t c_float_t = {
	.size = c_float_size,
	.create = c_float_create,
	.copy = c_float_copy,
	.destroy = c_float_destroy,
	.assign = c_float_assign,
	.less = c_float_less,
	.equal = c_float_equal
};
