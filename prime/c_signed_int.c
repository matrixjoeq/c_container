
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_signed_int_size(void)
{
	return sizeof(signed int);
}

__c_static __c_inline void c_signed_int_create(c_ref_t obj)
{
	*((signed int*)obj) = 0;
}

__c_static __c_inline void c_signed_int_copy(c_ref_t dst, const c_ref_t src)
{
	*((signed int*)dst) = *((signed int*)src);
}

__c_static __c_inline void c_signed_int_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_signed_int_assign(c_ref_t dst, const c_ref_t src)
{
	*((signed int*)dst) = *((signed int*)src);
	return dst;
}

__c_static __c_inline bool c_signed_int_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed int*)lhs) < *((signed int*)rhs);
}

__c_static __c_inline bool c_signed_int_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed int*)lhs) == (*(signed int*)rhs);
}

const c_containable_t c_signed_int_t = {
	.size = c_signed_int_size,
	.create = c_signed_int_create,
	.copy = c_signed_int_copy,
	.destroy = c_signed_int_destroy,
	.assign = c_signed_int_assign,
	.less = c_signed_int_less,
	.equal = c_signed_int_equal
};
