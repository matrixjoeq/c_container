
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_short_size(void)
{
    return sizeof(short);
}

__c_static __c_inline void c_short_create(c_ref_t obj)
{
    *((short*)obj) = 0;
}

__c_static __c_inline void c_short_copy(c_ref_t dst, const c_ref_t src)
{
    *((short*)dst) = *((short*)src);
}

__c_static __c_inline void c_short_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_short_assign(c_ref_t dst, const c_ref_t src)
{
    *((short*)dst) = *((short*)src);
    return dst;
}

__c_static __c_inline bool c_short_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(short*)lhs) < *((short*)rhs);
}

__c_static __c_inline bool c_short_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(short*)lhs) == (*(short*)rhs);
}

c_containable_t* c_get_short_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_short_size,
        .create = c_short_create,
        .copy = c_short_copy,
        .destroy = c_short_destroy,
        .assign = c_short_assign,
        .less = c_short_less,
        .equal = c_short_equal
    };

    return &type_info;
}
