
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_signed_char_size(void)
{
	return sizeof(signed char);
}

__c_static __c_inline void c_signed_char_create(c_ref_t obj)
{
	*((signed char*)obj) = 0;
}

__c_static __c_inline void c_signed_char_copy(c_ref_t dst, const c_ref_t src)
{
	*((signed char*)dst) = *((signed char*)src);
}

__c_static __c_inline void c_signed_char_destroy(c_ref_t obj)
{
	__c_unuse(obj);
}

__c_static __c_inline c_ref_t c_signed_char_assign(c_ref_t dst, const c_ref_t src)
{
	*((signed char*)dst) = *((signed char*)src);
	return dst;
}

__c_static __c_inline bool c_signed_char_less(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed char*)lhs) < *((signed char*)rhs);
}

__c_static __c_inline bool c_signed_char_equal(const c_ref_t lhs, const c_ref_t rhs)
{
	return (*(signed char*)lhs) == (*(signed char*)rhs);
}

const c_containable_t c_signed_char_t = {
	.size = c_signed_char_size,
	.create = c_signed_char_create,
	.copy = c_signed_char_copy,
	.destroy = c_signed_char_destroy,
	.assign = c_signed_char_assign,
	.less = c_signed_char_less,
	.equal = c_signed_char_equal
};
