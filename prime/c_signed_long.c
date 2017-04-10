
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_signed_long_size(void)
{
	return sizeof(signed long);
}

__c_static __c_inline void c_signed_long_create(c_ref_t obj)
{
	*((signed long*)obj) = 0l;
}

__c_static __c_inline void c_signed_long_copy(c_ref_t dst, const c_ref_t src)
{
	*((signed long*)dst) = *((signed long*)src);
}

__c_static __c_inline void c_signed_long_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_signed_long_assign(c_ref_t dst, const c_ref_t src)
{
	*((signed long*)dst) = *((signed long*)src);
	return dst;
}

__c_static __c_inline bool c_signed_long_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed long*)lhs) < *((signed long*)rhs);
}

__c_static __c_inline bool c_signed_long_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed long*)lhs) == (*(signed long*)rhs);
}

const c_containable_t c_signed_long_t = {
	.size = c_signed_long_size,
	.create = c_signed_long_create,
	.copy = c_signed_long_copy,
	.destroy = c_signed_long_destroy,
	.assign = c_signed_long_assign,
	.less = c_signed_long_less,
	.equal = c_signed_long_equal
};
