
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_unsigned_long_size(void)
{
	return sizeof(unsigned long);
}

__c_static __c_inline void c_unsigned_long_create(c_ref_t obj)
{
	*((unsigned long*)obj) = 0ul;
}

__c_static __c_inline void c_unsigned_long_copy(c_ref_t dst, const c_ref_t src)
{
	*((unsigned long*)dst) = *((unsigned long*)src);
}

__c_static __c_inline void c_unsigned_long_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_unsigned_long_assign(c_ref_t dst, const c_ref_t src)
{
	*((unsigned long*)dst) = *((unsigned long*)src);
	return dst;
}

__c_static __c_inline bool c_unsigned_long_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(unsigned long*)lhs) < *((unsigned long*)rhs);
}

__c_static __c_inline bool c_unsigned_long_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(unsigned long*)lhs) == (*(unsigned long*)rhs);
}

const c_containable_t c_unsigned_long_t = {
	.size = c_unsigned_long_size,
	.create = c_unsigned_long_create,
	.copy = c_unsigned_long_copy,
	.destroy = c_unsigned_long_destroy,
	.assign = c_unsigned_long_assign,
	.less = c_unsigned_long_less,
	.equal = c_unsigned_long_equal
};
