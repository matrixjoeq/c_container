
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_long_size(void)
{
    return sizeof(long);
}

__c_static __c_inline void c_long_create(c_ref_t obj)
{
    *((long*)obj) = 0l;
}

__c_static __c_inline void c_long_copy(c_ref_t dst, const c_ref_t src)
{
    *((long*)dst) = *((long*)src);
}

__c_static __c_inline void c_long_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_long_assign(c_ref_t dst, const c_ref_t src)
{
    *((long*)dst) = *((long*)src);
    return dst;
}

__c_static __c_inline bool c_long_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(long*)lhs) < *((long*)rhs);
}

__c_static __c_inline bool c_long_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(long*)lhs) == (*(long*)rhs);
}

c_containable_t* c_get_long_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_long_size,
        .create = c_long_create,
        .copy = c_long_copy,
        .destroy = c_long_destroy,
        .assign = c_long_assign,
        .less = c_long_less,
        .equal = c_long_equal
    };

    return &type_info;
}
