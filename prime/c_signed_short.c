
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_signed_short_size(void)
{
    return sizeof(signed short);
}

__c_static __c_inline void c_signed_short_create(c_ref_t obj)
{
    *((signed short*)obj) = 0;
}

__c_static __c_inline void c_signed_short_copy(c_ref_t dst, const c_ref_t src)
{
    *((signed short*)dst) = *((signed short*)src);
}

__c_static __c_inline void c_signed_short_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_signed_short_assign(c_ref_t dst, const c_ref_t src)
{
    *((signed short*)dst) = *((signed short*)src);
    return dst;
}

__c_static __c_inline bool c_signed_short_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(signed short*)lhs) < *((signed short*)rhs);
}

__c_static __c_inline bool c_signed_short_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(signed short*)lhs) == (*(signed short*)rhs);
}

c_containable_t* c_get_sshort_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_signed_short_size,
        .create = c_signed_short_create,
        .copy = c_signed_short_copy,
        .destroy = c_signed_short_destroy,
        .assign = c_signed_short_assign,
        .less = c_signed_short_less,
        .equal = c_signed_short_equal
    };

    return &type_info;
}
