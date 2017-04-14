
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_unsigned_short_size(void)
{
	return sizeof(unsigned short);
}

__c_static __c_inline void c_unsigned_short_create(c_ref_t obj)
{
	*((unsigned short*)obj) = 0u;
}

__c_static __c_inline void c_unsigned_short_copy(c_ref_t dst, const c_ref_t src)
{
	*((unsigned short*)dst) = *((unsigned short*)src);
}

__c_static __c_inline void c_unsigned_short_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_unsigned_short_assign(c_ref_t dst, const c_ref_t src)
{
	*((unsigned short*)dst) = *((unsigned short*)src);
	return dst;
}

__c_static __c_inline bool c_unsigned_short_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(unsigned short*)lhs) < *((unsigned short*)rhs);
}

__c_static __c_inline bool c_unsigned_short_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(unsigned short*)lhs) == (*(unsigned short*)rhs);
}

const c_containable_t c_unsigned_short_t = {
	.size = c_unsigned_short_size,
	.create = c_unsigned_short_create,
	.copy = c_unsigned_short_copy,
	.destroy = c_unsigned_short_destroy,
	.assign = c_unsigned_short_assign,
	.less = c_unsigned_short_less,
	.equal = c_unsigned_short_equal
};